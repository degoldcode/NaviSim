reset
set term postscript enhanced color eps "Helvetica,18"
set output "../../figs/reward.eps"

plot "../reward.dat" u 2:3 w filledcurve x1 lt -1 lw 0.1 lc rgb "red" t "Reward"
set output
