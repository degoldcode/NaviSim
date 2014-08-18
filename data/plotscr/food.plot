reset
set term postscript enhanced color eps "Helvetica,12" size 5,1
set output "../food.eps"

set xlabel "Time {/Helvetica-Oblique t} [s]"
set ylabel "Rate"
#set xrange [-50:6050]
set yrange [-0.1:1.1]
set ytics 0.5 nomirror
set xtics autofreq nomirror
unset key
set grid

plot "../food.dat" u 1:9 w l lc rgb "#0000FF" t "Amount of food 10", \
	"" u 1:26 w l lc rgb "#00FF00" t "Amount of food 8", \
	"" u 1:40 w l lc rgb "#FF0000" t "Amount of food 9", \
	"" u 1:41 w l lc rgb "#00FFFF" t "Amount of food 10", \
	"" u 1:38 w l lc rgb "#FF00FF" t "Amount of food 11", \
	"" u 1:32 w l lc rgb "#FFFF00" t "Amount of food 12"
	#"" u 1:41 w l lc rgb "#002233" t "Amount of food 13"
	#"" u 1:15 w l lc rgb "#002222" t "Amount of food 14", \
	#"" u 1:16 w l lc rgb "#002211" t "Amount of food 15", \
	#"" u 1:17 w l lc rgb "#002200" t "Amount of food 16", \
	#"" u 1:18 w l lc rgb "#ff22FF" t "Amount of food 17", \
	#"" u 1:19 w l lc rgb "#dd22FF" t "Amount of food 18", \
	
set output
