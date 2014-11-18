reset
set term postscript enhanced color eps "Helvetica,18"
set output "../figs/track.eps"

set xlabel "x"
set ylabel "y"
set cblabel "Trials" offset 1

set size square
SCALE = 20.
OFFSETX = 0.0
OFFSETY = 0.0
set xtics 5
set ytics 5
set xrange [-SCALE/2+OFFSETX:SCALE/2+OFFSETX]
set yrange [-SCALE/2+OFFSETY:SCALE/2+OFFSETY]

set key opaque
set key left outside
set key spacing 2
set nokey
ERASEFIRST = 0 ##18
ERASELAST = 5000
col(x) = int(255*x)*65536 + int(68+170*x)*256 + int(255*(1-x))


plot "../agent.dat" u 2:3:1 w p pt 7 ps (10/SCALE)*0.3 lc palette t ""
