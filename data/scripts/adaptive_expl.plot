reset
set term postscript enhanced color eps "Helvetica,18"
set output "../../figs/beta.eps"

set palette maxcolors 6
set palette defined (1 "#e74c3c", 2 "#f1c40f", 3 "#2ecc71", 4 "#3498db", 5 "#9b59b6", 6 "#34495e")

#set xrange [0:70000]
set yrange [0:10]

plot "../adaptive_expl.dat" u 2:5:3 w l lt -1 lw 1 lc palette t "Value", "" u 2:6 w l lt -1 lw 1 lc rgb "red" t "Avg R", "" u 2:(1/$4) w l lt -1 lw 1 lc rgb "yellow" t "Term", "" u 2:($8/100) w l lt -1 lw 1 lc rgb "violet" t "Antiterm", "" u 2:7 w l lt -1 lw 1 lc rgb "green" t "Exploration rate", "" u 2:4 w l lt -1 lw 1  lc rgb "blue" t "Beta"
set output
