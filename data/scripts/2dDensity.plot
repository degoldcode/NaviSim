
reset
set term postscript enhanced color eps "Helvetica,12" size 3.5,2.5

set xlabel "x"
set ylabel "y"
set cblabel "p(x,y)"

stat "../agent.dat" u 5 nooutput

set size square
SCALE = 19.5
OFFSETX = 0.0
OFFSETY = 0.0
set xtics 5.
set ytics 5.
set xrange [-SCALE/2+OFFSETX:SCALE/2+OFFSETX]
set yrange [-SCALE/2+OFFSETY:SCALE/2+OFFSETY]
set cbrange [0:0.005]

#set key opaque
set key left outside
set key spacing 2
set nokey
#set grid
#set palette defined ( 0 "white", 1 "yellow", 2 "orange", 3 "red", 4 "black")
#set palette defined (0 "white", 0.01 "blue", 0.02 "green", 0.04 "red")
#set palette defined ( 0 '#000090',\
                      1 '#000fff',\
                      2 '#0090ff',\
                      3 '#0fffee',\
                      4 '#90ff70',\
                      5 '#ffee00',\
                      6 '#ff7000',\
                      7 '#ee0000',\
                      8 '#7f0000')
set palette defined(\
0 '#000000',\
0.03125 0.2667  0.3529  0.8000,\
0.0625  0.3020  0.4078  0.8431,\
0.09375 0.3412  0.4588  0.8824,\
0.125   0.3843  0.5098  0.9176,\
0.15625 0.4235  0.5569  0.9451,\
0.1875  0.4667  0.6039  0.9686,\
0.21875 0.5098  0.6471  0.9843,\
0.25    0.5529  0.6902  0.9961,\
0.28125 0.5961  0.7255  1.0000,\
0.3125  0.6392  0.7608  1.0000,\
0.34375 0.6824  0.7882  0.9922,\
0.375   0.7216  0.8157  0.9765,\
0.40625 0.7608  0.8353  0.9569,\
0.4375  0.8000  0.8510  0.9333,\
0.46875 0.8353  0.8588  0.9020,\
0.5     0.8667  0.8667  0.8667,\
0.53125 0.8980  0.8471  0.8196,\
0.5625  0.9255  0.8275  0.7725,\
0.59375 0.9451  0.8000  0.7255,\
0.625   0.9608  0.7686  0.6784,\
0.65625 0.9686  0.7333  0.6275,\
0.6875  0.9686  0.6941  0.5804,\
0.71875 0.9686  0.6510  0.5294,\
0.75    0.9569  0.6039  0.4824,\
0.78125 0.9451  0.5529  0.4353,\
0.8125  0.9255  0.4980  0.3882,\
0.84375 0.8980  0.4392  0.3451,\
0.875   0.8706  0.3765  0.3020,\
0.90625 0.8353  0.3137  0.2588,\
0.9375  0.7961  0.2431  0.2196,\
0.96875 0.7529  0.1569  0.1843,\
1       0.7059  0.0157  0.1490\
) 
#set cbrange [0:25]
f(x) = 0
set output "../../figs/distri.eps"
set pm3d map
set isosample 400
splot '../agent_histogram.dat' t '', \
	"../goals.dat" u 1:2:(0.0) w p pt 6 ps (29./SCALE) lc rgb "white" t "", "../home.dat" u 1:2:(0.0) w p pt 6 ps (29./SCALE) lc rgb "#666666" t "", "../goals.dat" u 1:(($3 > 0)?$2:1/0):(0.0) w p pt 6 ps (29./SCALE) lc rgb "green" t ""	
#"../landmarks.dat" u 1:2:(0.0) w p pt 6 ps 1.5 lc rgb "blue" t "Landmark",	\
# "" u 1:(($3 > 0)?$2:1/0):(0.0) w p pt 12 ps (5./SCALE) lc rgb "red" t "Visited landmark", 	\
#
