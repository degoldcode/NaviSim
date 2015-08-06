reset
set term postscript enhanced color eps "Helvetica,18"
set output "../../figs/track.eps"

set xlabel "x"
set ylabel "y"
set cblabel "Trials" offset 1

set size square
SCALE = 30.
OFFSETX = 0.0 ##-2.5
OFFSETY = 0.0 ##-2.5
	
set xtics 10
set ytics 10
set xrange [-SCALE/2+OFFSETX:SCALE/2+OFFSETX]
set yrange [-SCALE/2+OFFSETY:SCALE/2+OFFSETY]

set key opaque
set key left outside
set key spacing 2
set nokey
ERASEFIRST = 0 ##18
ERASELAST = 5000
col(x) = int(255*x)*65536 + int(68+170*x)*256 + int(255*(1-x))
blacktogreen(x) = int(255-100*x)*65536 + int(255)*256 + int(255-100*x) 
rgb(r,g,b) = int(r)*65536 + int(g)*256 + int(b)


plot "../agent.dat" u 3:4:2 w p pt 7 ps (10/SCALE)*0.3 lc palette t "", "../homevector.dat" u 3:4 w p pt 7 ps (10/SCALE)*0.3 lc rgb "blue" t "", "../goals.dat" u 1:2:(0.2):(blacktogreen($3)) w circles fs solid border rgb "black" lt -1 lw 0.2 lc rgb var t "Goal", "../home.dat" u 1:2:(0.2) w circles fs solid border rgb "black" lt -1 lw 0.2 lc rgb "gray" t "Goal"
