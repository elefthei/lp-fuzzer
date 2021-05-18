from pysmps import smps_loader as smps
import sys
import pprint

class constraint:
    def __init__(self, mult, sign, const):
        self.mult = mult
        self.sign = sign
        self.const = const

    def show(self):
        num_vars = len(self.mult)
        terms = [ None  if m == 0.0 else "X{}".format(v) if m == 1.0 else "{:.3f}*X{}".format(m, v) for (m,v) in zip(self.mult, range(num_vars)) ]
        terms = filter(lambda x: x != None, terms)
        if self.sign == 0:
            return " + ".join(terms) + " == " + "{:.3f}".format(self.const)
        elif self.sign == 1:
            return " + ".join(terms) + " >= " + "{:.3f}".format(self.const)
        else:
            return " + ".join(terms) + " <= " + "{:.3f}".format(self.const)

# substitution dict
def substitutions(constraints, objective, num_vars, num_constr, max_min):
    C = """
  int main() {
    typedef double fp32;
    fp32 $vars;

    __GADGET_$maxmin(
      $objective, // objective
      $constraints
    );
  }
    """

    substitutions = {
        'vars' : ", ".join(["X{}".format(i) for i in range(num_vars)]),
        'maxmin' : "minimize" if max_min == 0 else "maximize",
        'objective': objective.show().split(">=")[0],
        'constraints': ",\n\t".join([c.show() for c in constraints])
    }

    for (term, subst) in sorted(substitutions.items(), key=lambda k: -len(k[0])):
        C = C.replace("${}".format(term), subst)
    return C

def parse(filename):
    ex = smps.load_mps(filename)

    # Is it max or min objective?
    min_max = 1
    if("MIN" in ex[0]):
        min_max = 0

    equaz = ex[2]
    varz = ex[3]
    num_vars = len(varz)
    num_constr = len(equaz)
    obj = constraint(ex[6], min_max, 0)
    constraints = []

    if ex[8] == []:
        for (mult, s) in zip(ex[7], ex[5]):
            sign = 0
            if(s == 'G'):
                sign = 1
            elif(s == 'E'):
                sign = 0
            elif(s == 'L'):
                sign = -1
            else:
                raise ValueError("Interpret sign")

            constraints += [constraint(mult, sign, 0.0)]
    else:
        RHS = ex[8][0]
        if len(ex[7]) != len(ex[5]) or len(ex[5]) != len(ex[9][RHS]):
            raise ValueError("Unaligned equations {}, {}, {}".format(len(ex[5]), len(ex[7]), len(ex[9][RHS])))

        for (mult, s, rhs) in zip(ex[7], ex[5], ex[9][RHS]):
            sign = 0
            if(s == 'G'):
                sign = 1
            elif(s == 'E'):
                sign = 0
            elif(s == 'L'):
                sign = -1
            else:
                raise ValueError("Interpret sign")

            constraints += [constraint(mult, sign, rhs)]

    if ex[10] != []:
        BOUND = ex[10][0]
        for b in ex[11][BOUND]:
            i = 0
            for v in ex[11][BOUND][b]:
                mult = [0.0]*num_vars
                mult[i] = 1.0
                if b == 'UP' and v != float('inf') and v!= float('-inf'):
                    constraints += [constraint(mult, -1, v)]
                elif b == 'LO' and v != float('inf') and v != float('-inf'):
                    constraints += [constraint(mult, 1, v)]
                elif b == 'FX' and v != float('inf') and v != float('-inf'):
                    constraints += [constraint(mult, 0, v)]
                elif b == 'MI':
                    constraints += [constraint(mult, -1, 0.0)]
                elif b == 'PL':
                    constraints += [constraint(mult, 1, 0.0)]
                i+=1

    return (constraints, obj, num_vars, num_constr, min_max)

def prettyprint(substitutions):
    with open("simplex_template.c") as templf:
        templ = templf.read()

# MAIN
(constraints, objective, num_vars, num_constr, min_max) = parse(sys.argv[1])
print(substitutions(constraints, objective, num_vars, num_constr, min_max))
