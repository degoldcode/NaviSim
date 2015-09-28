reset
set term postscript enhanced color eps "Helvetica,18"
set output "../../figs/lv_signals.eps"

set multiplot layout 3, 1
plot "../reward.dat" u 2:3 w l lt -1 lw 0.1 lc rgb "red" t "Reward"
set output
