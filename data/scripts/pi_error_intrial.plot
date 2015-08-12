reset
set term postscript enhanced color eps "Helvetica,18"
set output "../../figs/pi_error_intrial.eps"

# fit length
f(x) = L*sqrt(x)
set fit quiet
fit f(x) '../homevector.dat' u 1:9 via L
set yrange [0:0.5]

set xlabel "Time in [s]"
set ylabel "Mean positional estimation error {/Symbol \341}&{j}{/Symbol-Oblique e}_R&{j}{/Symbol \361} in [m]"

#set label 1 sprintf("f(x) = m*x^{1/2}, m = %3.4f", L) at 200,0.4

plot "../homevector.dat" every 100 u 1:9 w p ps 0.1 pt 7 lc rgb "gray" t "each trial", '' using 1:9 smooth unique lt -1 lc rgb "red" t "smoothed", f(x) lt 1 lc rgb "green" t "squareroot fit f(x)"
set output
