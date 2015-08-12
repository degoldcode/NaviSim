reset
set term postscript enhanced color eps "Helvetica,18"
set output "../../figs/lscale.eps"

set ylabel "Sum Firing Rate"
set xlabel "Length"

f(x) = m*x
fit f(x) "../l_scale.dat" u ($2/($3*$3)):1 via m

plot "../l_scale.dat" u ($2/($3*$3)):1 w p t '', f(x)
set output
