reset
set terminal postscript eps enhanced color "Helvetica, 14" size 2.5,2
set grid
#set size square
set encoding utf8
set style fill   pattern 2 border

set yrange [-0.05:10]

set ylabel "Mean positional estimation error {\74}&{j}{/Symbol-Oblique e}&{j}{\76} in [m]"
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
#TURN
set output 'turn_new.eps'		###TURN
set xrange [0:100]		###TURN
set yrange [-15:40]
set xlabel 'Average Turning Rate d{/Symbol-Oblique f}/d{/Helvetica-Italic t}' ###TURN
#plot 'nrmse_turn.dat' u (180*$5/PI):(180*$3/PI):(180*$4/PI) with errorbars lt 1 pt 7 lw 2 lc rgb '#09ad00' t 'Average error (1000 trials)'
set output

#LENGTH
set output 'length_input_newNEW.eps'	###LENGTH
set xrange [0:55]			###LENGTH
set yrange [-0.5:12.5] 
set xlabel 'Outbound path length in m' ###LENGTH
#plot 'results_length.dat' u 1:(180*($3+$4)/PI):(180*($3-$4)/PI) w errorbars
#plot "results_length.dat" u 1:2 w l lt 1 lw 2 lc rgb '#00ad31' t 'Average error (1000 trials)'
#plot "results_length.dat" u 1:2:3 w errorbars lt 1 ps 0.75 pt 7 lw 2 lc rgb '#00ad31' t 'Average error (1000 trials)'
#set output

#RESPONSE
#plot 'cosinput.dat' u 2:(180*$3/PI):(180*$4/PI) with errorbars lt 1 pt 7 lw 2 lc rgb '#00ad31' t 'Average error (1000 trials)', 'binaryinput.dat' u 2:(180*$3/PI):(180*$4/PI) with errorbars lt 1 pt 7 lw 2 lc rgb '#00ad88' t 'Average error (1000 trials)', 'gaussianinput.dat' u 2:(180*$3/PI):(180*$4/PI) with errorbars lt 1 pt 7 lw 2 lc rgb 'red' t 'Average error (1000 trials)'

#NOISE
set output '../../figs/multinoise.eps'		###NOISE
set xrange [-1:21]		###NOISE
set yrange [-0.5:5.5]
#set xlabel 'Sensor Noise Variance {/Symbol-Oblique d}'	###NOISE
set xlabel 'Sensory Noise Level in %'	###NOISE
plot '../nrmse_noise_fullAdd.dat' u (100*$1):2:3 with errorbars lt 1 pt 7 ps 0.5 lc rgb '#00ada4' t '', '../nrmse_noise_fullMult.dat' u (100*$1):2:3 with errorbars lt 1 pt 7 ps 0.5 lc rgb '#ad00a4' t ''
set output

#plot 'input_neurons_gauss2.dat' u 1:(180*$3/PI):(180*$4/PI) with errorbars lt 1 pt 7 lw 2 lc rgb '#00ad31' t 'Gauss 2', 'input_neurons_gauss4.dat' u 1:(180*$3/PI):(180*$4/PI) with errorbars lt 1 pt 7 lw 2 lc rgb '#0042ad' t 'Gauss 4',  'input_neurons_gauss8n.dat' u 1:(180*$3/PI):(180*$4/PI) with errorbars lt 1 pt 7 lw 2 lc rgb 'red' t 'Gauss 8',  'input_neurons_binary.dat' u 1:(180*$3/PI):(180*$4/PI) with errorbars lt 1 pt 7 lw 2 lc rgb 'brown' t 'Binary',  'input_neurons_cos.dat' u 1:(180*$3/PI):(180*$4/PI) with errorbars lt 1 pt 7 lw 2 lc rgb 'orange' t 'Cosine'

#SYSTEMATIC
set output 'systematic_newboth.eps'		###SYSTEMATIC
set xtics 30				###SYSTEMATIC
set ytics 10				###SYSTEMATIC
set xlabel "Second Leg Angle {/Symbol-Oblique a}"	###SYSTEMATIC
set ylabel "Angular Error {/Symbol-Oblique e}"
set xrange [-5:185]			###SYSTEMATIC
set yrange [-2:36] 
#plot 'nrmse_0.dat' u (180*$1/PI):(180*$3/PI):(180*($4*0.5)/PI) w l ls 2 lc rgb '#836FFF' t "{/Symbol-Oblique l} = 0.000", "nrmse_1.dat" u (180*$1/PI):(180*$3/PI):(180*($4*0.5)/PI) w l ls 4 lc rgb '#B22222' t "{/Symbol-Oblique l} = 0.007", "nrmse_2.dat" u (180*$1/PI):(180*$3/PI):(180*($4*0.5)/PI) w l ls 6 lc rgb '#00EE00' t "{/Symbol-Oblique l} = 0.0075", "nrmse_3.dat" u (180*$1/PI):(180*$3/PI):(180*($4*0.5)/PI) w l ls 8 lc rgb '#FFC125' t "{/Symbol-Oblique l} = 0.008", "nrmse_4.dat" u (180*$1/PI):(180*$3/PI):(180*($4*0.5)/PI) w l ls 10 lc rgb '#CD00CD' t "{/Symbol-Oblique l} = 0.009", 'mueller89.dat' u 1:2:3 w errorbars pt 7 lt 1 lw 1.5 lc rgb 'black' t "Data from Müller (1989)", 'muellerwehner88.dat' u 1:2:3 w errorbars pt 7 lt 1 lw 1.5 lc rgb 'red' t "Data from Müller/Wehner (1988)"
#set output

#set output 'errorsNEW.eps'
#set yrange [-90:90]
#set xlabel "Foraging distance {/Helvetica-Italic l}"
#plot 'error.dat' u 1:(180*$2/PI) w p
#plot 'nrmse.dat' u 5:(180*($3+$4)/PI):(180*($3-$4)/PI) w errorbars

#filledcurve lt 1 lc rgb '#C684FF' t "", '' u 5:(180*($3+$4)/PI) w l lt 1 lc rgb '#AA47FF'  t "",  '' u 5:(180*($3-$4)/PI) w l lt 1 lc rgb '#AA47FF'  t "", "" u 5:(180*$3/PI) w l lt 1 lc rgb '#8900FF' t "{/Helvetica-Italic b} = 0.5"
#, "nrmse_1.dat" u 2:(180*$3/PI) w l lt 1 lc rgb 'green' t "{/Helvetica-Italic b} = 1.0", "nrmse_2.dat" u 2:(180*$3/PI) w l lt 1 lc rgb 'blue' t "{/Helvetica-Italic b} = 2.0", "nrmse_3.dat" u 2:(180*$3/PI) w l lt 1 lc rgb 'red' t "{/Helvetica-Italic b} = 4.0", "nrmse_4.dat" u 2:(180*$3/PI) w l lt 1 lc rgb 'brown' t "{/Helvetica-Italic b} = 8.0"

#NEURONS
unset grid
set output 'neurons_input_new.eps'	###NEURONS
set logscale x				###NEURONS
set xlabel "Number of Neurons"		###NEURONS
set ylabel "Mean Angular Error {\74}&{j}{/Symbol-Oblique e}&{j}{\76}"
set yrange [-20.0:90]
set xrange [0.95:10000]			###NEURONS
set xtics autofreq
set ytics 20
#plot "nrmse_n.dat" u 1:(180*$3/PI):(180*$4/PI) with errorbars lt 1 pt 7 lw 2 lc rgb '#00ad14' t ''
#set output
