#pragma once
#include "Game.h"

class Gomoku : public Game {
public:

	friend ostream & operator<<(ostream & os, const Gomoku & game);

	Gomoku();
	~Gomoku();

	virtual void print();
	virtual bool done();
	virtual bool draw();
	virtual int turn();
	virtual int play();

private:
	int numWinStone_;
};

ostream & operator<<(ostream & os, const Gomoku & game);
