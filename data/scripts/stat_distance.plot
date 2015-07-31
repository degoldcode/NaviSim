reset
set term postscript enhanced color eps "Helvetica,18"
set output "../../figs/statl.eps"

plot "../agent.dat" u 2:5 w p ps 0.1 pt 7 lc rgb "gray", '' using 2:5 smooth unique lt -1 lc rgb "red"
set output
