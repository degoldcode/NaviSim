reset
set terminal postscript enhanced color eps "Helvetica, 12" size 5,1
set offsets graph 0, 0, 0.1, 0.1

rows = "`cat ../mat/pi_activity.mat | wc -l`"
columns = "`head ../mat/pi_activity.mat -n1 | wc -w`"
rowss = "`cat ../agent.dat | wc -l`"

## Ranges
set xrange [-0.5:columns-0.5]
set yrange [-0.5:rows-0.5]
set cbrange [0.:200.]

## Tics
set xtics 500 out
set ytics rows/6
set ytics ("0" 0, "60" 3, "120" 6, "180" 9, "240" 12, "300" 15)
set cbtics 50

## Labels
set xlabel "Time {/Helvetica-Oblique t} [s]"
set ylabel "Angle [{/Symbol \260}]" #"Unit index"
set cblabel "Firing rate [Hz]"

set pm3d map
set key below right
set palette rgb 21,22,23 ## hot
#set pal gray
#set palette negative

##### Reservoir activations
set output "../../figs/pi_activity.eps"
plot "../mat/pi_activity.mat" matrix with image t "", "../homevector.dat" u ($2*columns/(rowss/10)):(rows*$7/360 - 0.5) w l lt -1 lw 1 lc rgb "green" t "Angle of vector average"
#, "../homevector.dat" u ($2*columns/(rowss/10)):(rows*$8/360 - 1.0) w l lt 1 lw 0.1 lc rgb "blue" t "max"
#, "../homevector.dat" u ($2*columns/(rowss/10)):(rows*$8/360 - 1.0) w l lt 1 lw 0.1 lc rgb "blue" t "max", "../agent.dat" u ($8*columns/(rowss/10)):(rows*$7/360 - 0.5) w l lt -1 lw 0.1 lc rgb "violet"  t "real"
set output
