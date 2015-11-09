reset
set term postscript enhanced color eps "Helvetica,18" size 4,7
set output "../../figs/lv_signals.eps"

rows = "`cat ../mat/ref_activity.mat | wc -l`"
columns = "`head ../mat/ref_activity.mat -n1 | wc -w`"
rowss = "`cat ../lmr_signals.dat | wc -l`"

set multiplot layout 6, 1
set xrange [-0.5:columns-0.5]
set yrange [-0.5:rows-0.5]
set ytics ("0" 0, "180" 9)
set cbtics 100
set xlabel "Time t"
set ylabel "Sigma_k"
plot "../mat/ref_activity.mat" matrix with image,"../lmr_signals.dat" u ($2*columns/(rowss/10)):(360*$3) w l lt -1 lw 0.5 lc rgb "green" t "", "" u ($2*columns/(rowss/10)):(360*$9) w l lt -1 lw 0.5 lc rgb "blue" t ""
set ylabel "DSigma_k"
plot "../mat/ref_activity.mat" matrix with image,"../lmr_signals.dat" u ($2*columns/(rowss/10)):(360*$4) w l lt -1 lw 0.1 lc rgb "green" t "", "" u ($2*columns/(rowss/10)):(360*$10) w l lt -1 lw 0.1 lc rgb "blue" t ""
set ylabel "Clipped DSigma_k"
plot "../mat/ref_activity.mat" matrix with image,"../lmr_signals.dat" u ($2*columns/(rowss/10)):(360*$5) w l lt -1 lw 0.1 lc rgb "green" t "", "" u ($2*columns/(rowss/10)):(360*$11) w l lt -1 lw 0.1 lc rgb "blue" t ""
#set yrange [-1.05:1.05]
set ylabel "e_k"
plot "../mat/ref_activity.mat" matrix with image,"../lmr_signals.dat" u ($2*columns/(rowss/10)):(360*$6) w l lt -1 lw 0.1 lc rgb "green" t "", "" u ($2*columns/(rowss/10)):(360*$12) w l lt -1 lw 0.1 lc rgb "blue" t ""
set ylabel "Value"
set yrange [-0.1:300]
set ytics autofreq
plot "../lmr_signals.dat" u ($2*columns/(rowss/10)):7 w l lt -1 lw 0.1 lc rgb "green" t "", "" u ($2*columns/(rowss/10)):13 w l lt -1 lw 0.1 lc rgb "blue" t ""
set ylabel "Value raw"
plot "../lmr_signals.dat" u ($2*columns/(rowss/10)):8 w l lt -1 lw 0.1 lc rgb "green" t "", "" u ($2*columns/(rowss/10)):14 w l lt -1 lw 0.1 lc rgb "blue" t ""
unset multiplot
