reset
set term postscript enhanced color eps "Helvetica,12" size 8,2
set output "../lm_rec.eps"

set xlabel "t"
set ylabel "Ampl"
set yrange [-0.1:1.1]
set grid

plot "../control.dat" u 1:17 w l lt 1 lc rgb "#ff6666" t "LM rec", "" u 1:18 w l lt 1 lc rgb "#6666ff" t "LM lowpass",  "" u 1:19 w l lt 1 lc rgb "#66ff66" t "reward"
set output
