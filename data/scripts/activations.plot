reset
set terminal postscript enhanced color eps "Helvetica, 12" size 5,1
set offsets graph 0, 0, 0.1, 0.1

rows = "`cat ../mat/pi_activity.mat | wc -l`"
columns = "`head ../mat/pi_activity.mat -n1 | wc -w`"

## Ranges
set xrange [-0.5:columns+0.5]
set yrange [-0.5:rows+0.5]
#set cbrange [0.:300.]

## Tics
set xtics autofreq
set ytics rows/6
set cbtics 1000

## Labels
set xlabel "Time {/Helvetica-Oblique t} [s]"
set ylabel "Unit index"
set cblabel "State"

set pm3d map
unset key
#set palette rgb 21,22,23 ## hot
set pal gray
set palette negative

##### Reservoir activations
set output "../../figs/pi_activity.eps"
plot "../mat/pi_activity.mat" matrix with image, "../homevector.dat" u 2:5 w l lt -1 lc rgb "green", "../homevector.dat" u 2:6 w l lt 1 lc rgb "blue", "../agent.dat" u 2:7 w l lt -1 lc rgb "red"
set output
