reset
set term postscript enhanced color eps "Helvetica,18"
set output "../../figs/signals.eps"

plot "../signals.dat" u 2:6 w l lt 1 lw 1 lc rgb "#f39c12" t "Random", \
"../signals.dat" u 2:3 w l lt 1 lw 1 lc rgb "#2ecc71" t "HV", \
"../signals.dat" u 2:4 w l lt 1 lw 1 lc rgb "#3498db" t "GV", \
"../signals.dat" u 2:5 w l lt 1 lw 1 lc rgb "#e74c3c" t "LV"


set output

set output "../../figs/turning.eps"
plot "../agent.dat" u 8:9 w l lt 1 lw 2 lc rgb "red" t ""
#set yrange [-5:5]
#plot "../signals.dat" u 2:3 w l lt 0 lw 0 lc rgb "red" t "Random", \
#"../signals.dat" u 2:5 w l lt -1 lw 1 lc rgb "green" t "PI", \
#"../signals.dat" u 2:7 w l lt -1 lw 1 lc rgb "blue" t "Global", \
#"../signals.dat" u 2:9 w l lt -1 lw 1 lc rgb "violet" t "Local"

#set output
