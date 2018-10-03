/**
	Created by: Jabari Booker
	
	This is the source file where the functions of Utilities.h are defined.
*/

#include "Utilities.h"

int usage(char *programName, int error) {
	switch (error) {
	case NEED_ONE_ARG:
		cout << "One and only one argument must be passed!" << endl;
		break;
	case INVALID_ARGUEMENT:
		cout << "Invalid arguement passed! Valid arguements are: ";
		for (int i = 0; i < validArguments.size(); ++i) {
			cout << validArguments[i] << " ";
		}
		cout << endl;
	}

	cout << "usage: " << programName << " <game_arg>" << endl;

	return error;
}