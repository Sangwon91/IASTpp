set title"[Henry Isotherm]\n[Parameters] k = 5"
set term pngcairo size 600,500 font 'Arial,15'
set output "henry.dat.png"
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
           0.01           0.05
          0.028           0.14
          0.046           0.23
          0.064           0.32
          0.082           0.41
            0.1            0.5
           0.12            0.6
           0.14            0.7
           0.16            0.8
           0.18            0.9
            0.2              1
            0.2              1
           0.22            1.1
           0.24            1.2
           0.26            1.3
           0.28            1.4
            0.3            1.5
           0.34            1.7
           0.38            1.9
           0.42            2.1
           0.46            2.3
            0.5            2.5
            0.5            2.5
            0.6              3
            0.7            3.5
            0.8              4
            0.9            4.5
              1              5
              1              5
            1.2              6
            1.4              7
            1.6              8
            1.8              9
              2             10
              2             10
            2.2             11
            2.4             12
            2.6             13
            2.8             14
              3             15
            3.2             16
            3.4             17
            3.6             18
            3.8             19
              4             20
            4.2             21
            4.4             22
            4.6             23
            4.8             24
              5             25
            5.2             26
            5.4             27
            5.6             28
            5.8             29
              6             30
            6.2             31
            6.4             32
            6.6             33
            6.8             34
              7             35
            7.2             36
            7.4             37
            7.6             38
            7.8             39
              8             40
            8.2             41
            8.4             42
            8.6             43
            8.8             44
              9             45
              9             45
            9.2             46
            9.4             47
            9.6             48
            9.8             49
             10             50
e
           0.01           0.05
            0.1            0.5
            0.2              1
            0.3            1.5
            0.5            2.5
              1              5
              2             10
              3             15
              4             20
              5             25
              6             30
              7             35
              8             40
              9             45
             10             50
