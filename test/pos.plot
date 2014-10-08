reset
set term x11 enhanced font "Helvetica, 32" title "Trajectory" persist

set size square
set xlabel "x"
set ylabel "y"

SCALE = 20.
set xrange [-SCALE/2:SCALE/2]
set yrange [-SCALE/2:SCALE/2]

plot "traj.dat" u 1:2 w p pt 7 ps 0.55 lc rgb "red" t "PI", "" u 3:4 w l lt 1 lw 1 lc rgb "blue" t "Real", "" u 5:6 w p pt 7 ps 0.55 lc rgb "green" t "GV" , "" u (0.0):(0.0) w p pt 4 lc rgb "gray" t "Home", "" u (0.0):(5.0):(0.2) w circles fs solid lc rgb "gray" t "Goal", 
set output
