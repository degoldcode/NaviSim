### check if file exists
file="multineuron_pi"
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
g++ test/pi_multineuron.cpp src/agent.cpp src/environment.cpp src/simulation.cpp src/controller.cpp src/goal.cpp src/landmark.cpp src/pipe.cpp src/object.cpp src/pin.cpp src/goallearning.cpp src/routelearning.cpp -std=c++11 -o $file -O1 -larmadillo
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
gnuplot nrmse_neuron.plot
fi

cd ..
### backup data with timestamp
if [ "$1" = "all" ] || [ "$1" = "run" ] ; then
echo "Backup data."
timestamp=$( date +"%y%m%d-%T")
mkdir ../data_container/pi_neurons/$timestamp/
cp nrmse_neuron.dat ../data_container/pi_neurons/$timestamp/
cp sim.cfg  ../data_container/pi_neurons/$timestamp/
fi

if [ "$1" = "" ] ; then
echo "Nothing"
fi
echo "Done."
