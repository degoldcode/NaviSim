reset
set terminal postscript eps enhanced color "Helvetica, 28" size 5,4
set grid
#set size square
#set encoding utf8
set style fill   pattern 2 border

set ylabel "Mean position error {/Symbol \341}&{j}{/Symbol-Oblique e}_R&{j}{/Symbol \361} in [m]"
PI = 3.1415926
set key above

# color definitions
set border linewidth 1.5
set style line 1 lc rgb '#0060ad' lt 1 lw 2.5 pt 7 # --- blue
set style line 2  lc rgb '#0025ad' lt 1 lw 1.5 # --- blue
set style line 3  lc rgb '#0042ad' lt 1 lw 1.5 #      .
set style line 4  lc rgb '#0060ad' lt 1 lw 1.5 #      .
set style line 5  lc rgb '#007cad' lt 1 lw 1.5 #      .
set style line 6  lc rgb '#0099ad' lt 1 lw 1.5 #      .
set style line 7  lc rgb '#00ada4' lt 1 lw 1.5 #      .
set style line 8  lc rgb '#00ad88' lt 1 lw 1.5 #      .
set style line 9  lc rgb '#00ad6b' lt 1 lw 1.5 #      .
set style line 10 lc rgb '#00ad4e' lt 1 lw 1.5 #      .
set style line 11 lc rgb '#00ad31' lt 1 lw 1.5 #      .
set style line 12 lc rgb '#00ad14' lt 1 lw 1.5 #      .
set style line 13 lc rgb '#09ad00' lt 1 lw 3.5 # --- green
unset grid

f(x) = a*exp(-b*x)+c
a=350 
b=0.2 
c=0.1
set fit quiet
fit f(x) "../nrmse_neuron.dat" u 1:2 via a,b,c



#NEURONS
#unset grid
set output '../../figs/nrmse_neurons.eps'	###NEURONS
set xlabel "Number of Neurons (log. scale)"			###NEURONS
set yrange [-0.:0.75]
set xrange [3:1000]
set x2range [3:1000]				###NEURONS
set xtics (3, 6, 9, 18, 36, 90, 180, 360)
set grid x2tics lw 4
#set x2tics format "" scale 0 
set x2tics (16,32)
set ytics 0.1

set style rect fc rgb "#aaffaa" fs solid 0.15 noborder
set obj rect from 16, graph 0 to 32, graph 1 behind

set logscale x	
set logscale x2	
plot "../nrmse_neuron.dat" u 1:2:3 with errorbars ls 1 pt 2 ps 1.5 t '', f(x) w l ls 13 t "decay exponential fit f(x)"
set output
