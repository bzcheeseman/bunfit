########### CHANGE THESE FOR YOUR SYSTEM #########################

#omp compiler
ccomp=clang-omp++

#normal compiler
cc=clang++

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
target=test

#######python linking only#######

#matplotlib linkings
mpl=-lboost_python -lpython2.7

#python include
pi=/usr/include/python2.7

#generic options
cflags=-std=c++11

######dylibs only#######
dyflags=-dynamiclib -current_version 0.1

#chi squared dependency
chisq=chi_squared/chi_squared

#lib name
chisqlib=chi_squared

#get_data dependency
gd=get_data/get_data

#lib name
gdlib=get_data

#error handling
eh=error_handling/error_handling

#lib name
ehlib=error_handling

######residual include#####
residual=residuals

################################commands################################
(all): $(output).o

$(output).o: $(target).cpp lib$(gdlib).dylib lib$(ehlib).dylib
	$(ccomp) $(cflags) $(ll) $(libs) -I $(hf) -I $(eigen) -I $(residual) -I $(pi)\
	 -L $(ehlib) -l$(ehlib) -L $(chisqlib) -l$(chisqlib) \
	 -L $(gdlib) -l$(gdlib) -o $(output).o $(target).cpp $(mpl)

lib$(chisqlib).dylib: $(chisq).cpp
	$(ccomp) -fopenmp $(cflags) $(dyflags) -I $(hf) -I $(eigen) -I $(residual)\
	 -o chi_squared/lib$(chisqlib).dylib $(chisq).cpp

lib$(gdlib).dylib: $(gd).cpp
	$(cc) $(cflags) $(dyflags) -I $(hf) -I $(eigen) -I $(residual)\
	 -o get_data/lib$(gdlib).dylib $(gd).cpp

lib$(ehlib).dylib: $(eh).cpp
	$(cc) $(cflags) $(dyflags) -o error_handling/lib$(ehlib).dylib $(eh).cpp

clean:
	rm $(target) chi_squared/$(chisqlib) get_data/$(gdlib) \
	error_handling/$(ehlib)












