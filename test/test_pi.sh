cd ..
g++ test/test_pi.cpp pin.cpp agent.cpp -std=c++11 -o pin_test -O1 -larmadillo
./pin_test
cd test
gnuplot pos.plot
