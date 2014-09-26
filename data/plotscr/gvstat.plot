reset
set term postscript enhanced color eps "Helvetica,14" size 3,3
set output "../gl_stats.eps"

set size square

set xlabel "Distance to learned goal"
set ylabel "Exploration rate"
#set xrange [-50:6050]
set yrange [-0.01:0.06]
set ytics autofreq nomirror
set xtics autofreq nomirror
unset key
set grid

f(x)=b*log(c+d*exp(e*x+a))
g(x)=a*x+b
#fit g(x) '../gl_stats.dat' using 4:$2 via a,b
plot "../gl_stats.dat" u 4:2 w p pt 1 lc rgb "#FF2200" t "Learned goal"
#, f(x) w l lt 1 lc rgb "green" t "linear fit"
#, "" u 1:4 w p pt 7 ps 0.2 lc rgb "#FF2200" t "Average homing success rate per trial", "" u 1:5 w p pt 7 ps 0.2 lc rgb "#00FF22" lt 1 t "Exploration rate"  
set output

