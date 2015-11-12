reset
set term postscript enhanced color eps "Helvetica,28" size 7.5, 4
set output "../../figs/multiperform.eps"

stat "../m_cycles_trials.dat" u 1 nooutput

set key above center
set yrange [-0.05:1.05]
set xtics (1,10,20,30,40,50)
set ytics 0.5

set xlabel "Trials"
set ylabel "Rate"

plot "../m_cycles_trials.dat" u 1:2:3 w errorbars lw 3 lc rgb "green" t "Exploration", "" u 1:4:5 w errorbars lw 3 lc rgb "red" t "Homing success", "" u 1:6:7 w errorbars lw 3 lt 4 lc rgb "blue" t "Goal success"

#, '../reward.dat' u 2:4  w l lt 1 lw 0.1 lc rgb "violet" t "Value"
#, '' u 2:8  w l lt 1 lw 0.1 lc rgb "blue" t "Goal hits per timestep"
set output
