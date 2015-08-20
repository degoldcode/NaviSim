reset
set terminal postscript enhanced color eps "Helvetica, 12" size 5,1
set offsets graph 0, 0, 0.1, 0.1

rows = "`cat ../mat/gv_activity.mat | wc -l`"
columns = "`head ../mat/gv_activity.mat -n1 | wc -w`"
rowss = "`cat ../agent.dat | wc -l`"

## Ranges
set xrange [-0.5:columns-0.5]
set yrange [-0.5:rows-0.5]
#set cbrange [0.:300.]

## Tics
set xtics autofreq
set ytics rows/6
set ytics ("0" 0, "60" 3, "120" 6, "180" 9, "240" 12, "300" 15)
set cbtics autofreq

## Labels
set xlabel "Time {/Helvetica-Oblique t} [s]"
set ylabel "Unit index"
set ylabel "Angle [{/Symbol \260}]" #"Unit index"
set cblabel "Synaptic strength"

set pm3d map
unset key
set palette rgb 21,22,23 ## hot
#set pal gray
#set palette negative

PI = 3.14159
f(x)=180./20.

##### Reservoir activations
set output "../../figs/gv_activity.eps"
plot "../mat/gv_activity.mat" matrix with image, f(x) w l lt -1 lc rgb "red" t "correct", "../globalvector.dat" u ($2*columns/(rowss/10)):(rows*$5/360) w l lt -1 lc rgb "green" t "avg"
#, "../homevector.dat" u ($2*columns/(rowss/10)):(rows*$7/360) w l lt -1 lc rgb "green" t "avg", "../homevector.dat" u ($2*columns/(rowss/10)):(rows*$8/360) w l lt 1 lc rgb "blue" t "max", "../agent.dat" u ($8*columns/(rowss/10)):(rows*$7/360) w l lt -1 lc rgb "violet"  t "real"
set output
