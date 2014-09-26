reset
set term postscript enhanced color eps "Helvetica,14" size 8,2
set output "../gvlearn.eps"

set xlabel "Trials"
set ylabel "Rate"
#set xrange [-50:6050]
set yrange [-0.02:1.02]
set ytics 0.2 nomirror
set xtics autofreq nomirror
set key above
set grid

plot "../gvlearn.dat" u 1:3 w l lw 4  lc rgb "#0022FF" t "Average goal success rate per trial", "" u 1:4 w l lw 4 lc rgb "#FF2200" t "Average homing success rate per trial", "" u 1:5 w l lw 4  lc rgb "#00FF22" lt 1 t "Exploration rate"  
#, "../control.dat" u (0.1*$1):16 w l lc rgb "#00FF22" lt 1 t "Exploration rate"  
#, "" u 0:3 w p pt 7 ps 0.2 lc rgb "blue" t "GV length" 
#, "" u 0:2 w p pt 7 ps 0.2 lc rgb "green" t "PI length", "" u 0:3 w p pt 7 ps 0.2 lc rgb "blue" t "GV length" 
set output


set terminal pngcairo  transparent enhanced font "Helvetica,64" size 3200,1200
set output "../gvlearnavg.png"
set key samplen 2
set style fill transparent solid 0.4 noborder
plot '../allcycles.dat' using 0:($1-$2):($1+$2) with filledcurves lc rgb "#64FF64" title '95% confidence', \
     '' using 0:1 w l lt 1 lw 8 lc rgb "#00FF00" title 'Mean exploration rate', \
     '' using 0:($3-$4):($3+$4) with filledcurves lc rgb "#6464FF" title '95% confidence', \
     '' using 0:3 w l lt 1 lw 8 lc rgb "#0000FF" title 'Mean success rate'
set output
