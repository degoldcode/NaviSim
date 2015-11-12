reset
set term postscript enhanced color eps "Helvetica,18"
set output "../../figs/track.eps"

set x2label "x"
set ylabel "y" rotate by 0
set cblabel "Time" offset 1

stat "../agent.dat" u 5 nooutput
print STATS_max

set size square
SCALE = 6##(STATS_max+STATS_max/10.)*2.
OFFSETX = -2.5
OFFSETY = -2.5
	
set xtics 10,1
set xtics format "" nomirror
set x2tics 5 nomirror
set ytics 5 nomirror
set xrange [-SCALE/2+OFFSETX:SCALE/2+OFFSETX]
set x2range [-SCALE/2+OFFSETX:SCALE/2+OFFSETX]
set yrange [-SCALE/2+OFFSETY:SCALE/2+OFFSETY]

#set key opaque
#set key left outside
set noborder
set key spacing 2
set key above right
#set nokey
FIRST = 0 ##18
LAST = 500
col(x) = int(255*x)*65536 + int(68+170*x)*256 + int(255*(1-x))
blacktogreen(x) = int(255-100*x)*65536 + int(255)*256 + int(255-100*x) 
rgb(r,g,b) = int(r)*65536 + int(g)*256 + int(b)


plot "../agent.dat" u 3:(($1<=LAST && $1>=FIRST)?($4):1/0) axes x2y1 w p pt 7 ps 0.3 lc rgb "#222222" t "", "../home.dat" u 1:2:(0.2) axes x2y1 w circles fs transparent border rgb "gray" lt -1 lc rgb "gray" lw 1.5 t "Nest", "" u (-5.):(0.):(0.05) axes x2y1 w circles fs transparent border rgb "red" lt -1 lc rgb "red" lw 1.5 t "", "" u (-5.):(-5.):(0.05) axes x2y1 w circles fs transparent border rgb "red" lt -1 lc rgb "red" lw 1.5 t "", "" u (-0.):(-5.):(0.05) axes x2y1 w circles fs transparent border rgb "red" lt -1 lc rgb "red" lw 1.5 t "", "" u (-0.):(-0.2):(0.05) axes x2y1 w circles fs transparent border rgb "red" lt -1 lc rgb "red" lw 1.5 t ""

#, "../agent.dat" u 3:(($1<=LAST && $1>=FIRST)&&$10>0.7?$4:1/0) w p ps 0.05 pt 7 lc rgb "red" notitle, "../agent.dat" u 3:(($1<=LAST && $1>=FIRST)&&$11>0.7?$4:1/0) w p ps 0.05 pt 7 lc rgb "green" notitle
#, "../agent.dat" u 3:($9>0.?$4:1/0) w p ps 0.2 pt 7 lc rgb "green" notitle
#, "../agent.dat" u 3:($10>0.9?$4:1/0) w p ps 0.05 pt 7 lc rgb "red" notitle
#, "../refvector.dat" u ($3-1):($4+0) w p pt 7 ps 0.05 lc rgb "violet" t ""
#, "../agent.dat" u 3:($10>0.1?$4:1/0) w p ps 0.1 pt 7 lc rgb "red" notitle
