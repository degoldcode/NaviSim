### check if file exists
file="onelm_multi_lvlearn"
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
g++ test/lvlearn_multi_onelm.cpp src/agent.cpp src/environment.cpp src/simulation.cpp src/controller.cpp src/goal.cpp src/landmark.cpp src/pipe.cpp src/object.cpp src/pin.cpp src/goallearning.cpp src/routelearning.cpp -std=c++11 -o $file -O1 -larmadillo
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
#gnuplot track_vectors.plot
#gnuplot histogram.gnu
#gnuplot stat_distance.plot
#gnuplot track.plot
gnuplot track_local.plot
gnuplot track_lmr_vectors.plot
gnuplot distri.plot
#gnuplot activations.plot
#gnuplot gv.plot
gnuplot lv.plot
gnuplot lv_signals.plot
gnuplot signals.plot
gnuplot ref.plot
#gnuplot reward.plot
#python circle.py
#python circle_gv.py
#gnuplot gv_performance.plot
#python density.py
fi

cd ..
### backup data with timestamp
if [ "$1" = "all" ] || [ "$1" = "run" ] ; then
echo "Backup data."
timestamp=$( date +"%y%m%d-%T")
mkdir ../data_container/lvlearn_multi_onelm/$timestamp/
cp *.dat ../data_container/lvlearn_multi_onelm/$timestamp/
fi

if [ "$1" = "" ] ; then
echo "Nothing"
fi
echo "Done."
