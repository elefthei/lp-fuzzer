
  int main() {
    typedef double fp32;
    fp32 X0 = __GADGET_exist(), X1 = __GADGET_exist(), X2 = __GADGET_exist(), X3 = __GADGET_exist(), X4 = __GADGET_exist(), X5 = __GADGET_exist(), X6 = __GADGET_exist(), X7 = __GADGET_exist(), X8 = __GADGET_exist(), X9 = __GADGET_exist(), X10 = __GADGET_exist(), X11 = __GADGET_exist(), X12 = __GADGET_exist(), X13 = __GADGET_exist(), X14 = __GADGET_exist(), X15 = __GADGET_exist(), X16 = __GADGET_exist(), X17 = __GADGET_exist(), X18 = __GADGET_exist(), X19 = __GADGET_exist(), X20 = __GADGET_exist(), X21 = __GADGET_exist(), X22 = __GADGET_exist(), X23 = __GADGET_exist(), X24 = __GADGET_exist(), X25 = __GADGET_exist(), X26 = __GADGET_exist(), X27 = __GADGET_exist(), X28 = __GADGET_exist(), X29 = __GADGET_exist(), X30 = __GADGET_exist(), X31 = __GADGET_exist();

    __GADGET_maximize(
      -0.400*X1 + -0.320*X12 + -0.600*X16 + -0.480*X28 + 10.000*X31, // objective
      -1.000*X0 + X1 + X2 == 0.000,
	-1.060*X0 + X3 == 0.000,
	-1.000*X1 + 1.400*X12 <= 0.000,
	-1.000*X4 + -1.000*X5 + -1.000*X6 + -1.000*X7 + X12 + X13 == 0.000,
	-1.060*X4 + -1.060*X5 + -0.960*X6 + -0.860*X7 + X14 == 0.000,
	X4 + -1.000*X8 <= 80.000,
	X5 + -1.000*X9 <= 0.000,
	X6 + -1.000*X10 <= 0.000,
	X7 + -1.000*X11 <= 0.000,
	-1.000*X15 + X16 + X17 + X18 == 0.000,
	-0.430*X15 + X19 == 0.000,
	-1.000*X16 + 1.400*X28 <= 0.000,
	-0.430*X20 + -0.430*X21 + -0.390*X22 + -0.370*X23 + X30 == 0.000,
	X20 + X21 + X22 + X23 + -1.000*X28 + X29 + X31 == 44.000,
	X20 + -1.000*X24 <= 500.000,
	X21 + -1.000*X25 <= 0.000,
	X22 + -1.000*X26 <= 0.000,
	X23 + -1.000*X27 <= 0.000,
	2.364*X8 + 2.386*X9 + 2.408*X10 + 2.429*X11 + -1.000*X18 + 2.191*X24 + 2.219*X25 + 2.249*X26 + 2.279*X27 <= 0.000,
	-1.000*X2 + 0.109*X15 <= 0.000,
	-1.000*X13 + 0.109*X20 + 0.108*X21 + 0.108*X22 + 0.107*X23 <= 0.000,
	0.301*X0 + -1.000*X17 <= 0.000,
	0.301*X4 + 0.313*X5 + 0.313*X6 + 0.326*X7 + -1.000*X29 <= 0.000
    );
  }
    
