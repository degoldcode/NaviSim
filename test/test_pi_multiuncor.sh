### check if file exists
file="multiuncor_pi_test"
if [ -f "$file" ]
then
	echo "Remove $file."
	rm ../$file
else
	echo "$file not found."
fi

### compile c++ code
echo "Compile."
cd ..
g++ test/test_pi_multiuncor.cpp src/agent.cpp src/environment.cpp src/simulation.cpp src/controller.cpp src/goal.cpp src/landmark.cpp src/object.cpp src/pin.cpp src/goallearning.cpp -std=c++11 -o $file -O1 -larmadillo

### run program
echo "Run program."
./$file

### plot data using gnuplot gui
echo "Plot data."
cd data/scripts
gnuplot nrmse_uncor.plot
#gnuplot track_homevector.plot
#gnuplot histogram.gnu
#gnuplot stat_distance.plot
#gnuplot distri.plot
#gnuplot activations.plot
#python circle.py
#python density.py

### backup data with timestamp
echo "Backup data."
cd ..
timestamp=$( date +"%y%m%d-%T")
mkdir ../data_container/pi_uncorrelated/$timestamp/
cp *.dat ../data_container/pi_uncorrelated/$timestamp/

echo "Done."
