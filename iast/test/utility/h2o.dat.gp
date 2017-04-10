set title"[Interpolator Isotherm]\n[Parameters] front (x, y) = (3, 0.595238) , back (x, y) = (15, 37.2619) "
set term pngcairo size 600,500 font 'Arial,15'
set output "h2o.dat.png"
set xtics format '10^{%L}'
set ytics format '10^{%L}'
set xlabel 'P (bar)'
set ylabel 'Uptake (mol/kg)'
set style line 1 lc rgb 'blue'  lw 2
set style line 2 lc rgb 'black' ps 1.5 pt 9
set grid
set key left
set logscale xy
plot '-' w l ls 1 title 'Fit', '-' w p ls 2 title 'Data'
              3       0.595238
        3.19355       0.658986
         3.3871       0.722734
        3.58064       0.786482
        3.77419        0.85023
        3.96774       0.913978
        3.96774       0.913978
        4.18064        1.04839
        4.39355        1.18279
        4.60645         1.3172
        4.81936        1.45161
        5.03226        1.58602
        5.23549        1.72081
        5.43871         1.8556
        5.64194         1.9904
        5.84516        2.12519
        6.04839        2.25998
        6.04839        2.25998
        6.25162        2.53763
        6.45484        2.81528
        6.65807        3.09293
        6.86129        3.37058
        7.06452        3.64823
        7.25807        4.06912
        7.45162        4.49001
        7.64516        4.91091
        7.83871         5.3318
        8.03226        5.75269
        8.03226        5.75269
        8.22581        6.67357
        8.41936        7.59445
         8.6129        8.51534
        8.80645        9.43622
              9        10.3571
        9.20322        13.1348
        9.40644        15.9124
        9.60966        18.6901
        9.81288        21.4677
        10.0161        24.2454
        10.2097        25.3091
        10.4032        26.3729
        10.5968        27.4366
        10.7903        28.5004
        10.9839        29.5641
        10.9839        29.5641
        11.1871         30.556
        11.3903         31.548
        11.5936        32.5399
        11.7968        33.5319
             12        34.5238
        12.2032          34.73
        12.4064        34.9362
        12.6097        35.1425
        12.8129        35.3487
        13.0161        35.5549
        13.2097        35.6187
        13.4032        35.6824
        13.5968        35.7462
        13.7903        35.8099
        13.9839        35.8737
        13.9839        35.8737
        14.1871        36.1513
        14.3903         36.429
        14.5936        36.7066
        14.7968        36.9843
e
              3       0.595238
        3.96774       0.913978
        5.03226        1.58602
        6.04839        2.25998
        7.06452        3.64823
        8.03226        5.75269
              9        10.3571
        10.0161        24.2454
        10.9839        29.5641
             12        34.5238
        13.0161        35.5549
        13.9839        35.8737
             15        37.2619
