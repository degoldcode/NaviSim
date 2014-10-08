cd ..
g++ test/test_gl.cpp goallearning.cpp pin.cpp environment.cpp agent.cpp goal.cpp landmark.cpp pipe.cpp -std=c++11 -o gl_test -O1 -larmadillo
./gl_test
cd test
gnuplot pos.plot
gnuplot weights.plot
