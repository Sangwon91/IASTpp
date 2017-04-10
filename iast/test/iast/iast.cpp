#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "isotherm_utility.hpp"
#include "iast.hpp"

using namespace std;

int
main(int argc, char* argv[])
try {
    if (argc < 2)
        {
        cerr << "Usage: ./iast inputfile N x1 x2 x3 ..." << endl;
        cerr << "Example: ./iast mg-mof-74.txt 8.0 0.2 0.3 0.5" << endl;
        cerr << "Input File Format:\n" <<
                "# of component.\n" <<
                "isotherm data file path 1\n" <<
                "isotherm data file path 2\n" <<
                "..." << endl;
        return 1;
        }

    ifstream inputFile {argv[1]};

    int numComponents = 0;
    inputFile >> numComponents;
    string buffer;
    getline(inputFile, buffer);

    IsothermModeler modeler;
    Iast::IsothermVector isotherms;
    for (int i = 0; i < numComponents; ++i)
        {
        vector<double> x;
        vector<double> y;
        string path;

        getline(inputFile, path);
        ::readTwoColumns(path, x, y);

        isotherms.push_back(modeler.autofit(x, y));
        }

    double uptake = atof(argv[2]);

    vector<double> x;
    for (int i = 3; i < 3 + numComponents; ++i)
        x.push_back(atof(argv[i]));

    Iast::Mode mode = Iast::Mode::FIX_NX;
    Iast iast;

    iast.setIsotherms(isotherms).calculate(mode, uptake, x);

    double pressure;
    vector<double> y;

    tie(pressure, y) = iast.getResult();

    cout << setw(15) << "Pressure" << setw(15) << "X" << endl;
    cout << setw(15) << pressure;

    for (auto a : y)
        cout << setw(15) << a;

    cout << endl;

    return 0;
    }
catch (exception& e)
    {
    cerr << e.what() << endl;
    }
