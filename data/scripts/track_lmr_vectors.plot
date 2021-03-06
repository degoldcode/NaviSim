reset
set term postscript enhanced color eps "Helvetica,18"
set output "../../figs/track_lmr_vecs.eps"

set xlabel "x"
set ylabel "y"
set cblabel "Trials" offset 1

stat "../agent.dat" u 5 nooutput
print STATS_max

set size square
SCALE = 3.2##(STATS_max+STATS_max/20.)*2.
OFFSETX = -1 ##-2.5
OFFSETY = 0 ##-2.5
	
set xtics 1
set ytics 1
set xrange [-SCALE/2+OFFSETX:SCALE/2+OFFSETX]
set yrange [-SCALE/2+OFFSETY:SCALE/2+OFFSETY]

#set key opaque
#set key left outside
set key spacing 2
#set nokey
FIRST = 1 ##18
LAST = 100
col(x) = int(255*x)*65536 + int(68+170*x)*256 + int(255*(1-x))
blacktogreen(x) = int(255-100*x)*65536 + int(255)*256 + int(255-100*x) 
rgb(r,g,b) = int(r)*65536 + int(g)*256 + int(b)

#set palette maxcolors 6
#set palette defined (1 "#e74c3c", 2 "#f1c40f", 3 "#2ecc71", 4 "#3498db", 5 "#9b59b6", 6 "#34495e")
set cbtics ("1" FIRST, "2" FIRST+1, "3" FIRST+2,"4" FIRST+3,"5" FIRST+4, "6" FIRST+5)
set cbrange [FIRST-0.5:LAST+0.5]
#FFDB58
#rgb "#cccccc"
plot  "../agent.dat" u 3:(($1<=LAST && $1>=FIRST)?($4):1/0):1 w p pt 7 ps 0.1 lc palette t "", "" every 50 u 3:4:(0.1*cos($6)):(0.1*sin($6)) w vectors lt -1 lc rgb "#6698FF" t '', "../goals.dat" u 1:2:(0.2) w circles fs transparent border rgb "#32cd32" lw 0.2 lt -1 lc rgb "#ffff00" t "", "../landmarks.dat" u 1:2:(0.1) w circles fs transparent border rgb "red" lw 0.1 lt -1 lc rgb "red" t "", "../landmarks.dat" u 1:2:(0.6) w circles fs transparent border rgb "blue" lw 0.1 lt 2 lc rgb "blue" t "", "../home.dat" u 1:2:(0.2) w circles fs transparent border rgb "#444444" lt -1 lc rgb "#444444" lw 0.2 t ""
#plot "../agent.dat" u 3:(($1<=LAST && $1>=FIRST)?($4):1/0):1 w p pt 7 ps 0.1 lc rgb "#eeeeee" t "", "../goals.dat" u 1:2:(0.2) w circles fs transparent border rgb "#32cd32" lw 0.2 lt -1 lc rgb "#32cd32" t "Goal", "../landmarks.dat" u 1:2:(0.1) w circles fs transparent border rgb "red" lw 0.1 lt -1 lc rgb "red" t "Landmark", "../landmarks.dat" u 1:2:(0.5) w circles fs transparent border rgb "blue" lw 0.1 lt 2 lc rgb "blue" t "Landmark", "../home.dat" u 1:2:(0.2) w circles fs transparent border rgb "#444444" lt -1 lc rgb "#444444" lw 0.2 t "Home", "../agent.dat" u 3:(($1<=LAST && $1>=FIRST)&&$10>0.1?$4:1/0) w p ps 0.05 pt 7 lc rgb "violet" notitle, "../agent.dat" u 3:(($1<=LAST && $1>=FIRST)&&$11>0.1?$4:1/0) w p ps 0.05 pt 7 lc rgb "green" notitle
#, "../landmarks.dat" u 1:2:(0.1) w circles fs transparent border rgb "red" lw 1.5 lt -1 lc rgb "red" t "Landmark"
#, "../agent.dat" u 3:(($1<=LAST && $1>=FIRST)&&$10>0.7?$4:1/0) w p ps 0.05 pt 7 lc rgb "red" notitle, "../agent.dat" u 3:(($1<=LAST && $1>=FIRST)&&$11>0.7?$4:1/0) w p ps 0.05 pt 7 lc rgb "green" notitle
#, "../agent.dat" u 3:($9>0.?$4:1/0) w p ps 0.2 pt 7 lc rgb "green" notitle
#, "../agent.dat" u 3:($10>0.9?$4:1/0) w p ps 0.05 pt 7 lc rgb "red" notitle
#, "../refvector.dat" u ($3-1):($4+0) w p pt 7 ps 0.05 lc rgb "violet" t ""
#, "../agent.dat" u 3:($10>0.1?$4:1/0) w p ps 0.1 pt 7 lc rgb "red" notitle
