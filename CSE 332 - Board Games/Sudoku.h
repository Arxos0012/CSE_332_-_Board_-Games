#pragma once
#include "Game.h"

class Sudoku : public Game {
public:
	friend ostream & operator<<(ostream & os, const Sudoku & game);

	Sudoku();
	~Sudoku();

	virtual void print();
	virtual bool done();
	virtual int prompt(unsigned int & row, unsigned int & col, unsigned int & cellChoice);
	virtual int turn();
	virtual int play();
	virtual bool draw();	//not used but needs to be declared

private:
	int blockSize_;
	string exSaveMarker_, nonExSaveMarker_;
	bool **preexisting_;	//will mark the cells that already had numbers from the start of the game

	enum Invalidity_Type {
		IN_ROW,
		IN_COLUMN,
		IN_BLOCK,
		NONE
	};

	bool invalid(int x, int y, int choice, Invalidity_Type & type);
};


ostream & operator<<(ostream & os, const Sudoku & game);
