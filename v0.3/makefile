all:
	clang-omp++ ops_test.cpp -o ops -std=c++11 -lboost_python3 -I /usr/local/Cellar/python3/3.5.1/frameworks/python.framework/versions/3.5/headers -L/usr/local/Cellar/python3/3.5.1/frameworks/python.framework/versions/3.5/lib -lpython3.5 -I /usr/local/include/eigen3 -lglog -lgflags -lceres -g

clean:
	rm -rf ops, ops.dSYM
