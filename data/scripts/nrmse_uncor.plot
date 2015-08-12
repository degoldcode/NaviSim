reset
set terminal postscript eps enhanced color "Helvetica, 14" size 2.5,2
set grid
#set size square
#set encoding utf8
set style fill   pattern 2 border

set yrange [-0.05:10]

set ylabel "Mean positional estimation error {/Symbol \341}&{j}{/Symbol-Oblique e}_R&{j}{/Symbol \361} in [m]"
PI = 3.1415926
set key above

# color definitions
set border linewidth 1.5
set style line 1 lc rgb '#0060ad' lt 1 lw 0.5 pt 7 # --- blue
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
set style line 13 lc rgb '#09ad00' lt 1 lw 1.5 # --- green
unset grid

#NOISE
set output '../../figs/noise_uncor.eps'		###NOISE
set xrange [-1:21]				###NOISE
set yrange [-0.5:15.5]
set xlabel 'Uncorrelated Noise Level in %'	###NOISE
plot '../nrmse_uncor.dat' u (100*$1):2:3 with errorbars ls 1 pt 7 ps 0.5 t ''
set output

