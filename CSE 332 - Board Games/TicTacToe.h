#pragma once
#include "Game.h"

class TicTacToe : public Game {
public:

	//allows for the board to be printed out to the console properly
	friend ostream & operator<<(ostream & os, const TicTacToe & game);

	TicTacToe();		//sets up the board
	~TicTacToe();		//deallocates memory for board from heap

	virtual void print();
	virtual bool done();
	virtual bool draw();
	virtual int turn();
	virtual int play();
};

ostream & operator<<(ostream & os, const TicTacToe & game);
