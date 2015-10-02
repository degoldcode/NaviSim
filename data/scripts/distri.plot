
reset
set term postscript enhanced color eps "Helvetica,12"

set xlabel "x"
set ylabel "y"
set cblabel "p(x,y)"

stat "../agent.dat" u 5 nooutput

set size square
SCALE = 5.
OFFSETX = 0.0
OFFSETY = 0.0
set xtics 5.
set ytics 5.
set xrange [-SCALE/2+OFFSETX:SCALE/2+OFFSETX]
set yrange [-SCALE/2+OFFSETY:SCALE/2+OFFSETY]

#set key opaque
set key left outside
set key spacing 2
set nokey
#set grid
set palette defined ( 0 "white", 1 "yellow", 2 "orange", 3 "red", 4 "black")
#set cbrange [0:25]
f(x) = 0
set output "../../figs/distri.eps"
set pm3d map interpolate 0,0 corners2color c1
splot '< python distri2.py' nonuniform matrix t '', \
	 "../landmarks.dat" u 1:2:(0.0) w p pt 6 ps 1.5 lc rgb "blue" t "Landmark",	\
	 "" u 1:(($3 > 0)?$2:1/0):(0.0) w p pt 12 ps (5./SCALE) lc rgb "red" t "Visited landmark", 	\
	"../goals.dat" u 1:2:(0.0) w p pt 6 ps (29./SCALE) lc rgb "black" t "Goal", "../home.dat" u 1:2:(0.0) w p pt 6 ps (29./SCALE) lc rgb "#bbbbbb" t "Home"	

#"" u 1:(($3 > 0)?$2:1/0):(0.0) w p pt 7 ps (29./SCALE) lc rgb "green" t "Visited goal", \
