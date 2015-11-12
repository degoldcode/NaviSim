reset
set term postscript enhanced color eps "Helvetica,28" size 7.5, 4
set output "../../figs/statcycles.eps"

stat "../stat_cycles.dat" u 1 nooutput

set key above center
set yrange [-0.05:1.05]
set xtics STATS_max/10
set xrange [0.5:STATS_max+0.5]
set y2range [-0.05*50:50*1.05]
set y2tics 
set ytics 0.5 nomirror

set xlabel "Cycle"
set ylabel "Rate"
set y2label "Trial"

plot "../stat_cycles.dat" u 1:2:3 w errorbars lw 3 lc rgb "green" t "Ratio", "" u 1:($4/50):($5/50) w errorbars lw 3 lc rgb "red" t "Time convergence", "" u 1:6 w p lw 3 pt 5 lc rgb "blue" t "Convergence"

#, '../reward.dat' u 2:4  w l lt 1 lw 0.1 lc rgb "violet" t "Value"
#, '' u 2:8  w l lt 1 lw 0.1 lc rgb "blue" t "Goal hits per timestep"
set output
