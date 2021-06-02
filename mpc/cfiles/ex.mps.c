
  int main() {
    typedef double fp32;
    fp32 X0 = __GADGET_exist(), X1 = __GADGET_exist();

    __GADGET_maximize(
      3.000*X0 + 4.000*X1, // objective
      X0 + 2.000*X1 <= 14.000,
	3.000*X0 + -1.000*X1 >= 0.000,
	X0 + -1.000*X1 <= 2.000
    );
  }
    
