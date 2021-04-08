num_vars = 2
num_constr = 3

# derived
num_vars_plus = num_vars + 1
num_constr_plus = num_constr + 1
mat_dim = num_vars_plus * num_constr_plus

# substitution dict
substitutions = {
    'num_vars' = num_vars,
    'num_vars_plus' = num_vars_plus,
    'num_constr' = num_constr,
    'num_constr_plus' = num_constr_plus,
    'zero_matrix' = "{ {} }".format(", ".join([0]*mat_dim),
    'zero_constr' = "{ {} }".format(", ".join([0]*num_constr_plus),
    'zero_vars' = "{ {} }".format(", ".join([0]*num_vars_plus),
    'mat_dim' = mat_dim,
    'mat_fp_exploded' =
        "\n".join(["\tfixed_point_precision_16_16 m{};".format(i) for i in range(mat_dim)]),
    'x_fp_exploded' =
        "\n".join(["\tfixed_point_precision_16_16 x{};".format(i) for i in range(num_vars_plus)]),
    'y_fp_exploded' =
        "\n".join(["\tfixed_point_precision_16_16 y{};".format(i) for i in range(num_vars_plus)]),
    'p_tableau_mat' =
        "\n".join(["\p_tableau.mat[{}],".format(i) for i in range(mat_dim)]),
    'sol_x' =
        "\n".join(["\sol.x[{}],".format(i) for i in range(num_vars_plus)]),
    'sol_y' =
        "\n".join(["\sol.y[{}],".format(i) for i in range(num_constr_plus)]),






