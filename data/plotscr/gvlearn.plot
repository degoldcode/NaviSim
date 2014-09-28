reset
set term postscript enhanced color eps "Helvetica,18" size 4,1.25
set output "../gvlearn.eps"

set xlabel "Trials"
set ylabel "Rate"
set xrange [-0.5:400.5]
set yrange [-0.05:1.05]
set xtics 50
set ytics 0.5
set key above
set grid

plot "../gvlearn.dat" u 1:3 w l lw 4  lc rgb "#0022FF" t "Average goal success rate per trial", "" u 1:5 w l lw 4  lc rgb "#00FF22" lt 1 t "Exploration rate"  
#, "" u 1:4 w l lw 4 lc rgb "#FF2200" t "Average homing success rate per trial"
#, "../control.dat" u (0.1*$1):16 w l lc rgb "#00FF22" lt 1 t "Exploration rate"  
#, "" u 0:3 w p pt 7 ps 0.2 lc rgb "blue" t "GV length" 
#, "" u 0:2 w p pt 7 ps 0.2 lc rgb "green" t "PI length", "" u 0:3 w p pt 7 ps 0.2 lc rgb "blue" t "GV length" 
set output


set terminal pngcairo  transparent enhanced font "Helvetica,64" size 3200,3600
set output "../gvlearnavg.png"
set key samplen 2
set xlabel "Trials"
set ylabel "Exploration rate"
set style fill transparent solid 0.4 noborder

set multiplot layout 3,1

#set size 1.0,0.3
#set origin 0.0,0.7
plot '../allcycles.dat' using 0:($1-$2):($1+$2) with filledcurves lc rgb "#0044FF" title '95% confidence', \
     '' using 0:1 w l lt 1 lw 8 lc rgb "#0044FF" title 'Blue', \
     '' using 0:($5-$6):($5+$6) with filledcurves lc rgb "#FFEE00" title '95% confidence', \
     '' using 0:5 w l lt 1 lw 8 lc rgb "#FFEE00" title 'Yellow', \

#set size 1.0,0.3
#set origin 0.0,0.4
set ylabel "Probability"
plot '' using 0:($9-$10):($9+$10) with filledcurves lc rgb "#0044FF" title '95% confidence', \
     '' using 0:9 w l lt 1 lw 8 lc rgb "#0044FF" title 'Blue', \
     '' using 0:($11-$12):($11+$12) with filledcurves lc rgb "#FFEE00" title '95% confidence', \
     '' using 0:11 w l lt 1 lw 8 lc rgb "#FFEE00" title 'Yellow'

#set size 1.0,0.3
#set origin 0.0,0.1
set yrange [0:250]
set ytics 50
set ylabel "Accumulated visits"
plot '' using 0:($3-$4):($3+$4) with filledcurves lc rgb "#0044FF" title '95% confidence', \
     '' using 0:3 w l lt 1 lw 8 lc rgb "#0044FF" title 'Blue', \
     '' using 0:($7-$8):($7+$8) with filledcurves lc rgb "#FFEE00" title '95% confidence', \
     '' using 0:7 w l lt 1 lw 8 lc rgb "#FFEE00" title 'Yellow'

#set bmargin 1.0
#set tmargin 1.0
#set size 1.0,0.1
#set origin 0.0,-0.0
#set yrange [0.49:0.51]
#unset xtics
#set ytics 50
#set ylabel "Choice"
#unset key
#set style fill solid 0.6 border
#plot '' using 0:(0.0):(1-$13) with filledcurves lc rgb "#0044FF" title 'Blue', \
     '' using 0:13:(0.0) with filledcurves lc rgb "#FFEE00" title 'Yellow'

unset multiplot
set output
