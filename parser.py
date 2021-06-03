from pysmps import smps_loader as smps
import sys
import traceback
import itertools

var = 'X'
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
        newsign = self
        newsign.sign = newsign.sign * -1
        return newsign

    def bound(self):
        if self.sign == 0:
            return "FX"
        elif self.sign == 1:
            return "LO"
        else:
            return 'UP'

    def show(self):
        if self.sign == 0:
            return "=="
        elif self.sign == 1:
            return "<="
        else:
            return ">="

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


class RangeConstraint(Show):
    def __init__(self, num_var, b, v):
        if v == float('inf') or v == float('-inf'):
            self.bounded = False
        elif b == 'UP':
            self.index = num_var
            self.sign = Sign('L')
            self.bound = v
            self.bounded = True
        elif b == 'LO':
            self.index = num_var
            self.sign = Sign('G')
            self.bound = v
            self.bounded = True
        elif b == 'FX':
            self.index = num_var
            self.sign = Sign('E')
            self.bound = v
            self.bounded = True
        elif b == 'MI':
            self.index = num_var
            self.sign = Sign('L')
            self.bound = 0.0
            self.bounded = True
        elif b == 'PL':
            self.index = num_var
            self.sign = Sign('G')
            self.bound = 0.0
            self.bounded = True
        else:
            self.bounded = False

    def sign_flip(self):
        if self.bounded:
            return self.sign.flip()
        else:
            return Sign('E')

    def show(self):
        if(self.bounded):
            return "{}{} {} {:.3f}".format(var,self.index, self.sign.show(), self.bound)
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
    return """
    __GADGET_check(
      {}
    );
    """.format(
        ",\n\t".join(
            [c.show() for c in constraints if max(c.mult) != 0 and min(c.mult) != 0] +
            [c.show() for c in range_constraints if c.show() != ""]
        )
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
    if ex[10] != []:
        BOUND = ex[10][0]
        for b in ex[11][BOUND]:
            i = 0
            for v in ex[11][BOUND][b]:
                c = RangeConstraint(i, b, v)
                prangeconstraints += [c]
                i+=1
            while(i < num_vars):
                prangeconstraints += [RangeConstraint(i, 'UNCONSTRAINED', 0.0)]
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

int dequal(fp32 a, fp32 b, fp32 delta) {
    if (a > b) {
        return (a - b) <= delta;
    } else {
        return (b - a) <= delta;
    }
}

int main() {
'''

primal = cfuncgen(
    pconstraints,
    pobj,
    pnum_vars,
    pmin_max,
    prangeconstraints
)

primal_check = ccheckgen(
    pconstraints,
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

dual_check = ccheckgen(
    dconstraints,
    drangeconstraints
)

dual_obj = dobj.show()

certificate = "    __GADGET_check(dequal({}, {}, 0.01));".format(
    primal_obj,
    dual_obj
);

c_foot = '''
}
'''

print("\n".join([c_header, primal, dual, primal_check, dual_check, certificate, c_foot]))

