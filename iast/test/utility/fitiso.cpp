#include <iostream>
#include <iomanip>
#include <exception>
#include <vector>
#include <sstream>
#include <string>

#include "isotherm_utility.hpp"

int
main(int argc, char* argv[])
try {
    if (argc < 2)
        {
        std::cerr << "Usage: fitiso datafile." << std::endl;
        return 1;
        }

    IsothermModeler modeler;

    std::vector<double> x;
    std::vector<double> y;
    ::readTwoColumns(argv[1], x, y);
    auto iso = modeler.autofit(x, y);

    std::string filename (argv[1]);

    std::stringstream ss;
    std::string buffer;
    ss << iso->getInfoString();
    std::getline(ss, buffer);
    std::cout << "set title" << '"' << buffer;
    std::getline(ss, buffer);
    std::cout << "\\n" << buffer << '"' << std::endl;

    std::cout << "set term pngcairo size 600,500 font 'Arial,15'\n" <<
                 "set output \"" + filename + ".png\"\n" <<
                 "set xtics format '10^{%L}'\n"
                 "set ytics format '10^{%L}'\n"
                 "set xlabel 'P (bar)'\n" <<
                 "set ylabel 'Uptake (mol/kg)'\n" <<
                 "set style line 1 lc rgb 'blue'  lw 2\n" <<
                 "set style line 2 lc rgb 'black' ps 1.5 pt 9\n" <<
                 "set grid\n" <<
                 "set key left\n" <<
                 "set logscale xy\n"
                 "plot '-' w l ls 1 title 'Fit', '-' w p ls 2 title 'Data'" <<
                 std::endl;

    for (int i = 0; i < x.size() - 1; ++i)
        {
        double du = (x[i + 1] - x[i]) / 5.0;
        for (double u = x[i]; u < x[i + 1]; u += du)
            std::cout << std::setw(15) << u <<
                         std::setw(15) << iso->loading(u) <<
                         std::endl;
        }

    std::cout << "e" << std::endl;

    for (int i = 0; i < x.size(); ++i)
        std::cout << std::setw(15) << x[i] <<
                     std::setw(15) << y[i] <<
                     std::endl;

    std::cerr << modeler.getRSquare() << std::endl;

    return 0;
    }
catch (std::exception& e)
    {
    std::cerr << e.what() << std::endl;
    return 1;
    }
