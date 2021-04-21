from pysmps import smps_loader as smps

def strs_fl(l):
    ss=[]
    for a in l:
        ss += ["{:.2f}".format(float(a))]
    return ss

def strs_int(l):
    ss=[]
    for a in l:
        ss += ["%d" % a]
    return ss

class constraint:
    def __init__(self, mult, sign, const):
        self.mult = mult
        self.sign = sign
        self.const = const


    def show(self):
        return "{{ %s }, %s, %s }" % (", ".join(strs_fl(self.mult)), self.sign, self.const)

# substitution dict
def substitutions(constraints, objective, num_vars, num_constr, max_min):
    # derived
    num_vars_plus = num_vars + 1
    num_constr_plus = num_constr + 1
    mat_dim = num_vars_plus * num_constr_plus

    def dot(vars_a, vars_b):
        l = []
        if(len(vars_a) != len(vars_b)):
            print(vars_a)
            print(vars_b)
            raise ValueError

        for (a,b) in zip(vars_a, vars_b):
            l += ["{} * {}".format(a,b)]

        return ' + '.join(l)

    return {
        'max_min' : "%d" % max_min,
        'num_vars' : "%d" % num_vars,
        'num_vars_plus' : "%d" % num_vars_plus,
        'num_constr' : "%d" % num_constr,
        'num_constr_plus' : "%d" % num_constr_plus,
        'zero_matrix' : "{ %s }" % ", ".join(strs_int([0]*mat_dim)),
        'zero_constr' : "{ %s }" % ", ".join(strs_int([0]*num_constr_plus)),
        'zero_vars' : "{ %s }" % ", ".join(strs_int([0]*num_vars_plus)),
        'mat_dim' : "%d" % mat_dim,
        'mat_fp_exploded' :
            "\n  ".join(["\tfixed_point_precision_16_16 m{};".format(i) for i in range(mat_dim)]),
        'x_fp_exploded' :
            "\n  ".join(["\tfixed_point_precision_16_16 x{};".format(i) for i in range(num_vars_plus)]),
        'y_fp_exploded' :
            "\n  ".join(["\tfixed_point_precision_16_16 y{};".format(i) for i in range(num_vars_plus)]),
        'p_tableau_mat' :
            "\n  ".join(["p_tableau.mat[{}],".format(i) for i in range(mat_dim)]),
        'sol_x' :
            "\n  ".join(["sol.x[{}],".format(i) for i in range(num_vars_plus)]),
        'sol_y' :
            "\n  ".join(["sol.y[{}],".format(i) for i in range(num_constr_plus)]),
        'x_dot_m':
            dot(
                ["sf.m{}".format(i) for i in range(1, num_vars_plus)],
                ["sf.x{}".format(i) for i in range(num_vars)]),
        'y_dot_m':
            dot(
                ["sf.m{}".format(i) for i in range(num_vars_plus, num_constr_plus * num_vars_plus, num_vars_plus)],
                ["sf.y{}".format(i) for i in range(num_constr)]),
        'x_prods':
            "\n  ".join(
                ["fixed_point_precision_16_16 prod{} = {};".format(
                    k,
                    dot(
                        ["sf.m{}".format(i+1) for i in range(k*num_vars, num_vars+k*num_vars)],
                        ["sf.x{}".format(i) for i in range(num_vars)])
                    ) for k in range(num_constr_plus)]),
        'constraints':
            "\n  ".join(
                ["Constraint c{} = {};".format(k, constraints[k].show()) for k in range(len(constraints))]),
        'constraints_add':
            "\n  ".join(
                ["tab = add(tab, c{});".format(k) for k in range(len(constraints))]),
        'objective': "{ %s }" % objective.show()
    }

def parse(filename):
    ex = smps.load_mps(filename)

    # Is it max or min objective?
    min_max = 0
    if("MAX" in ex[0]):
        min_max = 1

    equaz = ex[2]
    varz = ex[3]
    num_vars = len(varz)
    num_constr = len(equaz)
    obj = constraint(ex[6], min_max, 0)
    constraints = []
    for (mult, s, rhs) in zip(ex[7], ex[5], ex[9]['RHS']):
        sign = 0
        if(s == 'G'):
            sign = 1
        constraints += [constraint(mult, sign, rhs)]

    return substitutions(constraints, obj, num_vars, num_constr, min_max)

def prettyprint(substitutions):
    with open("simplex_template.c") as templf:
        templ = templf.read()
        for (term, subst) in sorted(substitutions.items(), key=lambda k: -len(k[0])):
            templ = templ.replace("${}".format(term), subst)
        return templ

print(prettyprint(parse("ex.mps")))
