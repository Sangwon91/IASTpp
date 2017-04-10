#include <iostream>

#include "../../interpolator.hpp"
#include "../../linear_interpolator.hpp"

using namespace std;

int
main(int argc, char* argv[])
    {
    Interpolator&& interp = LinearInterpolator {};

    interp.pushBack(0.0, 0.0).
           pushBack(1.0, 1.0).
           pushBack(2.0, 5.0).
           pushBack(4.0, -1.0).
           pushBack(10.0, 3.0);

    for (double x = 0.0; x < 10.01; x += 0.5)
        cout << x << "  " << interp(x) << endl;

    cout << interp.getXData().front() << endl;
    cout << interp.getYData().front() << endl;

    cout << interp.getXData().back() << endl;
    cout << interp.getYData().back() << endl;

    return 0;
    }
