reset
set terminal postscript enhanced color eps "Helvetica, 12" size 5,1
set offsets graph 0, 0, 0.1, 0.1

rows = "`cat ../mat/lv_activity_0.mat | wc -l`"
columns = "`head ../mat/lv_activity_0.mat -n1 | wc -w`"
rowss = "`cat ../localvector.dat | wc -l`"
stat "../localvector.dat" u 2 nooutput
#print rows
#print columns
#print rowss
#print STATS_max


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
g(x)=225./20.
f(x)=180./20.

FACTOR = (1.*columns/(1.*STATS_max))

##### Reservoir activations
set output "../../figs/lv_activity_0.eps"
plot "../mat/lv_activity_0.mat" matrix with image, g(x) w l lt -1 lc rgb "red" t "correct", "../localvector.dat" u ($2*FACTOR):(rows*$11/360) w l lt -1 lc rgb "cyan" t "vec avg"
#, "../localvector.dat" u ($2*columns/(rowss/10)):(rows*$11/360) w l lt -1 lc rgb "green" t "avg",
#, "../homevector.dat" u ($2*columns/(rowss/10)):(rows*$7/360) w l lt -1 lc rgb "green" t "avg", "../homevector.dat" u ($2*columns/(rowss/10)):(rows*$8/360) w l lt 1 lc rgb "blue" t "max", "../agent.dat" u ($8*columns/(rowss/10)):(rows*$7/360) w l lt -1 lc rgb "violet"  t "real"
set output
set output "../../figs/lv_activity_1.eps"
plot "../mat/lv_activity_1.mat" matrix with image, f(x) w l lt -1 lc rgb "red" t "correct", "../localvector.dat" u ($2*FACTOR):(rows*$12/360) w l lt -1 lc rgb "cyan" t "vec avg"
#, "../localvector.dat" u ($2*columns/(rowss/10)):(rows*$5/360) w l lt -1 lc rgb "green" t "avg"
set output
