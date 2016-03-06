all: g_d.o plot_data.cpp
	g++ plot_data.cpp -o test -std=c++11 g_d.o -lglog -lboost_python3 -I /usr/local/Cellar/python3/3.5.1/frameworks/python.framework/versions/3.5/headers -L/usr/local/Cellar/python3/3.5.1/frameworks/python.framework/versions/3.5/lib -lpython3.5

g_d.o: get_data.cpp
	g++ -c get_data.cpp -o g_d.o -std=c++11

clean:
	rm -rf g_d.o test
