#!/usr/bin/gnuplot
#
# Plot a historgram from a one dimensional data array
# compare: http://stackoverflow.com/questions/2471884/histogram-using-gnuplot
#
# AUTHOR: Hagen Wierstorf
# gnuplot 4.6 patchlevel 0

reset
set macros

# wxt
#set terminal wxt size 350,262 enhanced font 'Verdana,10' persist
# png
#set terminal pngcairo size 350,262 enhanced font 'Verdana,10'
#set output 'histogram2.png'
# eps
set term postscript enhanced color eps "Helvetica,18"
set output "../../figs/histogram.eps"

# colors
set style line 1 lt 1 lc rgb '#FB9A99' # light red
set style line 2 lt 1 lc rgb '#A6CEE3' # light blue

unset key

# axes
set style line 11 lc rgb '#808080' lt 1
set border 3 front ls 11
set tics nomirror out scale 0.75
# grid
set style line 12 lc rgb'#808080' lt 0 lw 1
set grid back ls 12
set grid xtics ytics mxtics


PI=3.14159265359
set xrange [-180:180]
#set yrange [0:500]
set xlabel 'angle [{/Symbol \260}]' offset 0,0.5
set ylabel 'number of occurences' offset 1,0
set xtics 45
set mxtics 2

#set label 'first distribution' at -50,23 center front tc ls 11
#set arrow from -51,21 to -14,14.8 nohead front ls 11
#set label 'second distribution' at 70,13 center front tc ls 11
#set arrow from 70,11 to 44,6.4 nohead front ls 11

# call histogram function
binwidth = 1
binstart = -180
load 'hist.fct'

# store data in a temporary file for fitting
set table '../histogram_dphi.dat'
plot '../agent.dat' @hist ls 1
unset table

# fit Gaussian
Gauss(x) = a/(sigma*sqrt(2*PI)) * exp( -(x-mu)**2 / (2*sigma**2) )
a = 30.
mu = 3.
sigma = 3.
set fit quiet
fit Gauss(x) '../histogram_dphi.dat' u 1:2 via a,mu,sigma


set format x '%g'
plot '../histogram_dphi.dat' using 1:2 w boxes ls 1,\
     Gauss(x) w lines ls 2 lw 2
