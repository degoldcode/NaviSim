reset
set term postscript enhanced color eps "Helvetica,12" size 5,1
set output "../gvlearn.eps"

set xlabel "Time {/Helvetica-Oblique t} [s]"
set ylabel "Rate"
#set xrange [-50:6050]
set yrange [-0.1:1.1]
set ytics 0.5 nomirror
set xtics autofreq nomirror
set key right outside
set grid

plot "../gvlearn.dat" u (0.1*$2):3 w l lc rgb "#0022FF" t "Average goal success rate per trial", "" u (0.1*$2):4 w l lt 1 lc rgb "#FF2200" t "Average homing success rate per trial", "../control.dat" u (0.1*$1):16 w l lc rgb "#00FF22" lt 1 t "Exploration rate"  
#, "" u 0:3 w l lt 1 lc rgb "blue" t "GV length" 
#, "" u 0:2 w l lt 1 lc rgb "green" t "PI length", "" u 0:3 w l lt 1 lc rgb "blue" t "GV length" 
set output

