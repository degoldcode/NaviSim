### check if file exists
file="../randforage_test"
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
g++ test/test_random_foraging.cpp src/agent.cpp src/environment.cpp src/simulation.cpp src/controller.cpp src/goal.cpp src/landmark.cpp src/object.cpp src/pin.cpp src/goallearning.cpp -std=c++11 -o randforage_test -O1 -larmadillo

### run program
echo "Run program."
./randforage_test

### plot data using gnuplot gui
echo "Plot data."
cd data/scripts
gnuplot track.plot
gnuplot histogram.gnu
gnuplot stat_distance.plot
gnuplot distri.plot
#python density.py

### backup data with timestamp
echo "Backup data."
cd ..
timestamp=$( date +"%y%m%d-%T")
mkdir ../data_container/$timestamp/
cp *.dat ../data_container/$timestamp/

echo "Done."
