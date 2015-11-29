# Bunfit

This project is dedicated to using C++ for curve fitting, and using the impressive plotting abilities of Matplotlib (from python) to visualize results.  The applicability is mostly for live-type code, where results are required instantly.  This project is under construction, and anything even under the master branch has no guarantee of any kind.  Eventually I will hopefully have something that is robust enough and works well enough with minimal input from the person working on it that I will be able to release it for real.  For now, if you have suggestions please, by all means, let me know how to improve - or make the changes and tell me what they are!

This is a pet project, and so there are no guarantees anything will work, or that I will bug fix in a timely manner.  Very much a side project, at least for now.

If a program returns an error code, here is what they mean:

0 = Success
1 = File error - Did not open/couldn’t read or write
2 = Problem with solution of system
3 = Unforseen problem - contact owner/maintainer for debugging
4 = Improper command line arguments, need more or the ones entered are invalid
5 = Need file I/O arguments
6 = Unknown arguments passed to function
7 = Data error - check data output/input to function type
8 = Not implemented (Yet……) - implement it!
9 = Use different function, requested operation not within function parameters
10 = Check function and guesses, make sure they align
11 = Item already exists in an immutable file, please choose a different name
