### check if file exists
file="../single_pi"
if [ -f "$file" ]
then
	echo "Remove $file."
	rm $file
else
	echo "$file not found."
fi

### compile c++ code
echo "Compile."
cd ..
g++ test/pi_single.cpp src/agent.cpp src/environment.cpp src/simulation.cpp src/controller.cpp src/goal.cpp src/landmark.cpp src/pipe.cpp src/object.cpp src/pin.cpp src/goallearning.cpp src/routelearning.cpp -std=c++11 -o $file -O1 -larmadillo

### run program
echo "Run program."
./single_pi_test

### plot data using gnuplot gui
echo "Plot data."
cd data/scripts
gnuplot track_homevector.plot
#gnuplot histogram.gnu
#gnuplot stat_distance.plot
#gnuplot distri.plot
gnuplot activations.plot
#python circle.py
#python density.py

### backup data with timestamp
echo "Backup data."
cd ..
timestamp=$( date +"%y%m%d-%T")
mkdir ../data_container/pi_single/$timestamp/
cp *.dat ../data_container/pi_single/$timestamp/

echo "Done."
