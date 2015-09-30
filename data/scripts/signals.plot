reset
set term postscript enhanced color eps "Helvetica,18"
set output "../../figs/signals.eps"

plot "../signals.dat" u 2:4 w l lt 0 lw 0 lc rgb "red" t "Random", \
"../signals.dat" u 2:6 w l lt -1 lw 1 lc rgb "green" t "PI", \
"../signals.dat" u 2:8 w l lt -1 lw 1 lc rgb "blue" t "Global", \
"../signals.dat" u 2:10 w l lt -1 lw 1 lc rgb "violet" t "Local"

set output

set output "../../figs/weights.eps"

set yrange [-5:5]
plot "../signals.dat" u 2:3 w l lt 0 lw 0 lc rgb "red" t "Random", \
"../signals.dat" u 2:5 w l lt -1 lw 1 lc rgb "green" t "PI", \
"../signals.dat" u 2:7 w l lt -1 lw 1 lc rgb "blue" t "Global", \
"../signals.dat" u 2:9 w l lt -1 lw 1 lc rgb "violet" t "Local"

set output
