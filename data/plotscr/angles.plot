reset
set term postscript enhanced color eps "Helvetica,12" size 8,2
set output "../angles.eps"

set xlabel "t"
set ylabel "Ampl"

set grid

plot "../control.dat" u 0:2 w l t "PI angle", "" u 0:3 w l t "GV angle", "" u 0:4 w l t "FB error", "" u 0:6 w l t "Outb", "" u 0:7 w l t "Inv HV", "../agent.dat" u 0:3 w l t "Phi"
#u 0:1 w l t 'Chaotic Map', ""
set output
