reset
set term postscript enhanced color eps "Helvetica,12" size 8,2
set output "../dist.eps"

set xlabel "t"
set ylabel "Ampl"

set yrange [0:10]

set grid

#plot "../distor.dat" u 0:($3==0?1/0:($2/$3)) w l t "GV distortion" 
#, "" u 0:3 w l lt 1 lc rgb "blue" t "GV length" 
#, "" u 0:2 w l lt 1 lc rgb "green" t "PI length", "" u 0:3 w l lt 1 lc rgb "blue" t "GV length" 
set output
