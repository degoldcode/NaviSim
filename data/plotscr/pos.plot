reset
set term postscript enhanced color eps "Helvetica,12"
set output "../agent.eps"

set xlabel "x"
set ylabel "y"
set cblabel "Time t [s]" offset 1

set size square
SCALE = 50.
OFFSETX = 0.0
OFFSETY = 0.0
set xtics SCALE/8.
set ytics SCALE/8.
set xrange [-SCALE/2+OFFSETX:SCALE/2+OFFSETX]
set yrange [-SCALE/2+OFFSETY:SCALE/2+OFFSETY]
#set xrange [15:35]
#set yrange [15:35]
set key opaque
set key left outside
set key spacing 2
#set nokey
#set grid
set xtics 5
set ytics 5
ERASEFIRST = 0 ##18
ERASELAST = 20000
#set palette defined (0 "black", 1 "red")

plot	 "../goals.dat" u 1:2:(0.2) w circles fs solid noborder lc rgb "black" t "Goal", 					\
	 "../landmarks.dat" u 1:2 w p pt 12 ps (5./SCALE) lc rgb "black" t "Landmark",				\
	 "../pipes.dat" u 1:2 w l lw (182./SCALE) lt 1 lc rgb "black" t "", 					\
         "../pipes.dat" u 1:2 w l lw (172./SCALE) lt 1 lc rgb "#aaaaaa" t "Channel", 				\
	 "../agent.dat" u 2:(($9>ERASEFIRST && $9<ERASELAST)?$3:1/0):9 w p pt 7 ps (10/SCALE)*0.3 lc palette t "" ,		\
	 "../landmarks.dat" u 1:(($3 > 0)?$2:1/0) w p pt 12 ps (5./SCALE) lc rgb "red" t "Visited landmark", 	\
	 "../home.dat" u 1:2:(0.2) w circles fs solid noborder lc rgb "#FFB546" t "Home"
	#"../control.dat" u 8:($0>ERASEFIRST && $0<ERASELAST?$9:1/0) w p pt 7 ps (10/SCALE)*0.3 lc rgb "green" t "",	\
	 #"../goals.dat" u 1:(($3 > 0)?$2:1/0) w p pt 6 lw (4./SCALE) ps (29./SCALE) lc rgb "green" t "Visited goal" 
	 #"../lm_rec.dat" u 2:3:((1./SCALE)*$5) w p pt 1 ps var lc rgb "blue" t "Recognized landmark" , \ 
	 #"../control.dat" u 7:($0>ERASEFIRST && $0<ERASELAST?$8:1/0) w p pt 7 ps (10/SCALE)*0.3 lc rgb "red" t ""	
set output
set cblabel "Trials" offset 1
ERASEFIRST = 0 ##18
ERASELAST = 100
set output "../agent_learntrials.eps"

plot	 "../goals.dat" u 1:2:(0.2) w circles fs solid noborder lc rgb "black" t "Goal", 					\
	 "../landmarks.dat" u 1:2 w p pt 12 ps (5./SCALE) lc rgb "black" t "Landmark",				\
	 "../pipes.dat" u 1:2 w l lw (182./SCALE) lt 1 lc rgb "black" t "", 					\
         "../pipes.dat" u 1:2 w l lw (172./SCALE) lt 1 lc rgb "#aaaaaa" t "Channel", 				\
	 "../agent.dat" u 2:($9>ERASEFIRST?$3:1/0):8 w p pt 7 ps (10/SCALE)*0.3 lc palette t "" ,		\
	 "../landmarks.dat" u 1:(($3 > 0)?$2:1/0) w p pt 12 ps (5./SCALE) lc rgb "red" t "Visited landmark", 	\
	 "../home.dat" u 1:2 w p pt 6 ps (29./SCALE) lc rgb "#bbbbbb" t "Home"
	#"../control.dat" u 8:($0>ERASEFIRST && $0<ERASELAST?$9:1/0) w p pt 7 ps (10/SCALE)*0.3 lc rgb "green" t "",	\
	 #"../goals.dat" u 1:(($3 > 0)?$2:1/0) w p pt 6 lw (4./SCALE) ps (29./SCALE) lc rgb "green" t "Visited goal" 
	 #"../lm_rec.dat" u 2:3:((1./SCALE)*$5) w p pt 1 ps var lc rgb "blue" t "Recognized landmark" , \ 
	 #"../control.dat" u 7:($0>ERASEFIRST && $0<ERASELAST?$8:1/0) w p pt 7 ps (10/SCALE)*0.3 lc rgb "red" t ""	
#plot "agent.dat" u 1:2:0 w l lc palette t ""
#, "../reward.dat" u 1:2 w p pt 7 ps 0.3 lc rgb "red" t "" 
#l lc palette t ""
#pt 7 ps 0.2 t ""
set output

set cblabel "Time t [s]" offset 1
set output "../goal_learning.eps"
plot	"../goals.dat" u 1:2 w p pt 6 ps (29./SCALE) lc rgb "black" t "Goal", 					\
	"../lm_rec.dat" u 2:3:((5./SCALE)*$5) w p pt 1 lw (4./SCALE) ps var lc rgb "blue" t "Recognized landmark" , \
	"../landmarks.dat" u 1:2 w p pt 12 ps (5./SCALE) lc rgb "black" t "Landmark", 					\
	#"../pipes.dat" u 1:2 w l lw (182./SCALE) lt 1 lc rgb "black" t "", 					\
	#"../pipes.dat" u 1:2 w l lw (182./SCALE) lt 1 lc rgb "#aaaaaa" t "Pipe", 				\
	#"../landmarks.dat" u 1:(($3 > 0)?$2:1/0) w p pt 12 ps (5./SCALE) lc rgb "red" t "Visited landmark", 			\
	"../reward.dat" u 3:4:0 w p pt 7 ps 0.6 lc palette t "",					\
	"../home.dat" u 1:2 w p pt 4 ps (29./SCALE) lc rgb "#bbbbbb" t "Home" 				
	#"../goals.dat" u 1:(($3 > 0)?$2:1/0) w p pt 6 lw (4./SCALE) ps (29./SCALE) lc rgb "green" t "Visited goal" 	
set output

set output "../route_learning.eps"
plot	"../goals.dat" u 1:2 w p pt 6 ps (29./SCALE) lc rgb "black" t "Goal", 					\
	"../lm_rec.dat" u 2:3:((5./SCALE)*$5) w p pt 1 lw (4./SCALE) ps var lc rgb "blue" t "Recognized landmark" , \
	"../landmarks.dat" u 1:2 w p pt 12 ps (5./SCALE) lc rgb "black" t "Landmark", 					\
	#"../pipes.dat" u 1:2 w l lw (182./SCALE) lt 1 lc rgb "black" t "", 					\
	#"../pipes.dat" u 1:2 w l lw (182./SCALE) lt 1 lc rgb "#aaaaaa" t "Pipe", 				\
	#"../landmarks.dat" u 1:(($3 > 0)?$2:1/0) w p pt 12 ps (5./SCALE) lc rgb "red" t "Visited landmark", 			\
	"../reward.dat" u ($5+0.5):($6+1.):0 w p pt 7 ps 0.6 lc palette t "",					\
	"../home.dat" u 1:2 w p pt 4 ps (29./SCALE) lc rgb "#bbbbbb" t "Home"
	#"../goals.dat" u 1:(($3 > 0)?$2:1/0) w p pt 6 lw (4./SCALE) ps (29./SCALE) lc rgb "green" t "Visited goal" 	
set output
