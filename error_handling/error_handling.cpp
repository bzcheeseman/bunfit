#include "error_handling.h"

using namespace std;

std::string error_handling::out(int code)
{
	ifstream err ("error_handling/error_codes.txt");
	if (err.is_open()){
		string numcode;
		bool done = false;
		while (err.good()){
			if (!done){
				getline(err, numcode);

				stringstream ss (numcode);

				int i; char c; string words;

				ss >> i >> c >> words;

				if (i == code){
					done = true;
					err.close();
					break;
				}
			}
		}
		if (done == true)
			return numcode;

		else{
			cout << "Error code " << code 
			<< " not found, please create the entry!" << endl;
			return "End";
		}
	}
	else{
		cout << "File error - Did not open/couldn't read or write error codes text" << endl;
		return "File error";
	}

	return "End";
}