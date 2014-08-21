reset
set term postscript enhanced color eps "Helvetica, 18" size 10,2

f(x) = 45
set key textcolor rgb "white"
set xlabel "Time t [ts]"
set ylabel "Neuron index"
set cblabel "Firing rate r [a.u.]" offset 1.5
#set xrange [0:50.4]
set yrange [-0.5:359.5]
#set xtics ("1000" 10, "2000" 20, "3000" 30, "4000" 40, "5000" 50)
set ytics 45
set output "../out.eps"
set view map
plot "../out.mat" matrix with image, "../control.dat" u 0:6 w l lw 3 lc rgb "green" t "Maximum activity"

f(x) = (x<0) ? x+180 : x

#set output "../pi.eps"
#set view map
#plot "../pi.mat" matrix with image

set output "../mu.eps"
set view map
plot "../mu.mat" matrix with image

set output "../ref.eps"
set view map
plot "../ref.mat" matrix with image

set output "../lv.eps"
set view map
plot "../lv.mat" matrix with image

set output "../gv.eps"
set view map
plot "../gv.mat" matrix with image, "../control.dat" u 0:(($11<0)?($11+180):$11) w l lw 3 lc rgb "green" t "Maximum activity"

set output "../gv_out.eps"
set view map
plot "../gv_out.mat" matrix with image, "../control.dat" u 0:(($11<0)?($11+180):$11) w l lw 3 lc rgb "green" t "Maximum activity" 

set output "../gv_w.eps"
set view map
plot "../gv_w.mat" matrix with image

set output "../lv_w.eps"
set view map
plot "../lv_w.mat" matrix with image, "../control.dat" u 0:(($21<0)?($21+180):$21) w l lw 3 lc rgb "green" t "Maximum weight" 

