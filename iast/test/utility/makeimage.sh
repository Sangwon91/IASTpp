#!/bin/bash

for i in *.dat; do
    ./fitiso $i > "$i.gp"
done

for i in *.gp; do
    gnuplot $i
done
