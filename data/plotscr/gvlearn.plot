reset
set term postscript enhanced color eps "Helvetica,14" size 4,1.5
set output "../gvlearn.eps"

set xlabel "Trials"
set ylabel "Rate"
#set xrange [-50:6050]
set yrange [-0.1:1.1]
set ytics 0.5 nomirror
set xtics autofreq nomirror
set key above
set grid

plot "../gvlearn.dat" u 1:3 w l lc rgb "#0022FF" t "Average goal success rate per trial", "" u 1:4 w l lt 1 lc rgb "#FF2200" t "Average homing success rate per trial", "" u 1:5 w l lc rgb "#00FF22" lt 1 t "Exploration rate"  
#, "../control.dat" u (0.1*$1):16 w l lc rgb "#00FF22" lt 1 t "Exploration rate"  
#, "" u 0:3 w l lt 1 lc rgb "blue" t "GV length" 
#, "" u 0:2 w l lt 1 lc rgb "green" t "PI length", "" u 0:3 w l lt 1 lc rgb "blue" t "GV length" 
set output

