typedef float fixed_point_precision_16_16;
static const fixed_point_precision_16_16 epsilon =
    (fixed_point_precision_16_16)1.0e-2;

typedef struct {
  int rows, cols;                            // mat[m x n]
  fixed_point_precision_16_16 mat[$mat_dim]; // upper bound yourself
  _Bool stars[$num_vars_plus];               // V+1
  int cntr;
} Tableau;

typedef struct {
  fixed_point_precision_16_16 vars[$num_vars]; // V
  int lt_gt;
  fixed_point_precision_16_16 goal;
} Constraint;

typedef struct {
  fixed_point_precision_16_16 x[$num_vars_plus]; // V+1
  fixed_point_precision_16_16 y[$num_vars_plus]; // C+1
} Solution;

typedef struct {
  $mat_fp_exploded

  $x_fp_exploded

  $y_fp_exploded

} Solution_Box;

// MACROS
Tableau make_problem() {
  Tableau tab = {
    $num_constr + 1,
    $num_vars + $num_constr + 1,
    $zero_matrix,
    $zero_vars,
    1
  };

  return tab;
}

/** No macros needed */
Tableau maximize(Constraint c) {
  Tableau t = make_problem();

  t.mat[0] = c.goal;
  for (int i = 1; i < $num_vars_plus; i++) {
    t.mat[i] = c.vars[i - 1] * -1;
  }

  return t;
}

/** No macros needed */
Tableau minimize(Constraint c) {
  Tableau t = make_problem();

  t.mat[0] = c.goal;
  for (int i = 1; i < $num_vars_plus; i++) {
    t.mat[i] = c.vars[i - 1];
  }

  return t;
}

/** No macros needed */
Tableau add(Tableau t, Constraint c) {

  t.mat[t.cntr * t.cols] = c.goal;
  for (int j = 1; j < $num_vars_plus; j++) {
    t.mat[(t.cntr * t.cols) + j] = c.vars[j - 1];
  }

  t.cntr = t.cntr + 1;
  return t;
}

/** No macros needed */
int d_equal(fixed_point_precision_16_16 a, fixed_point_precision_16_16 b) {
  if ((a - b) > 0) {
    return (a - b) < epsilon;
  } else {
    return -1 * (a - b) < epsilon;
  }
}

/** No macros needed */
fixed_point_precision_16_16 get(int i, int j, Tableau tab) {
  return tab.mat[i * (tab.cols) + j];
}

/** No macros needed */
Tableau add_slack(Tableau tab, int max_min, int vars) {

  int slack = tab.rows - 1;

  int index = 0;
  for (int i = 1; i < tab.rows; i++) {

    for (int j = vars + 1; j < vars + 1 + slack; j++) {
      if (j - vars == i) {
        if (max_min) {
          tab.mat[i * (tab.cols) + j] = 1.0;
        } else {
          tab.mat[i * (tab.cols) + j] = -1.0;
          tab.stars[i] = 1;
        }
      }
    }
  }

  return tab;
}

/** No macros needed */
Tableau calculate_dual(Tableau p, int max_min) {
  // transpose
  Tableau transpose = {$num_constr + 1, $num_vars + $num_constr + 1,
                       $zero_matrix, $zero_vars, 1};

  if (max_min) { // make dual max
    for (int i = 0; i < $num_vars_plus; i++) {
      for (int j = 0; j < $num_constr_plus; j++) {
        if (i == 0) {
          transpose.mat[i * (transpose.cols) + j] = get(j, i, p) * -1;
        } else {
          transpose.mat[i * (transpose.cols) + j] = get(j, i, p);
        }
      }
    }

  } else { // make dual min
    for (int i = 0; i < $num_vars_plus; i++) {
      for (int j = 0; j < $num_constr_plus; j++) {
        if (j == 0) {
          transpose.mat[i * (transpose.cols) + j] = get(j, i, p) * -1;
        } else {
          transpose.mat[i * (transpose.cols) + j] = get(j, i, p);
        }
      }
    }
  }

  return transpose;
}

/* No macros needed here */
Tableau simplex_max(Tableau tableau) {

  fixed_point_precision_16_16 lowest = -1;
  int no_ratio = 0;

  while (!(lowest >= 0) && no_ratio == 0) {
    int pivot_col, pivot_row;

    pivot_col = 1;
    lowest = get(0, pivot_col, tableau);

    int slack = tableau.rows - 1;
    for (int j = 1; j < tableau.cols - slack; j++) {

      fixed_point_precision_16_16 tab0j = get(0, j, tableau);
      if (tab0j < lowest) {
        lowest = tab0j;
        pivot_col = j;
      }
    }

    if (lowest < 0) {
      pivot_row = 0;
      no_ratio = 1;
      fixed_point_precision_16_16 min_ratio = 0;

      for (int i = 1; i < tableau.rows; i++) {
        fixed_point_precision_16_16 entry = get(i, 0, tableau);
        fixed_point_precision_16_16 pot_pivot = get(i, pivot_col, tableau);
        if (pot_pivot > 0 && !(d_equal(pot_pivot, 0))) {
          fixed_point_precision_16_16 ratio =
              entry / pot_pivot; // Issue with prover here

          if (ratio < min_ratio || no_ratio == 1) {
            min_ratio = ratio;
            pivot_row = i;
            no_ratio = 0;
          }
        }
      }

      if (no_ratio == 0) {
        fixed_point_precision_16_16 pivot = get(pivot_row, pivot_col, tableau);

        for (int k = 0; k < tableau.cols; k++) {
          if (!(d_equal(pivot, 0.0))) { // ADDED FOR PROVER
            tableau.mat[pivot_row * (tableau.cols) + k] =
                get(pivot_row, k, tableau) / pivot;
          }
        }

        for (int l = 0; l < tableau.rows; l++) {

          fixed_point_precision_16_16 multiplier = get(l, pivot_col, tableau);
          if (l != pivot_row) {
            for (int m = 0; m < tableau.cols; m++) {
              fixed_point_precision_16_16 set_val =
                  get(l, m, tableau) -
                  (multiplier * get(pivot_row, m, tableau));
              tableau.mat[l * (tableau.cols) + m] = set_val;
            }
          }
        }
      }
    }
  }

  return tableau;
}

/** No macros */
Tableau simplex_stars(Tableau tableau) {

  fixed_point_precision_16_16 highest = 0;
  int no_ratio = 0;
  _Bool stars = 1;

  int loop = 0;
  while (stars) {
    loop = loop + 1;

    int pivot_col, pivot_row;
    int b = 1;

    for (int r = 1; r < tableau.rows; r++) {
      if (tableau.stars[r] && b) {

        pivot_col = 1;
        highest = get(r, pivot_col, tableau); // what if no positives?

        int slack = tableau.rows - 1;
        for (int j = 1; j < tableau.cols; j++) {

          fixed_point_precision_16_16 tabrj = get(r, j, tableau);

          if (tabrj > highest) {
            highest = tabrj;
            pivot_col = j;
          }
        }

        b = 0;
      }
    }

    if (highest > 0) { //?

      pivot_row = 0;

      int no_ratio = 1;
      fixed_point_precision_16_16 min_ratio = 0;

      for (int i = 1; i < tableau.rows; i++) {
        fixed_point_precision_16_16 entry = get(i, 0, tableau);
        fixed_point_precision_16_16 pot_pivot = get(i, pivot_col, tableau);
        if (pot_pivot > 0 && !(d_equal(pot_pivot, 0))) {
          fixed_point_precision_16_16 ratio = entry / pot_pivot; // issue here

          if (ratio == min_ratio && tableau.stars[i]) { // test for no ratio?
            min_ratio = ratio;
            pivot_row = i;
            no_ratio = 0;
          }

          if (ratio < min_ratio || no_ratio == 1) {
            min_ratio = ratio;
            pivot_row = i;
            no_ratio = 0;
          }
        }
      }

      if (!no_ratio) {
        fixed_point_precision_16_16 pivot = get(pivot_row, pivot_col, tableau);

        tableau.stars[pivot_row] = 0;

        for (int k = 0; k < tableau.cols; k++) {
          if (!(d_equal(pivot, 0))) { // ADDED FOR PROVER
            tableau.mat[pivot_row * (tableau.cols) + k] =
                get(pivot_row, k, tableau) / pivot;
          }
        }

        for (int l = 0; l < tableau.rows; l++) {

          fixed_point_precision_16_16 multiplier = get(l, pivot_col, tableau);
          if (l != pivot_row) {
            for (int m = 0; m < tableau.cols; m++) {
              fixed_point_precision_16_16 set_val =
                  get(l, m, tableau) -
                  (multiplier * get(pivot_row, m, tableau));
              tableau.mat[l * (tableau.cols) + m] = set_val;
            }
          }
        }
      } // end no ratio

    } // end highest

    stars = 0;
    for (int s = 0; s < tableau.rows; s++) {
      stars = stars || tableau.stars[s];
    }
  }

  return tableau;
}

/** No macros needed */
fixed_point_precision_16_16 find_opt_var(Tableau t, int j) {
  fixed_point_precision_16_16 zero = 0;
  int x = -1;
  for (int i = 1; i < t.rows; i++) {
    if (d_equal(get(i, j, t), 1)) {
      if (x == -1) {
        x = i;
      } else {
        return zero;
      }
    } else if (!d_equal(get(i, j, t), 0)) {
      return zero;
    }
  }
  return get(x, 0, t);
}

/** MACROS */
Solution_Box simplex_prover(Tableau p_tableau, int p_max_min) {
  // PROVER CODE
  // calculate primal solution
  Tableau p_sol_tab_a = add_slack(p_tableau, p_max_min, $num_vars);

  Tableau p_sol_tab_b = simplex_stars(p_sol_tab_a);
  Tableau p_sol_tab = simplex_max(p_sol_tab_b);

  int d_max_min = !p_max_min;
  Tableau d_tableau = calculate_dual(p_tableau, d_max_min);

  Tableau d_sol_tab_a = add_slack(d_tableau, d_max_min, C);
  Tableau d_sol_tab_b = simplex_stars(d_sol_tab_a);
  Tableau d_sol_tab = simplex_max(d_sol_tab_b);

  Solution sol = {$zero_vars, $zero_constr};

  for (int i = 0; i < $num_vars; i++) {
    sol.x[i] = find_opt_var(p_sol_tab, (i + 1));
  }

  sol.x[$num_vars] = p_sol_tab.mat[0];
  fixed_point_precision_16_16 y[$num_constr_plus];

  for (int j = 0; j < $num_constr; j++) {
    sol.y[j] = find_opt_var(d_sol_tab, (j + 1));
  }
  sol.y[$num_constr] = d_sol_tab.mat[0];

  Solution_Box solf = {
	$p_tableau_mat

        $sol_x

        $sol_y
  };

  return solf;
}

/** MACROS */
int simplex_check(int max_min, Solution_Box sf) {

  // SOLUTION EQUALITY
  int sol_eq = 0;

  // cT * x = yT * b
  fixed_point_precision_16_16 xc = $x_dot_m;

  fixed_point_precision_16_16 yb = $y_dot_m;

  if (max_min) {
    sol_eq = d_equal(xc + yb, 0);
  } else {
    sol_eq = d_equal(xc, yb);
  }

  // SAT
  int sat = 1;

  //fixed_point_precision_16_16 prod0 = (sf.m1 * sf.x0) + (sf.m2 * sf.x1);
  //fixed_point_precision_16_16 prod1 = (sf.m7 * sf.x0) + (sf.m8 * sf.x1);
  //fixed_point_precision_16_16 prod2 = (sf.m13 * sf.x0) + (sf.m14 * sf.x1);
  //fixed_point_precision_16_16 prod3 = (sf.m19 * sf.x0) + (sf.m20 * sf.x1);
  $x_prods

  // eq?
  if (max_min) { // max prob
    sat = (d_equal(sf.m0, prod0) || sf.m0 > prod0) &&
          (d_equal(sf.m11, prod1) || sf.m11 > prod1) &&
          (d_equal(sf.m22, prod2) || sf.m22 > prod2) &&
          (d_equal(sf.m33, prod3) || sf.m33 > prod3);
  } else { // min prob
    sat = (d_equal(sf.m0, prod0) || sf.m0 < prod0) &&
          (d_equal(sf.m6, prod1) || sf.m6 < prod1) &&
          (d_equal(sf.m12, prod2) || sf.m12 < prod2) &&
          (d_equal(sf.m18, prod3) || sf.m18 < prod3);
  }

  return sol_eq && sat;
}

Solution_Box simplex_gadget(Tableau p_tableau, int p_max_min) {

  Solution_Box sol = __GADGET_compute(simplex_prover(p_tableau, p_max_min));
  __GADGET_check(simplex_check(p_max_min, sol));

  return sol;
}

int main(void) {

  Constraint obj = $objective;

  Tableau tab = minimize(obj);

  $constraints

  $constraints_add

  Solution_Box sol = simplex_gadget(tab, $min_max);

  return 0;
}
