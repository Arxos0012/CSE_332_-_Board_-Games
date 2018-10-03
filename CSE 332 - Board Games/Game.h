#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include "Utilities.h"

using namespace std;

typedef struct playerMove {
	int x, y;
} playerMove;

class Game {
public:

	virtual int play() = 0;		//starts and runs the game
	static void getGame(int len, char* args[]);
	static shared_ptr<Game> instance();	//returns a smart pointer the base game class if it's not null

protected:

	virtual void print() = 0;	//prints out the board
	virtual bool done() = 0;	//return true if ther is a victor
	virtual bool draw() = 0;	//return true if there are no moves left
	virtual int prompt(unsigned int & row, unsigned int & col);		//prompts current player to make a move
	virtual int turn() = 0;		//executes a turn of the game

	static shared_ptr<Game> game_ptr;

	int size_,
		leftBound_,
		rightBound_,
		topBound_,
		botBound_;
	
	unsigned int turns_, longestIconSize_;

	string** board_;
	string empty_, iconP1_, iconP2_;
	string saveEmpty_;
	bool is1sTurn_;

	vector<playerMove> p1Moves, p2Moves;

};
