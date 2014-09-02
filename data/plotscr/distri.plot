reset
set term postscript enhanced color eps "Helvetica,12"
#set output "../agentdist.eps"

set xlabel "x"
set ylabel "y"
set cblabel "p(x,y)"
set size square
SCALE = 25.
OFFSETX = 0.0
OFFSETY = 0.0
#set xtics SCALE/8.
#set ytics SCALE/8.
set xtics 5.
set ytics 5.
set xrange [-SCALE/2+OFFSETX:SCALE/2+OFFSETX]
set yrange [-SCALE/2+OFFSETY:SCALE/2+OFFSETY]
#set xrange [15:35]
#set yrange [15:35]
#set key opaque
set key left outside
set key spacing 2
set nokey
#set grid
set palette defined ( 0 "white", 1 "yellow", 2 "orange", 3 "red", 4 "black")
#set cbrange [0:25]
f(x) = 0
set output "../multidist.eps"
set multiplot layout 1,2
set lmargin screen 0.1 ; set rmargin screen 0.425
unset colorbox
set pm3d map interpolate 0,0 corners2color c1
set title "Before learning (exploration > 0.5)"
splot '< python distri2.py' nonuniform matrix t '', \
	 "../landmarks.dat" u 1:2:(0.0) w p pt 12 ps (5./SCALE) lc rgb "black" t "Landmark",	\
	 "" u 1:(($3 > 0)?$2:1/0):(0.0) w p pt 12 ps (5./SCALE) lc rgb "red" t "Visited landmark", 	\
	"../goals.dat" u 1:2:(0.0) w p pt 6 ps (29./SCALE) lc rgb "black" t "Goal", 	\
	"" u 1:(($3 > 0)?$2:1/0):(0.0) w p pt 7 ps (29./SCALE) lc rgb "green" t "Visited goal" , \
	"../home.dat" u 1:2:(0.0) w p pt 4 ps (29./SCALE) lc rgb "#bbbbbb" t "Home"
set lmargin screen 0.525 ; set rmargin screen 0.85
set colorbox user origin 0.9,0.2 size 0.025,0.6
set title "After learning (exploration < 0.5)"
splot '< python distri.py' nonuniform matrix t '', \
	 "../landmarks.dat" u 1:2:(0.0) w p pt 12 ps (5./SCALE) lc rgb "black" t "",	\
	 "" u 1:(($3 > 0)?$2:1/0):(0.0) w p pt 12 ps (5./SCALE) lc rgb "red" t "", 	\
	"../goals.dat" u 1:2:(0.0) w p pt 6 ps (29./SCALE) lc rgb "black" t "", 	\
	"" u 1:(($3 > 0)?$2:1/0):(0.0) w p pt 7 ps (29./SCALE) lc rgb "green" t "" , \
	"../home.dat" u 1:2:(0.0) w p pt 4 ps (29./SCALE) lc rgb "#bbbbbb" t ""

unset multiplot
set output

