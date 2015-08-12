reset
set term postscript enhanced color eps "Helvetica,18"
set output "../../figs/gv_performance.eps"

plot "../globalvector.dat" u 2:7 w l lt -1 lw 0.1 lc rgb "green" t "Exploration rate", '../reward.dat' u 2:4  w l lt 1 lw 0.1 lc rgb "violet" t "Value"
#, '' u 2:8  w l lt 1 lw 0.1 lc rgb "blue" t "Goal hits per timestep"
set output
