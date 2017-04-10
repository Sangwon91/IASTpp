set title"[Interpolator Isotherm]\n[Parameters] front (x, y) = (0, 0) , back (x, y) = (0.95, 170.718) "
set term pngcairo size 600,500 font 'Arial,15'
set output "h2o_cubtc.dat.png"
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
              0              0
           0.01       0.254824
           0.02       0.509648
           0.03       0.764472
           0.04         1.0193
           0.05        1.27412
           0.06        2.65154
           0.07        4.02896
           0.08        5.40639
           0.09        6.78381
            0.1        8.16123
            0.1        8.16123
           0.11        10.6909
           0.12        13.2206
           0.13        15.7503
           0.14          18.28
           0.15        20.8097
           0.16        23.6198
           0.17        26.4299
           0.18        29.2399
           0.19          32.05
            0.2        34.8601
           0.21         37.211
           0.22        39.5618
           0.23        41.9127
           0.24        44.2635
           0.25        46.6144
           0.26        48.3208
           0.27        50.0272
           0.28        51.7336
           0.29          53.44
            0.3        55.1464
           0.31        56.3177
           0.32        57.4891
           0.33        58.6604
           0.34        59.8318
           0.35        61.0031
           0.36        61.7986
           0.37         62.594
           0.38        63.3895
           0.39        64.1849
            0.4        64.9804
           0.41        65.5262
           0.42         66.072
           0.43        66.6178
           0.44        67.1636
           0.45        67.7094
           0.46        68.0912
           0.47        68.4731
           0.48        68.8549
           0.49        69.2368
            0.5        69.6186
           0.51        69.8928
           0.52        70.1669
           0.53        70.4411
           0.54        70.7152
           0.55        70.9894
           0.56        71.1964
           0.57        71.4034
           0.58        71.6103
           0.59        71.8173
            0.6        72.0243
           0.61        72.2069
           0.62        72.3895
           0.63        72.5722
           0.64        72.7548
           0.65        72.9374
           0.66        73.1806
           0.67        73.4237
           0.68        73.6669
           0.69          73.91
            0.7        74.1532
           0.71        74.6844
           0.72        75.2157
           0.73        75.7469
           0.74        76.2782
           0.75        76.8094
           0.76        78.2133
           0.77        79.6172
           0.78        81.0211
           0.79         82.425
            0.8        83.8289
           0.81        87.2381
           0.82        90.6473
           0.83        94.0566
           0.84        97.4658
           0.85        100.875
           0.86        107.183
           0.87        113.491
           0.88          119.8
           0.89        126.108
            0.9        132.416
           0.91        140.076
           0.92        147.737
           0.93        155.397
           0.94        163.058
e
              0              0
           0.05        1.27412
            0.1        8.16123
           0.15        20.8097
            0.2        34.8601
           0.25        46.6144
            0.3        55.1464
           0.35        61.0031
            0.4        64.9804
           0.45        67.7094
            0.5        69.6186
           0.55        70.9894
            0.6        72.0243
           0.65        72.9374
            0.7        74.1532
           0.75        76.8094
            0.8        83.8289
           0.85        100.875
            0.9        132.416
           0.95        170.718
