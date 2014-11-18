reset
set term x11 persist enhanced font "Helvetica,8"
set output "../figs/track.eps"
columns = "`head ../agent.dat -n1 | wc -w`"

load 'moreland.pal'

set xlabel "x"
set ylabel "y"
set cblabel "Time t" offset 1

set size square
SCALE = 10.
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

plot for [cole=2:columns:2] '../agent.dat' \
	u cole:cole+1:1 \
	w p pt 7 ps 0.6 lc palette t ""
set output
