#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include "isotherm_utility.hpp"

int
main(int argc, char* argv[])
    {
    using namespace std;

    vector<string> isonames
        {"henry", "langmuir", "lf", "bet", "quadratic", "dsl", "dslf"};

    vector<double> x;
    vector<double> y;
    ::readTwoColumns(argv[1], x, y);

    IsothermModeler modeler;

    for (const auto& isoname : isonames)
        {
        auto iso = modeler.fit(isoname, x, y);
        cout << iso->getInfoString() << endl;

        cout << "Isotherm : " << isoname << endl;

        cout << setw(15) << "x" <<
                setw(15) << "y" <<
                setw(15) << "y fit" <<
                endl;

        for (int i = 0; i < x.size(); ++i)
            cout << setw(15) << x[i] <<
                    setw(15) << y[i] <<
                    setw(15) << iso->loading(x[i]) <<
                    endl;

        cout << "Error = " << modeler.getError() << endl;
        cout << "R2    = " << modeler.getRSquare() << endl;
        }

    return 0;
    }

