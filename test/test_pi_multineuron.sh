### check if file exists
file="multineuron_pi_test"
if [ -f "../$file" ]
then
	echo "Remove $file."
	rm $file
else
	echo "$file not found."
fi

### compile c++ code
echo "Compile."
cd ..
g++ test/test_pi_multineuron.cpp src/agent.cpp src/environment.cpp src/simulation.cpp src/controller.cpp src/goal.cpp src/landmark.cpp src/object.cpp src/pin.cpp src/goallearning.cpp -std=c++11 -o $file -O1 -larmadillo

### run program
echo "Run program."
./$file

### plot data using gnuplot gui
echo "Plot data."
cd data/scripts
gnuplot nrmse_neuron.plot

### backup data with timestamp
echo "Backup data."
cd ..
timestamp=$( date +"%y%m%d-%T")
mkdir ../data_container/pi_neurons/$timestamp/
cp {nrmse_neuron.dat, sim.cfg}  ../data_container/pi_neurons/$timestamp/

echo "Done."
