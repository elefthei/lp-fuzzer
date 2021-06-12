from pysmps import smps_loader as smps
import sys
import traceback
import itertools

var = 'X'
delta = 0.01

class Show:
    def show():
        raise ValueError("Not implemented")
    def __repr__(self):
        return self.show()
    def __str__(self):
        return self.show()

class Sign(Show):
    def __init__(self, s):
        sign = 0
        if(s == 'G'):
            sign = 1
        elif(s == 'E'):
            sign = 0
        elif(s == 'L'):
            sign = -1
        else:
            raise ValueError("Interpret sign")
        self.sign = sign

    def flip(self):
        s = self.sign
        if(s == 1):
            return Sign('L')
        elif(s == 0):
            return Sign('E')
        elif(s == -1):
            return Sign('G')
        else:
            raise ValueError("Flip sign")

    def bound(self):
        if self.sign == 0:
            return "FX"
        elif self.sign == 1:
            return "LO"
        elif self.sign == -1:
            return 'UP'
        else:
            raise ValueError("Wrong sign")

    def show(self):
        if self.sign == 0:
            return "=="
        elif self.sign == 1:
            return ">="
        elif self.sign == -1:
            return "<="
        else:
            raise ValueError("Wrong sign in show")

class Constraint(Show):
    def __init__(self, mult, sign, const):
        self.mult = mult
        self.sign = sign
        self.const = const

    def show(self):
        num_vars = len(self.mult)
        terms = [ None  if m == 0.000 else "{}{}".format(var,v) if m == 1.0 else "{:.3f}*{}{}".format(m, var,v) for (m,v) in zip(self.mult, range(num_vars)) ]
        terms = filter(lambda x: x != None, terms)
        return " + ".join(terms) + " " + self.sign.show() + " {:.3f}".format(self.const)

    def show_delta(self):
        num_vars = len(self.mult)
        terms = [ None  if m == 0.000 else "{}{}".format(var,v) if m == 1.0 else "{:.3f}*{}{}".format(m, var,v) for (m,v) in zip(self.mult, range(num_vars)) ]
        terms = filter(lambda x: x != None, terms)
        if self.sign.sign == 0:
            return "deq(" + ", ".join([" + ".join(terms), "{:.3f}".format(self.const), str(delta)]) + ")"
        elif self.sign.sign == 1:
            return "dge(" + ", ".join([" + ".join(terms), "{:.3f}".format(self.const), str(delta)]) + ")"
        elif self.sign.sign == -1:
            return "dle(" + ", ".join([" + ".join(terms), "{:.3f}".format(self.const), str(delta)]) + ")"
        else:
            raise ValueError("Wrong sign in show_delta " + self.show())

# Three types of range constraints x >= 0, x <= 0, x \in R (unbounded)
class RangeConstraint(Show):
    def __init__(self, num_var, b, v):
        if v == float('inf') or v == float('-inf'):
            self.bounded = False
        elif b == 'UP' and v <= 0.0:
            self.index = num_var
            self.sign = Sign('L')
            self.bounded = True
        elif b == 'LO' and v >= 0.0:
            self.index = num_var
            self.sign = Sign('G')
            self.bounded = True
        elif b == 'MI':
            self.index = num_var
            self.sign = Sign('L')
            self.bounded = True
        elif b == 'PL':
            self.index = num_var
            self.sign = Sign('G')
            self.bounded = True
        else:
            self.bounded = False

    def sign_flip(self):
        if self.bounded:
            return self.sign
        else:
            return Sign('E')

    def show(self):
        if(self.bounded):
            return "{}{} {} {:.3f}".format(var,self.index, self.sign.show(), 0.0)
        else:
            return ""

class ObjectiveConstraint(Show):
    def __init__(self, mult):
        self.mult = mult

    def show(self):
        num_vars = len(self.mult)
        terms = [ None  if m == 0.000 else "{}{}".format(var,v) if m == 1.0 else "{:.3f}*{}{}".format(m, var, v) for (m,v) in zip(self.mult, range(num_vars)) ]
        terms = filter(lambda x: x != None, terms)
        return " + ".join(terms)


def ccheckgen(constraints, range_constraints):
    return ",\n\t".join(
            [c.show_delta() for c in constraints if max(c.mult) != 0 and min(c.mult) != 0] +
            [c.show() for c in range_constraints if c.show() != ""]
        )

# substitution dict
def cfuncgen(constraints, objective, num_vars, max_min, range_constraints):
    C = """
    fp32 $vars;

    __GADGET_$maxmin(
      $objective, // objective
      $constraints, //constraints
      $rangeconstraints
    );
    """

    substitutions = {
        'vars' : ", ".join(["{}{} = __GADGET_exist()".format(var,i) for i in range(num_vars)]),
        'maxmin' : "minimize" if max_min == 0 else "maximize",
        'objective': objective.show(),
        'constraints': ",\n\t".join([c.show() for c in constraints if max(c.mult) != 0 and min(c.mult) != 0]),
        'rangeconstraints': ",\n\t".join([c.show() for c in range_constraints if c.show() != ""])
    }

    for (term, subst) in sorted(substitutions.items(), key=lambda k: -len(k[0])):
        C = C.replace("${}".format(term), subst)
    return C

def parse(filename):
    ex = smps.load_mps(filename)
    def rhs():
        if ex[8] == []:
            for d in ex[7]:
                yield 0.0
        else:
            for c in ex[9][ex[8][0]]:
                yield c

    # Is it max or min objective?
    min_max = 1
    num_vars = len(ex[3])
    pobj = ObjectiveConstraint(ex[6])
    r1, r2 = itertools.tee(rhs())
    dobj = ObjectiveConstraint(list(r1))
    pconstraints = []
    dconstraints = []

    for (mult, s, r) in zip(ex[7], ex[5], r2):
        pconstraints += [Constraint(mult, Sign(s), r)]

    prangeconstraints = []
    i = 0
    if ex[10] != []:
        BOUND = ex[10][0]
        for b in ex[11][BOUND]:
            for v in ex[11][BOUND][b]:
                c = RangeConstraint(i, b, v)
                prangeconstraints += [c]
                i+=1
    while(i < num_vars):
        prangeconstraints += [RangeConstraint(i, 'PL', 0.0)]
        i+=1

    for (mult, s, r) in zip(ex[7].transpose(), [c.sign_flip() for c in prangeconstraints], ex[6].transpose()):
        dconstraints += [ Constraint(mult, s, r) ]

    drangeconstraints = [ RangeConstraint(i, c.sign.flip().bound(), 0.0) for (i, c) in enumerate(pconstraints) ]
    return (
        pconstraints, pobj, num_vars, min_max, prangeconstraints,
        dconstraints, dobj, len(pconstraints), 1 - min_max, drangeconstraints
    )

# MAIN
(pconstraints, pobj, pnum_vars, pmin_max, prangeconstraints,
dconstraints, dobj, dnum_vars, dmin_max, drangeconstraints) = parse(sys.argv[1])

c_header = '''
typedef double fp32;

int deq(fp32 a, fp32 b, fp32 delta) {
    return ((-1 * delta) <= (a - b)) && ((a - b) <= delta);
}

int dge(fp32 a, fp32 b, fp32 delta) {
    return (a + delta) >= b;
}

int dle(fp32 a, fp32 b, fp32 delta) {
    return a <= (b + delta);
}

int main() {
'''

var = 'X'
primal = cfuncgen(
    pconstraints,
    pobj,
    pnum_vars,
    pmin_max,
    prangeconstraints
)
primal_obj = pobj.show()

var = 'Y'
dual = cfuncgen(
    dconstraints,
    dobj,
    dnum_vars,
    dmin_max,
    drangeconstraints
)
dual_obj = dobj.show()

check_header = "    int check = __GADGET_check("

var = 'X'
primal_check = ccheckgen(
    pconstraints,
    prangeconstraints
)

var = 'Y'
dual_check = ccheckgen(
    dconstraints,
    drangeconstraints
)

certificate = "deq({}, {}, {})".format(
    primal_obj,
    dual_obj,
    delta
);

c_foot = '''    );

    return check;
}
'''

print("\n".join([c_header, primal, dual, check_header, "\t" + primal_check +",\n\t" + dual_check + ",\n\t" + certificate, c_foot]))

