//specify problem size here
#define C 3 //constraints
#define V 2 //vars
#define CP 4 //constraints
#define VP 3 //vars

//compiler-friendly structs/types
#define LEQ 0
#define GEQ 1
#define MAX 1
#define MIN 0

typedef float fixed_point_precision_16_16;
static const fixed_point_precision_16_16 epsilon   = (fixed_point_precision_16_16)1.0e-2;

typedef struct {
  int rows, cols; // mat[m x n]
  fixed_point_precision_16_16 mat[70]; //upper bound yourself
  _Bool stars[VP]; // V+1
  int cntr;
} Tableau;

typedef struct {
  fixed_point_precision_16_16 vars[V]; // V
  int lt_gt;
  fixed_point_precision_16_16 goal;
} Constraint;

typedef struct {
  fixed_point_precision_16_16 x[VP]; //V+1
  fixed_point_precision_16_16 y[CP]; //C+1
} Solution;

typedef struct {
  fixed_point_precision_16_16 m0;
  fixed_point_precision_16_16 m1;
  fixed_point_precision_16_16 m2;
  fixed_point_precision_16_16 m3;
  fixed_point_precision_16_16 m4;
  fixed_point_precision_16_16 m5;
  fixed_point_precision_16_16 m6;
  fixed_point_precision_16_16 m7;
  fixed_point_precision_16_16 m8;
  fixed_point_precision_16_16 m9;
  fixed_point_precision_16_16 m10;
  fixed_point_precision_16_16 m11;
  fixed_point_precision_16_16 m12;
  fixed_point_precision_16_16 m13;
  fixed_point_precision_16_16 m14;
  fixed_point_precision_16_16 m15;
  fixed_point_precision_16_16 m16;
  fixed_point_precision_16_16 m17;
  fixed_point_precision_16_16 m18;
  fixed_point_precision_16_16 m19;
  fixed_point_precision_16_16 m20;
  fixed_point_precision_16_16 m21;
  fixed_point_precision_16_16 m22;
  fixed_point_precision_16_16 m23;
  fixed_point_precision_16_16 m24;
  fixed_point_precision_16_16 m25;
  fixed_point_precision_16_16 m26;
  fixed_point_precision_16_16 m27;
  fixed_point_precision_16_16 m28;
  fixed_point_precision_16_16 m29;
  fixed_point_precision_16_16 m30;
  fixed_point_precision_16_16 m31;
  fixed_point_precision_16_16 m32;
  fixed_point_precision_16_16 m33;
  fixed_point_precision_16_16 m34;
  fixed_point_precision_16_16 m35;
  fixed_point_precision_16_16 m36;
  fixed_point_precision_16_16 m37;
  fixed_point_precision_16_16 m38;
  fixed_point_precision_16_16 m39;
  fixed_point_precision_16_16 m40;
  fixed_point_precision_16_16 m41;
  fixed_point_precision_16_16 m42;
  fixed_point_precision_16_16 m43;
  fixed_point_precision_16_16 m44;
  fixed_point_precision_16_16 m45;
  fixed_point_precision_16_16 m46;
  fixed_point_precision_16_16 m47;
  fixed_point_precision_16_16 m48;
  fixed_point_precision_16_16 m49;
  fixed_point_precision_16_16 m50;
  fixed_point_precision_16_16 m51;
  fixed_point_precision_16_16 m52;
  fixed_point_precision_16_16 m53;
  fixed_point_precision_16_16 m54;
  fixed_point_precision_16_16 m55;
  fixed_point_precision_16_16 m56;
  fixed_point_precision_16_16 m57;
  fixed_point_precision_16_16 m58;
  fixed_point_precision_16_16 m59;
  fixed_point_precision_16_16 m60;
  fixed_point_precision_16_16 m61;
  fixed_point_precision_16_16 m62;
  fixed_point_precision_16_16 m63;
  fixed_point_precision_16_16 m64;
  fixed_point_precision_16_16 m65;
  fixed_point_precision_16_16 m66;
  fixed_point_precision_16_16 m67;
  fixed_point_precision_16_16 m68;
  fixed_point_precision_16_16 m69;
  fixed_point_precision_16_16 m70;

  fixed_point_precision_16_16 x0;
  fixed_point_precision_16_16 x1;
  fixed_point_precision_16_16 x2;
  fixed_point_precision_16_16 x3;
  fixed_point_precision_16_16 x4;
  fixed_point_precision_16_16 x5;

  fixed_point_precision_16_16 y0;
  fixed_point_precision_16_16 y1;
  fixed_point_precision_16_16 y2;
  fixed_point_precision_16_16 y3;
  fixed_point_precision_16_16 y4;
  fixed_point_precision_16_16 y5;



} Solution_Box;



//API
Tableau make_problem(){
  Tableau tab = { CP, V+C+1, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0}, 1 };

  return tab;
}

Tableau maximize(Constraint c){
  Tableau t = make_problem();

  t.mat[0] = c.goal;
  for (int i = 1; i < VP; i++){
    t.mat[i] = c.vars[i-1] * -1;
  }

  return t;
}

Tableau minimize(Constraint c){
  Tableau t = make_problem();

  t.mat[0] = c.goal;
  for (int i = 1; i < VP; i++){
    t.mat[i] = c.vars[i-1];
  }

  return t;
}

Tableau add(Tableau t, Constraint c){

  t.mat[t.cntr*t.cols] = c.goal;
  for (int j = 1; j < VP; j++){
    t.mat[(t.cntr*t.cols)+j] = c.vars[j-1];
  }

  t.cntr = t.cntr + 1;
  return t;
}


//simplex
int d_equal(fixed_point_precision_16_16 a, fixed_point_precision_16_16 b) {
  if ((a-b) > 0) {
    return (a-b) < epsilon;
  } else {
    return -1*(a-b) < epsilon;
  }
}

fixed_point_precision_16_16 get(int i, int j, Tableau tab){
  return tab.mat[i*(tab.cols)+j];
}

Tableau add_slack(Tableau tab, int max_min, int vars){

  int slack = tab.rows - 1;

  int index = 0;
  for (int i=1; i<tab.rows; i++) {

      for (int j=vars+1; j<vars+1+slack; j++) {
        if (j - vars == i) {
          if (max_min) {
            tab.mat[i*(tab.cols)+j] = 1.0;
          } else {
            tab.mat[i*(tab.cols)+j] = -1.0;
            tab.stars[i] = 1;
          }
        }

      }

  }

  return tab;
}


Tableau calculate_dual(Tableau p, int max_min){
  //transpose
  Tableau transpose = {VP, C+V+1, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0}, 1 };

  if (max_min){ //make dual max
    for (int i = 0; i < VP; i++) {
        for (int j = 0; j < CP; j++) {
          if (i == 0) {
            transpose.mat[i*(transpose.cols)+j] = get(j,i,p) * -1;
          } else {
            transpose.mat[i*(transpose.cols)+j] = get(j,i,p);
          }

        }
    }


  } else { //make dual min
    for (int i = 0; i < VP; i++) {
        for (int j = 0; j < CP; j++) {
          if (j == 0) {
            transpose.mat[i*(transpose.cols)+j] = get(j,i,p) * -1;
          } else {
            transpose.mat[i*(transpose.cols)+j] = get(j,i,p);
          }

        }
    }

  }

  return transpose;
}


Tableau simplex_max(Tableau tableau) {

  fixed_point_precision_16_16 lowest = -1;
  int no_ratio = 0;

  while( !(lowest >= 0) && no_ratio == 0) {
    int pivot_col, pivot_row;

    pivot_col = 1;
    lowest = get(0, pivot_col, tableau);

    int slack = tableau.rows - 1;
    for(int j=1; j<tableau.cols-slack; j++) {

      fixed_point_precision_16_16 tab0j = get(0,j,tableau);
      if (tab0j < lowest) {
        lowest = tab0j;
        pivot_col = j;
      }

    }

    if( lowest < 0 ) {
      pivot_row = 0;
      no_ratio = 1;
      fixed_point_precision_16_16 min_ratio = 0;

      for(int i=1;i<tableau.rows;i++){
        fixed_point_precision_16_16 entry = get(i,0,tableau);
        fixed_point_precision_16_16 pot_pivot = get(i,pivot_col,tableau);
        if (pot_pivot > 0 && !(d_equal(pot_pivot,0))) {
          fixed_point_precision_16_16 ratio = entry / pot_pivot; //Issue with prover here

          if (ratio < min_ratio || no_ratio == 1) {
            min_ratio = ratio;
            pivot_row = i;
            no_ratio = 0;
          }

        }

      }

      if (no_ratio == 0) {
        fixed_point_precision_16_16 pivot = get(pivot_row, pivot_col, tableau);


        for(int k=0;k<tableau.cols;k++) {
          if (!(d_equal(pivot,0.0))){ //ADDED FOR PROVER
            tableau.mat[pivot_row*(tableau.cols)+k] = get(pivot_row,k,tableau) / pivot;
          }
        }

        for(int l=0; l<tableau.rows; l++) {

          fixed_point_precision_16_16 multiplier = get(l, pivot_col, tableau);
          if(l!=pivot_row) {
            for(int m=0; m<tableau.cols; m++) {
              fixed_point_precision_16_16 set_val = get(l,m,tableau) - (multiplier * get(pivot_row,m,tableau));
              tableau.mat[l*(tableau.cols)+m] = set_val;
            }
          }
        }
      }


    }

  }

  return tableau;
}

Tableau simplex_stars(Tableau tableau) {

  fixed_point_precision_16_16 highest = 0;
  int no_ratio = 0;
  _Bool stars = 1;

  int loop = 0;
  while(stars) {
  loop = loop + 1;

    int pivot_col, pivot_row;
    int b = 1;

    for (int r=1; r < tableau.rows; r++){
      if (tableau.stars[r] && b) {

        pivot_col = 1;
        highest = get(r,pivot_col,tableau); // what if no positives?

        int slack = tableau.rows - 1;
        for(int j=1; j<tableau.cols; j++) {

          fixed_point_precision_16_16 tabrj = get(r,j,tableau);

          if (tabrj > highest) {
            highest = tabrj;
            pivot_col = j;

          }
        }

        b = 0;
      }
    }


    if( highest > 0 ) { //?

      pivot_row = 0;

      int no_ratio = 1;
      fixed_point_precision_16_16 min_ratio = 0;

      for(int i=1;i<tableau.rows;i++){
        fixed_point_precision_16_16 entry = get(i,0,tableau);
        fixed_point_precision_16_16 pot_pivot = get(i,pivot_col,tableau);
        if (pot_pivot > 0 && !(d_equal(pot_pivot,0))) {
          fixed_point_precision_16_16 ratio = entry / pot_pivot; //issue here

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

        for(int k=0;k<tableau.cols;k++) {
          if (!(d_equal(pivot,0))){ //ADDED FOR PROVER
            tableau.mat[pivot_row*(tableau.cols)+k] = get(pivot_row,k,tableau) / pivot;
          }
        }

        for(int l=0; l<tableau.rows; l++) {

          fixed_point_precision_16_16 multiplier = get(l, pivot_col, tableau);
          if(l!=pivot_row) {
            for(int m=0; m<tableau.cols; m++) {
              fixed_point_precision_16_16 set_val = get(l,m,tableau) - (multiplier * get(pivot_row,m,tableau));
              tableau.mat[l*(tableau.cols)+m] = set_val;
            }
          }
        }
      } // end no ratio

    } // end highest

  stars = 0;
  for (int s=0; s<tableau.rows; s++) {
    stars = stars || tableau.stars[s];

  }

  }

  return tableau;
}


fixed_point_precision_16_16 find_opt_var(Tableau t, int j){
  fixed_point_precision_16_16 zero = 0;
  int x=-1;
  for(int i=1; i < t.rows; i++) {
    if (d_equal(get(i, j, t), 1)) {
      if (x == -1) {
        x=i;
      } else {
        return zero;
      }
    } else if (!d_equal(get(i,j,t),0)) {
      return zero;
    }
  }
  return get(x,0,t);
}

Solution_Box simplex_prover(Tableau p_tableau, int p_max_min) {
  //PROVER CODE
  // calculate primal solution
  Tableau p_sol_tab_a = add_slack(p_tableau, p_max_min, V);

  Tableau p_sol_tab_b = simplex_stars(p_sol_tab_a);
  Tableau p_sol_tab = simplex_max(p_sol_tab_b);


  int d_max_min = !p_max_min;
  Tableau d_tableau = calculate_dual(p_tableau, d_max_min);

  Tableau d_sol_tab_a = add_slack(d_tableau, d_max_min, C);
  Tableau d_sol_tab_b = simplex_stars(d_sol_tab_a);
  Tableau d_sol_tab = simplex_max(d_sol_tab_b);


  Solution sol = {{0,0,0}, {0,0,0,0}};

  for(int i=0; i<V; i++) {
    sol.x[i] = find_opt_var(p_sol_tab, (i+1));
  }
  sol.x[V] = p_sol_tab.mat[0];
  fixed_point_precision_16_16 y[CP];
  for(int j=0; j<C; j++) {
    sol.y[j] = find_opt_var(d_sol_tab, (j+1));
  }
  sol.y[C] = d_sol_tab.mat[0];

  Solution_Box solf = { p_tableau.mat[0],
                      p_tableau.mat[1],
                      p_tableau.mat[2],
                      p_tableau.mat[3],
                      p_tableau.mat[4],
                      p_tableau.mat[5],
                      p_tableau.mat[6],
                      p_tableau.mat[7],
                      p_tableau.mat[8],
                      p_tableau.mat[9],
                      p_tableau.mat[10],
                      p_tableau.mat[11],
                      p_tableau.mat[12],
                      p_tableau.mat[13],
                      p_tableau.mat[14],
                      p_tableau.mat[15],
                      p_tableau.mat[16],
                      p_tableau.mat[17],
                      p_tableau.mat[18],
                      p_tableau.mat[19],
                      p_tableau.mat[20],
                      p_tableau.mat[21],
                      p_tableau.mat[22],
                      p_tableau.mat[23],
                      p_tableau.mat[24],
                      p_tableau.mat[25],
                      p_tableau.mat[26],
                      p_tableau.mat[27],
                      p_tableau.mat[28],
                      p_tableau.mat[29],
                      p_tableau.mat[30],
                      p_tableau.mat[31],
                      p_tableau.mat[32],
                      p_tableau.mat[33],
                      p_tableau.mat[34],
                      p_tableau.mat[35],
                      p_tableau.mat[36],
                      p_tableau.mat[37],
                      p_tableau.mat[38],
                      p_tableau.mat[39],
                      p_tableau.mat[40],
                      p_tableau.mat[41],
                      p_tableau.mat[42],
                      p_tableau.mat[43],
                      p_tableau.mat[44],
                      p_tableau.mat[45],
                      p_tableau.mat[46],
                      p_tableau.mat[47],
                      p_tableau.mat[48],
                      p_tableau.mat[49],
                      p_tableau.mat[50],
                      p_tableau.mat[51],
                      p_tableau.mat[52],
                      p_tableau.mat[53],
                      p_tableau.mat[54],
                      p_tableau.mat[55],
                      p_tableau.mat[56],
                      p_tableau.mat[57],
                      p_tableau.mat[58],
                      p_tableau.mat[59],
                      p_tableau.mat[60],
                      p_tableau.mat[61],
                      p_tableau.mat[62],
                      p_tableau.mat[63],
                      p_tableau.mat[64],
                      p_tableau.mat[65],
                      p_tableau.mat[66],
                      p_tableau.mat[67],
                      p_tableau.mat[68],
                      p_tableau.mat[69],
                      p_tableau.mat[70],

                      sol.x[0],
                      sol.x[1],
                      sol.x[2],
                      0.0,//sol.x[3],
                      0.0,//sol.x[4],
                      0.0,//sol.x[5],

                      sol.y[0],
                      sol.y[1],
                      sol.y[2],
                      sol.y[3],
                      0.0,//sol.y[4],
                      0.0,//sol.y[5]

                    };


  return solf;

}

int simplex_check(int max_min, Solution_Box sf) {

    //SOLUTION EQUALITY
    int sol_eq = 0;

    //cT * x = yT * b
    //fixed_point_precision_16_16 xc = 0;
    fixed_point_precision_16_16 xc = (sf.m1 * sf.x0) +
                                     (sf.m2 * sf.x1);

    fixed_point_precision_16_16 yb = (sf.m6 * sf.y0) +
                                     (sf.m12 * sf.y1) +
                                     (sf.m18 * sf.y2);

    if (max_min){
      sol_eq = d_equal(xc+yb,0);
    } else {
      sol_eq = d_equal(xc,yb);
    }

    //SAT
    int sat = 1;

    fixed_point_precision_16_16 prod0 = (sf.m1 * sf.x0) +
                                        (sf.m2 * sf.x1);

    fixed_point_precision_16_16 prod1 = (sf.m7 * sf.x0) +
                                        (sf.m8 * sf.x1);
    fixed_point_precision_16_16 prod2 = (sf.m13 * sf.x0) +
                                        (sf.m14 * sf.x1);
    fixed_point_precision_16_16 prod3 = (sf.m19 * sf.x0) +
                                        (sf.m20 * sf.x1);

    //eq?
    if (max_min){ // max prob
      sat = (d_equal(sf.m0,prod0) || sf.m0 > prod0) &&
            (d_equal(sf.m11,prod1) || sf.m11 > prod1) &&
            (d_equal(sf.m22,prod2) || sf.m22 > prod2) &&
            (d_equal(sf.m33,prod3) || sf.m33 > prod3);
    } else { // min prob
      sat = (d_equal(sf.m0,prod0) || sf.m0 < prod0) &&
            (d_equal(sf.m6,prod1) || sf.m6 < prod1) &&
            (d_equal(sf.m12,prod2) || sf.m12 < prod2) &&
            (d_equal(sf.m18,prod3) || sf.m18 < prod3);
    }

    return sol_eq && sat;
}


Solution_Box simplex_gadget(Tableau p_tableau, int p_max_min) {

  Solution_Box sol = __GADGET_compute(simplex_prover(p_tableau, p_max_min));
  __GADGET_check(simplex_check(p_max_min, sol));

  return sol;
}

int main(void) {

  Constraint obj = {{24.0, 60.0}, LEQ, 0};
  Tableau tab = minimize(obj);

  Constraint c1 = {{0.5, 1.0}, GEQ, 6.0};
  Constraint c2 = {{2.0, 2.0}, GEQ, 14.0};
  Constraint c3 = {{1.0, 4.0}, GEQ, 13.0};
  tab = add(tab, c1);
  tab = add(tab, c2);
  tab = add(tab, c3);

  //Solution_Box sol = simplex_prover(tab, MIN);
  //printf("CHECK %d", simplex_check(MIN, sol));

  Solution_Box sol = simplex_gadget(tab, MIN);

  return 0;

}


