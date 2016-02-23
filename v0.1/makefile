########### CHANGE THESE FOR YOUR SYSTEM #########################

#omp compiler
ccomp=g++-5

#homebrew installed headers
hf=/usr/local/include

#Eigen installation
eigen=/usr/local/include/eigen3

#output filename
output=test

#linked libraries
ll=-L /usr/local/lib

#generic libraries that need to be linked
libs=-lglog -lgflags -lceres

#target file
target=main

#######python linking only#######

#matplotlib linkings
mpl=-lboost_python -lpython2.7

#python include
pi=/usr/include/python2.7

#generic options
cflags=-std=c++11

#chi squared dependency
chisq=chi_squared/chi_squared

#get_data dependency
gd=get_data/get_data


######residual include#####
residual=$(HOME)/desktop/cpp/bunfit

################################commands################################
(all): $(output).o

$(output).o: $(target).cpp $(gd).cpp
	$(ccomp) -fopenmp $(target).cpp $(gd).cpp -o $(output).o -I $(eigen) -I $(hf)\
	 -I $(residual) $(cflags) -I $(pi) $(mpl)

clean:
	rm test.o
