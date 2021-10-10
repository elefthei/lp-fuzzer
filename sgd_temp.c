typedef double fp64;

// Iterate over d
int grad_check($ws, $xis, y) {
  return y*($dot_wsxis) >= 1;
}

int main() {

    fp64 $existentials;

    __GADGET_sgd($d, $n,
        $dataset
    );

    int check = __GADGET_check(
        $grad_checks);

    return check;
}

