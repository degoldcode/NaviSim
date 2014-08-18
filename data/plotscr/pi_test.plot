reset
set term postscript enhanced color eps "Helvetica,12" size 6,1
set output "../pi_test_angle.eps"

set xlabel "t"
set ylabel "x"

set grid
PI=3.14159
plot "../control.dat" u 0:5 w l t "PI angle avg", "" u 0:4 w l t "PI angle max", "../agent.dat" u 0:($5*180/PI) w l t "Theta"
set output

set output "../pi_test_length.eps"
plot "../control.dat" u 0:6 w l t "PI length", "../agent.dat" u 0:6 w l t "Dist" 
set output
