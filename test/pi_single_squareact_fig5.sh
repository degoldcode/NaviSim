### check if file exists
file="pi_single_fig5"
if [ -f "../$file" ]
then
	echo "Remove $file."
	rm ../$file
else
	echo "$file not found."
fi

cd ..
### compile c++ code
if [ "$1" == "all" ] || [ "$1" == "compile" ] || [ "$1" == "run" ] ; then
echo "Compile."
g++ test/pi_single_squareact_fig5.cpp src/agent.cpp src/environment.cpp src/simulation.cpp src/controller.cpp src/goal.cpp src/landmark.cpp src/pipe.cpp src/object.cpp src/pin.cpp src/goallearning.cpp src/routelearning.cpp -std=c++11 -o $file -O1 -larmadillo
fi

### run program
if [ "$1" == "all" ] || [ "$1" == "run" ] ; then
echo "Run program."
./$file
fi

cd data/scripts
### plot data using gnuplot gui
if [ "$1" = "all" ] || [ "$1" = "plot" ] ; then
echo "Plot data."
#gnuplot track_homevector.plot
#gnuplot histogram.gnu
#gnuplot stat_distance.plot
gnuplot track_fig5.plot
#gnuplot distri.plot
gnuplot activations_fig5.plot
#gnuplot gv.plot
#gnuplot lv.plot
#gnuplot ref.plot
#gnuplot reward.plot
#python circle.py
python circle_fig5.py
#gnuplot gv_performance.plot
#python density.py
fi

cd ..
### backup data with timestamp
if [ "$1" = "all" ] || [ "$1" = "run" ] ; then
echo "Backup data."
timestamp=$( date +"%y%m%d-%T")
mkdir ../data_container/pi_fig5/$timestamp/
cp *.dat ../data_container/pi_fig5/$timestamp/
fi

if [ "$1" = "" ] ; then
echo "Nothing"
fi
echo "Done."
