reset
set term postscript enhanced color eps "Helvetica,18"
set output "../../figs/track.eps"

set xlabel "x"
set ylabel "y"
set cblabel "Trials" offset 1

stat "../agent.dat" u 5 nooutput
print STATS_max

set size square
SCALE = (STATS_max+STATS_max/5.)*2.
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


plot "../agent.dat" u 3:4:1 w p pt 7 ps (10/SCALE)*0.3 lc palette t "", "../goals.dat" u 1:2:(0.2):(blacktogreen($3)) w circles fs solid border rgb "black" lt -1 lw 0.2 lc rgb var t "Goal", "../landmarks.dat" u 1:2:(0.2) w circles fs solid border rgb "brown" lt 1 lw 0.1 t "Landmark", "../home.dat" u 1:2:(0.2) w circles fs solid border rgb "black" lt -1 lw 0.2 lc rgb "gray" t "Goal", "../agent.dat" u 3:($9>0.?$4:1/0) w p ps 0.2 pt 7 lc rgb "green" notitle
#, "../agent.dat" u 3:($10>0.1?$4:1/0) w p ps 0.1 pt 7 lc rgb "red" notitle
