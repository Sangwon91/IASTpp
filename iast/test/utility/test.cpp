#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include "../../isotherm_utility.hpp"

int
main()
    {
    using namespace std;

    IsothermModeler modeler;

    vector<double> x;
    vector<double> y;

    ::readTwoColumns("co2.dat", x, y);
    auto iso = modeler.autofit(x, y);

    cout << "1.";
    cout << iso->getInfoString() << endl;
    for (int i = 0; i < x.size(); ++i)
        cout << setw(15) << x[i] << setw(15) << iso->loading(x[i]) << setw(15) << y[i] << endl;

    ::readTwoColumns("h2o.dat", x, y);
    iso = modeler.autofit(x, y);

    cout << "2.";
    cout << iso->getInfoString() << endl;
    for (int i = 0; i < x.size(); ++i)
        cout << setw(15) << x[i] << setw(15) << iso->loading(x[i]) << setw(15) << y[i] << endl;

    ::readTwoColumns("h2o_zn.dat", x, y);
    iso = modeler.autofit(x, y);

    cout << "3.";
    cout << iso->getInfoString() << endl;
    for (int i = 0; i < x.size(); ++i)
        cout << setw(15) << x[i] << setw(15) << iso->loading(x[i]) << setw(15) << y[i] << endl;

    ::readTwoColumns("ni_h2o.dat", x, y);
    iso = modeler.autofit(x, y);
    //iso = ::fitIsotherm("bet", x, y);

    cout << "4.";
    cout << iso->getInfoString() << endl;
    for (int i = 0; i < x.size(); ++i)
        cout << setw(15) << x[i] << setw(15) << iso->loading(x[i]) << setw(15) << y[i] << endl;

    ::readTwoColumns("ni_co2.dat", x, y);
    iso = modeler.autofit(x, y);

    cout << "5.";
    cout << iso->getInfoString() << endl;
    for (int i = 0; i < x.size(); ++i)
        cout << setw(15) << x[i] << setw(15) << iso->loading(x[i]) << setw(15) << y[i] << endl;

    ::readTwoColumns("n2_irmof-1.dat", x, y);
    iso = modeler.autofit(x, y);

    cout << "6.";
    cout << iso->getInfoString() << endl;
    for (int i = 0; i < x.size(); ++i)
        cout << setw(15) << x[i] << setw(15) << iso->loading(x[i]) << setw(15) << y[i] << endl;

    ::readTwoColumns("ethane.dat", x, y);
    iso = modeler.autofit(x, y);

    cout << "7.";
    cout << iso->getInfoString() << endl;
    for (int i = 0; i < x.size(); ++i)
        cout << setw(15) << x[i] << setw(15) << iso->loading(x[i]) << setw(15) << y[i] << endl;

    ::readTwoColumns("h2o_mgmof74.dat", x, y);
    iso = modeler.autofit(x, y);

    cout << "8.";
    cout << iso->getInfoString() << endl;
    for (int i = 0; i < x.size(); ++i)
        cout << setw(15) << x[i] << setw(15) << iso->loading(x[i]) << setw(15) << y[i] << endl;

    ::readTwoColumns("lang.dat", x, y);
    iso = modeler.autofit(x, y);

    cout << "9.";
    cout << iso->getInfoString() << endl;
    for (int i = 0; i < x.size(); ++i)
        cout << setw(15) << x[i] << setw(15) << iso->loading(x[i]) << setw(15) << y[i] << endl;

    ::readTwoColumns("henry.dat", x, y);
    iso = modeler.autofit(x, y);

    cout << "10.";
    cout << iso->getInfoString() << endl;
    for (int i = 0; i < x.size(); ++i)
        cout << setw(15) << x[i] << setw(15) << iso->loading(x[i]) << setw(15) << y[i] << endl;

    ::readTwoColumns("o2_irmof1.dat", x, y);
    iso = modeler.autofit(x, y);

    cout << "11.";
    cout << iso->getInfoString() << endl;
    for (int i = 0; i < x.size(); ++i)
        cout << setw(15) << x[i] << setw(15) << iso->loading(x[i]) << setw(15) << y[i] << endl;


    iso = modeler.autofit(x, y);
    cout << "11.";
    cout << iso->getInfoString() << endl;
    for (int i = 0; i < x.size(); ++i)
        cout << setw(15) << x[i] << setw(15) << iso->loading(x[i]) << setw(15) << y[i] << endl;

    return 0;
    }
