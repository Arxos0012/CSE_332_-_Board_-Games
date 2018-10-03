#include "Gomoku.h"

Gomoku::Gomoku() {
	/*initializing all member variables according to the
	Gomoku rules of play*/
	size_ = 19;

	topBound_ = size_ - 1;
	rightBound_ = size_ - 1;

	leftBound_ = 0;
	botBound_ = 0;

	empty_ = " ";
	saveEmpty_ = "~";

	turns_ = 0;				//no turns taken yet
	longestIconSize_ = (((int)empty_.size() - (int)to_string(size_ - 1).size()) > 0)
		? empty_.size() : to_string(size_ - 1).size();

	is1sTurn_ = true;

	iconP1_ = "B";
	iconP2_ = "W";

	numWinStone_ = 5;

	board_ = new string*[size_];

	try {
		//initiating the game board;
		board_ = new string*[size_];

		ifstream savefile("Gomoku.save");

		//checking to see if the savefile has the proper game data
		string gameType;
		savefile >> gameType;

		//if it does, load the last save
		if (gameType == "Gomoku") {

			//determining state of game based on save data

			string lastTurn;					//who's turn it is
			savefile >> lastTurn;
			is1sTurn_ = (lastTurn == iconP1_);

			savefile >> turns_;		//getting number of turns last played
			savefile >> longestIconSize_;	//getting size of longest icon

			string s;

			//reading moves for each player
			savefile >> s;
			if (s == "P1Moves") {
				savefile >> s;
				while (s != "P2Moves") {
					playerMove move;

					move.x = stoi(s);

					savefile >> s;
					move.y = stoi(s);

					p1Moves.push_back(move);

					savefile >> s;
				}
			}
			if (s == "P2Moves") {
				savefile >> s;
				while (s != "P2MovesEnd") {
					playerMove move;

					move.x = stoi(s);

					savefile >> s;
					move.y = stoi(s);

					p2Moves.push_back(move);

					savefile >> s;
				}
			}

			//reading last state of the board
			for (int i = 0; i < size_; ++i) {
				board_[i] = new string[size_];

				for (int j = 0; j < size_; ++j) {
					savefile >> s;
					s = (s == saveEmpty_) ? empty_ : s;
					board_[i][j] = s;
				}
			}

		}
		//other wise start a new game
		else {
			//allocating board_ and initializing all spaces to empty_
			for (int i = 0; i < size_; ++i) {
				board_[i] = new string[size_];

				for (int j = 0; j < size_; ++j) {
					board_[i][j] = empty_;
				}
			}
		}

		savefile.close();
	}
	catch (bad_alloc & e) {
		throw e;
	}
	catch (ofstream::failure & e) {
		cout << "Failed opening/closing file!" << endl;
		throw FILE_READ_ERROR;
	}
}

Gomoku::~Gomoku() {
	//deallocating board_
	for (int i = 0; i < size_; ++i) {
		delete[] board_[i];
	}

	delete[] board_;
}

ostream & operator<<(ostream & os, const Gomoku & game) {

	int largestIndexSize = to_string(game.size_ - 1).size();

	for (int i = game.size_ - 1; i >= 0; --i) {
		//row labelling
		os << setw(largestIndexSize) << i << " ";

		//print out contents of the board by row
		for (int j = 0; j < game.size_; ++j) {
			os << setw(game.longestIconSize_) << game.board_[i][j] << " ";
		}
		os << "\n";
	}

	os << setw(largestIndexSize) << "X" << " ";

	for (int x = 0; x < game.size_; ++x) {
		os << setw(game.longestIconSize_) << x << " ";
	}

	os << "\n";

	return os;
}

void Gomoku::print() {
	cout << *this << endl;
}

bool Gomoku::done() {
	//look for numWinStone_ stones in a row to win

	//checking horizontal mathces
	
	//shifting column based search over when done searching a column
	for (int x = 0; x < size_ - numWinStone_ + 1; ++x) {
		//checking every row in column
		for (int i = 0; i < size_; ++i) {
			string prev = board_[i][x];
			bool allSame = true;

			int j;
			for (j = 0; j < numWinStone_; ++j) {
				if (board_[i][j + x] != prev || board_[i][j + x] == empty_) {
					allSame = false;
					break;
				}
				prev = board_[i][j + x];
			}

			if (allSame) {
				return true;
			}
		}
	}
	
	//checking vertical mathces

	//shifting row based search over when done searching a row
	for (int x = 0; x < size_ - numWinStone_ + 1; ++x) {
		//checking every column in row
		for (int j = 0; j < size_; ++j) {
			string prev = board_[x][j];
			bool allSame = true;

			int i;
			for (i = 0; i < numWinStone_; ++i) {
				if (board_[x + i][j] != prev || board_[x + i][j] == empty_) {
					allSame = false;
					break;
				}
				prev = board_[x + i][j];
			}

			if (allSame) {
				return true;
			}
		}
	}

	//checking postive diagonal mathces

	//shifting start of search over to the right
	for (int x = 0; x < size_ - numWinStone_ + 1; ++x) {

		//shifting start of serach up
		for (int y = 0; y < size_ - numWinStone_ + 1; ++y) {
			string prev = board_[y][x];
			bool allSame = true;

			//searching for positive diagonal match starting from (x,y)
			int i;
			for (i = 0; i < numWinStone_; ++i) {
				if (board_[y + i][x + i] != prev || board_[y + i][x + i] == empty_) {
					allSame = false;
					break;
				}
				prev = board_[y + i][x + i];
			}

			if (allSame) {
				return true;
			}
		}
	}
	
	//checking negative diagonal mathces

	//shifting start of search over to the right
	for (int x = 0; x < size_ - numWinStone_ + 1; ++x) {

		//starting from numWinStone_ - 1 and shifting start of search up
		for (int y = numWinStone_ - 1; y < size_; ++y) {
			string prev = board_[y][x];
			bool allSame = true;

			//searching for negative diagonal match starting from (x,y)
			int i;
			for (i = 0; i < numWinStone_; ++i) {
				if (board_[y - i][x + i] != prev || board_[y - i][x + i] == empty_) {
					allSame = false;
					break;
				}
				prev = board_[y - i][x + i];
			}

			if (allSame) {
				return true;
			}
		}
	}

	return false;
}

bool Gomoku::draw() {

	bool noMoves = true,
		foundEmpty = false;

	//checking if all spaces on the board_ are occupaied
	//return true if so
	for (int i = botBound_; i <= topBound_; ++i) {
		for (int j = leftBound_; j <= rightBound_; ++j) {
			if (board_[i][j] == empty_) {
				noMoves = false;
				foundEmpty = true;
				break;
			}
		}

		if (foundEmpty) {
			break;
		}
	}

	return noMoves && !done();
}

int Gomoku::turn() {

	unsigned int choiceX, choiceY;
	string marker;

	cout << "<Player ";

	if (is1sTurn_) {
		cout << iconP1_;
		marker = iconP1_;
	}
	else {
		cout << iconP2_;
		marker = iconP2_;
	}

	cout << ">: ";

	vector<playerMove> & moveset = (is1sTurn_) ? p1Moves : p2Moves;

	for (int i = 0; i < (int)moveset.size(); ++i) {
		cout << moveset[i].x << ", " << moveset[i].y << "; ";
	}

	cout << endl;

	while (true) {
		if (prompt(choiceX, choiceY) == USER_QUIT) {
			cout << "Player " << marker << " has quit the game! " << turns_ << " turns were taken. " << endl;

			string ans;

			cout << "Would you like to save the game though (yes/no)?: ";
			cin >> ans;
			cout << endl;

			while (ans != "yes" && ans != "no") {
				cout << "Please answer \"yes\" or \"no\". Would you like to save the game?: ";
				cin >> ans;
				cout << endl;
			}

			try {
				if (ans == "yes") {
					ofstream savefile("Gomoku.save");

					savefile << "Gomoku";	//writing gametype to save

					savefile << "\n";

					//writing game data

					savefile << marker << " ";	//current player's turn
					savefile << turns_ << " ";	//number of turns taken
					savefile << longestIconSize_;	//size of longest icon
					savefile << "\nP1Moves ";

					//Moves take by the first player
					for (unsigned int i = 0; i < p1Moves.size(); ++i) {
						savefile << p1Moves[i].x << " " << p1Moves[i].y << " ";
					}
					savefile << "\nP2Moves ";

					//Moves take by the second player
					for (unsigned int i = 0; i < p2Moves.size(); ++i) {
						savefile << p2Moves[i].x << " " << p2Moves[i].y << " ";
					}
					savefile << "P2MovesEnd\n";

					//writing board state to file
					for (int x = 0; x < size_; ++x) {
						for (int y = 0; y < size_; ++y) {
							string in = board_[x][y];
							in = (in == empty_) ? saveEmpty_ : in;
							savefile << in << " ";
						}
					}

					savefile.close();
				}
				else {
					ofstream savefile("Gomoku.save");

					//wrtie to indicate no valid save data
					savefile << "NO DATA";

					savefile.close();
				}
			}
			catch (ofstream::failure & e) {
				cout << "Failed closing save file! Save not completed!" << endl;
				return FILE_WRITE_ERROR;
			}

			return USER_QUIT;
		}

		if (board_[choiceY][choiceX] == empty_) {
			board_[choiceY][choiceX] = marker;
			break;
		}

		cout << "This space is already marked!" << endl;
	}

	is1sTurn_ = !is1sTurn_;
	++turns_;

	return SUCCESS;
}

int Gomoku::play() {
	bool playing = true;

	while (playing) {
		print();

		if (turn() == USER_QUIT) {
			return USER_QUIT;
		}

		if (done()) {
			print();
			string player = !is1sTurn_ ? iconP1_ : iconP2_;
			cout << "Player " << player << " has won! Congrats!" << endl;

			//creating new save with no game data
			try {
				ofstream savefile("Gomoku.save");
				savefile << "NO DATA";
				savefile.close();
			}
			catch (ofstream::failure & e) {
				cout << "Failed closing save file! Save not completed!" << endl;
				return FILE_WRITE_ERROR;
			}

			break;
		}

		if (draw()) {
			print();
			cout << "Neither player won...with a total of " << turns_ << " turns...sad..." << endl;

			//creating new save with no game data
			try {
				ofstream savefile("Gomoku.save");
				savefile << "NO DATA";
				savefile.close();
			}
			catch (ofstream::failure & e) {
				cout << "Failed closing save file! Save not completed!" << endl;
				return FILE_WRITE_ERROR;
			}

			return DRAW_REACHED;
		}
	}

	return SUCCESS;
}