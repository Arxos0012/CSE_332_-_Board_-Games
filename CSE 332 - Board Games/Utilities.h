/**
	Created by: Jabari Booker
	
	This header file contains enumerations for program arguements and 
	error checking, as well as some commonly used functions for genereal
	program operation. The definition of the functions here are in Utilities.cpp.
*/

#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef unsigned int uint;

enum Arguements {
	PROGRAM_NAME,
	GAME_TYPE
};

enum ErrorChecks {
	SUCCESS,
	USER_QUIT,
	DRAW_REACHED,
	NEED_ONE_ARG,
	INVALID_ARGUEMENT,
	MEM_ALLOC_FAILURE,
	GAME_PTR_NULL,
	GAME_PTR_WRONGLY_SET,
	FILE_READ_ERROR,
	FILE_WRITE_ERROR
};

const vector<string> validArguments = {"TicTacToe", "Gomoku", "Sudoku"};

int usage(char* programName, int error);