
  int main() {
    typedef double fp32;
    fp32 X0 = __GADGET_exist(), X1 = __GADGET_exist(), X2 = __GADGET_exist(), X3 = __GADGET_exist(), X4 = __GADGET_exist(), X5 = __GADGET_exist(), X6 = __GADGET_exist(), X7 = __GADGET_exist(), X8 = __GADGET_exist(), X9 = __GADGET_exist(), X10 = __GADGET_exist(), X11 = __GADGET_exist(), X12 = __GADGET_exist(), X13 = __GADGET_exist(), X14 = __GADGET_exist(), X15 = __GADGET_exist(), X16 = __GADGET_exist(), X17 = __GADGET_exist(), X18 = __GADGET_exist(), X19 = __GADGET_exist(), X20 = __GADGET_exist(), X21 = __GADGET_exist(), X22 = __GADGET_exist(), X23 = __GADGET_exist(), X24 = __GADGET_exist(), X25 = __GADGET_exist(), X26 = __GADGET_exist(), X27 = __GADGET_exist(), X28 = __GADGET_exist(), X29 = __GADGET_exist(), X30 = __GADGET_exist(), X31 = __GADGET_exist(), X32 = __GADGET_exist(), X33 = __GADGET_exist(), X34 = __GADGET_exist(), X35 = __GADGET_exist(), X36 = __GADGET_exist(), X37 = __GADGET_exist(), X38 = __GADGET_exist(), X39 = __GADGET_exist(), X40 = __GADGET_exist();

    __GADGET_maximize(
      -16.500*X21 + 12.000*X26 + 16.000*X28 + 0.088*X30 + 0.088*X33, // objective
      -1.000*X0 + -1.000*X7 + -1.000*X14 + X22 == 0.000,
	-1.000*X1 + -1.000*X8 + -1.000*X15 + X23 == 0.000,
	-1.000*X2 + -1.000*X9 + -1.000*X16 + X24 == 0.000,
	-1.000*X3 + -1.000*X10 + -1.000*X17 + X25 == 0.000,
	-1.000*X4 + -1.000*X11 + -1.000*X18 + X26 == 0.000,
	-1.000*X5 + -1.000*X12 + -1.000*X19 + X27 == 0.000,
	-1.000*X6 + -1.000*X13 + -1.000*X20 + X28 == 0.000,
	-0.290*X29 + X31 == 0.000,
	-0.170*X29 + X32 == 0.000,
	-0.540*X29 + X34 == 0.000,
	-1.000*X21 + X29 == 0.000,
	X0 + X1 + X2 + X3 + X4 + X5 + X6 + -1.000*X31 == 0.000,
	X7 + X8 + X9 + X10 + X11 + X12 + X13 + -1.000*X32 == 0.000,
	X14 + X15 + X16 + X17 + X18 + X19 + X20 + -1.000*X34 == 0.000,
	99.186*X0 + 82.043*X1 + 83.994*X2 + 85.614*X3 + 98.064*X4 + 91.626*X5 + 90.496*X6 + 1.238*X30 + -2.105*X31 + -1.000*X35 >= 0.000,
	94.636*X0 + 79.405*X1 + 80.379*X2 + 80.368*X3 + 92.716*X4 + 90.038*X5 + 89.104*X6 + 3.429*X30 + -1.372*X31 + -1.000*X35 >= 0.000,
	98.090*X0 + 81.470*X1 + 83.220*X2 + 84.519*X3 + 96.866*X4 + 91.266*X5 + 90.149*X6 + 1.558*X30 + -2.025*X31 + -1.000*X35 >= 0.000,
	103.058*X0 + 95.022*X1 + 98.646*X2 + 88.466*X3 + 101.663*X4 + 102.518*X5 + 106.467*X6 + 1.271*X30 + -2.161*X31 + -1.000*X36 >= 0.000,
	98.703*X0 + 92.895*X1 + 95.383*X2 + 82.880*X3 + 97.330*X4 + 101.173*X5 + 105.477*X6 + 2.521*X30 + -1.009*X31 + -1.000*X36 >= 0.000,
	102.022*X0 + 94.571*X1 + 97.980*X2 + 87.333*X3 + 100.650*X4 + 102.214*X5 + 106.219*X6 + 1.550*X30 + -2.014*X31 + -1.000*X36 >= 0.000,
	99.186*X14 + 82.043*X15 + 83.994*X16 + 85.614*X17 + 98.064*X18 + 91.626*X19 + 90.496*X20 + 1.750*X33 + -2.975*X34 + -1.000*X39 >= 0.000,
	95.171*X14 + 79.729*X15 + 80.829*X16 + 81.038*X17 + 93.417*X18 + 90.224*X19 + 89.256*X20 + 4.419*X33 + -2.209*X34 + -1.000*X39 >= 0.000,
	97.110*X14 + 80.940*X15 + 82.499*X16 + 83.485*X17 + 95.866*X18 + 90.941*X19 + 89.846*X20 + 2.745*X33 + -2.745*X34 + -1.000*X39 >= 0.000,
	103.058*X14 + 95.022*X15 + 98.646*X16 + 88.466*X17 + 101.663*X18 + 102.518*X19 + 106.467*X20 + 1.644*X33 + -2.795*X34 + -1.000*X40 >= 0.000,
	99.190*X14 + 93.161*X15 + 95.809*X16 + 83.614*X17 + 97.869*X18 + 101.329*X19 + 105.584*X20 + 4.319*X33 + -2.160*X34 + -1.000*X40 >= 0.000,
	101.088*X14 + 94.148*X15 + 97.342*X16 + 86.245*X17 + 99.778*X18 + 101.938*X19 + 106.002*X20 + 2.627*X33 + -2.627*X34 + -1.000*X40 >= 0.000,
	-107.520*X31 + 0.730*X35 + 0.410*X36 >= 0.000,
	-97.410*X32 + 0.840*X37 + 0.270*X38 >= 0.000,
	-98.500*X34 + 0.810*X39 + 0.310*X40 >= 0.000,
	91.963*X7 + 78.091*X8 + 80.746*X9 + 77.374*X10 + 88.354*X11 + 88.580*X12 + 88.182*X13 + -1.000*X37 == 0.000,
	96.136*X7 + 90.996*X8 + 93.957*X9 + 79.780*X10 + 94.111*X11 + 99.832*X12 + 105.076*X13 + -1.000*X38 == 0.000,
	X30 + -1.500*X31 + -1.500*X32 + X33 + -1.500*X34 <= 0.000,
	6.000*X0 + -2.000*X1 + 7.000*X2 + 14.000*X3 + 80.000*X4 + 4.000*X5 + -1.000*X6 + -16.000*X31 <= 0.000,
	X30 + -1.700*X31 <= 0.000,
	4.000*X0 + 0.500*X1 + 4.500*X2 + 7.200*X3 + 70.000*X4 + 3.600*X5 + 1.200*X6 + -12.000*X31 <= 0.000,
	50.300*X0 + -15.600*X1 + 57.900*X2 + 102.300*X3 + 113.000*X4 + 28.900*X5 + 5.000*X6 + -61.000*X31 <= 0.000,
	6.000*X7 + -2.000*X8 + 7.000*X9 + 14.000*X10 + 80.000*X11 + 4.000*X12 + -1.000*X13 + -16.000*X32 <= 0.000,
	4.000*X7 + 0.500*X8 + 4.500*X9 + 7.200*X10 + 70.000*X11 + 3.600*X12 + 1.200*X13 + -12.000*X32 <= 0.000,
	50.300*X7 + -15.600*X8 + 57.900*X9 + 102.300*X10 + 113.000*X11 + 28.900*X12 + 5.000*X13 + -61.000*X32 <= 0.000,
	6.000*X14 + -2.000*X15 + 7.000*X16 + 14.000*X17 + 80.000*X18 + 4.000*X19 + -1.000*X20 + -16.000*X34 <= 0.000,
	X33 + -1.700*X34 <= 0.000,
	4.000*X14 + 0.500*X15 + 4.500*X16 + 7.200*X17 + 70.000*X18 + 3.600*X19 + 1.200*X20 + -12.000*X34 <= 0.000,
	50.300*X14 + -15.600*X15 + 57.900*X16 + 102.300*X17 + 113.000*X18 + 28.900*X19 + 5.000*X20 + -61.000*X34 <= 0.000
    );
  }
    
