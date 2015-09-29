reset
set term postscript enhanced color eps "Helvetica,18"
set output "../../figs/lv_signals.eps"

set multiplot layout 4, 1
set yrange [-0.05:1.05]
set ytics 0.5
set xlabel "Time t"
set ylabel "Sigma_k"
plot "../lmr_signals.dat" u 2:3 w l lt -1 lw 0.1 lc rgb "blue" t "LV0", "" u 2:7 w l lt -1 lw 0.1 lc rgb "green" t "LV1"
set ylabel "DSigma_k"
plot "../lmr_signals.dat" u 2:4 w l lt -1 lw 0.1 lc rgb "blue" t "LV0", "" u 2:8 w l lt -1 lw 0.1 lc rgb "green" t "LV1"
set ylabel "Clipped DSigma_k"
plot "../lmr_signals.dat" u 2:5 w l lt -1 lw 0.1 lc rgb "blue" t "LV0", "" u 2:9 w l lt -1 lw 0.1 lc rgb "green" t "LV1"
#set yrange [-1.05:1.05]
set ylabel "e_k"
plot "../lmr_signals.dat" u 2:6 w l lt -1 lw 0.1 lc rgb "blue" t "LV0", "" u 2:10 w l lt -1 lw 0.1 lc rgb "green" t "LV1"
unset multiplot
