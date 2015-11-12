reset
set term postscript enhanced color eps "Helvetica,18" size 7.5,3.5b
set output "../../figs/fig9.eps"

set xlabel "x"
set ylabel "y"
set cblabel "Trials" offset 0,4

stat "../agent.dat" u 5 nooutput
print STATS_max

set size square
SCALE = 28 ##(STATS_max+STATS_max/20.)*2.
OFFSETX = 0 ##-2.5
OFFSETY = 1 ##-2.5
	
set xtics 10
set ytics 10
set xrange [-SCALE/2+OFFSETX:SCALE/2+OFFSETX]
set yrange [-SCALE/2+OFFSETY:SCALE/2+OFFSETY-1]

# axes
set style line 11 lc rgb '#808080' lt 1
set border 3 front ls 11
set tics nomirror out scale 0.75
set noborder
unset tics
unset xlabel
unset ylabel

FIRST = 8 ##
LAST = 12
col(x) = int(255*x)*65536 + int(68+170*x)*256 + int(255*(1-x))
blacktogreen(x) = int(255-100*x)*65536 + int(255)*256 + int(255-100*x) 
rgb(r,g,b) = int(r)*65536 + int(g)*256 + int(b)

set colorbox horiz user origin 0.25,0.08 size 0.5,0.02
set palette maxcolors 5
set palette defined (1 "#e74c3c", 2 "#f1c40f", 3 "#2ecc71", 4 "#3498db", 5 "#9b59b6")
#, 6 "#34495e")
set cbtics ("1" FIRST, "2" FIRST+1, "3" FIRST+2,"4" FIRST+3,"5" FIRST+4)
#, "6" FIRST+5)
set cbrange [FIRST-0.5:LAST+0.5]


#### CALCULATE MARGINS FOR MULTIPLOT
set tmargin 1
init_margins(left, right, gap, cols) = \
  sprintf('left_margin = %f; right_margin = %f;', left, right) . \
  sprintf('col_count = %d; gap_size = %f;', cols, gap)

set_margins(col) = sprintf('set lmargin at screen %f;', get_lmargin(col)) . \
  sprintf('set rmargin at screen %f;', get_rmargin(col));

get_lmargin(col) = (left_margin + (col - 1) * (gap_size + ((right_margin - left_margin)-(col_count - 1) * gap_size)/col_count))

get_rmargin(col) = (left_margin + (col - 1) * gap_size + col * ((right_margin - left_margin)-(col_count - 1) * gap_size)/col_count)


set multiplot layout 1,2

#### TOP ROW
eval(init_margins(-0.05, 1.05, -0., 2))
### TOP LEFT
eval(set_margins(1))
## LABELS
set label "Feeder" at -3.8,0.5 font "Helvetica, 28"
set label "Nest" at 0.6,-0.1 font "Helvetica, 28"
set style rect fc rgb "#7f8c8d" fs solid 1 noborder
set obj rect from 3, -3 to 4, -2.9
set label "1m" at 3.15, -2.65 font "Helvetica, 28"  

plot "../agent.dat" u 3:(($1<=LAST && $1>=FIRST)?($4):1/0):1 w p pt 7 ps 0.1 lc palette t "", "../goals.dat" u 1:2:(0.2) w circles fs transparent border rgb "#32cd32" lw 1.5 lt -1 lc rgb "#32cd32" t "", "../home.dat" u 1:2:(0.2) w circles fs transparent border rgb "#444444" lt -1 lc rgb "#444444" lw 1.5 t ""



### TOP RIGHT
unset label
unset obj 
unset colorbox

## LABELS
set label "Feeder" at -3.8,10.7 font "Helvetica, 16"
set label "Nest" at 1.2,-0.1 font "Helvetica, 16"
set style rect fc rgb "#7f8c8d" fs solid 1 noborder
set obj rect from -8, 11.5 to -6, 11.75
set label "2m" at -7.8, 12.4 font "Helvetica, 16"  

eval(set_margins(2))
plot "../agent.dat" u 3:(($1<=LAST && $1>=FIRST)?($4):1/0):1 w p pt 7 ps 0.1 lc palette t "", "../goals.dat" u 1:2:(0.2) w circles fs transparent border rgb "#32cd32" lw 1.5 lt -1 lc rgb "#32cd32" t "", "../home.dat" u 1:2:(0.2) w circles fs transparent border rgb "#444444" lt -1 lc rgb "#444444" lw 1.5 t ""

unset multiplot
