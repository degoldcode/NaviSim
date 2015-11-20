### check if file exists
file="fixedlv_single_lvlearn"
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
g++ test/lvlearn_single_fixedlv.cpp src/agent.cpp src/environment.cpp src/simulation.cpp src/controller.cpp src/goal.cpp src/landmark.cpp src/pipe.cpp src/object.cpp src/pin.cpp src/goallearning.cpp src/routelearning.cpp -std=c++11 -o $file -O1 -larmadillo
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
gnuplot lv.plot -e "num_lv=3"
gnuplot track_lv.plot
gnuplot track_eligtrac.plot
gnuplot signals.plot
gnuplot gv_performance.plot
fi

cd ..
### backup data with timestamp
if [ "$1" = "all" ] || [ "$1" = "run" ] ; then
echo "Backup data."
timestamp=$( date +"%y%m%d-%T")
mkdir ../data_container/lvlearn_single_fixedlm/$timestamp/
cp *.dat ../data_container/lvlearn_single_fixedlm/$timestamp/
fi

if [ "$1" = "" ] ; then
echo "Nothing"
fi
echo "Done."
