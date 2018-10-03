// CSE 332 Lab 4.cpp : Defines the entry point for the console application.
//

#include "Utilities.h"
#include "Game.h"

#include <iostream>

using namespace std;

int main(int argc, char * argv[]) {

	//endStatus will vary depending on the error/value thrown/returned from program
	int endStatus;

	try {
		//look at #17 for this part
		Game::getGame(argc, argv);
		endStatus = (Game::instance())->play();
	}
	catch (bad_alloc & e) {
		cout << "Failed to allocate memory for game: " << e.what();
		endStatus = MEM_ALLOC_FAILURE;
	}
	catch (int error) {
		return error;
	}

	return endStatus;
}