#include "TicTacToe.h"

TicTacToe::TicTacToe() {
	/*initializing all member variables according to the 
	TicTacToe rules of play*/
	size_ = 5;

	topBound_ = size_ - 2;
	rightBound_ = size_ - 2;

	leftBound_ = 1;
	botBound_ = 1;

	empty_ = " ";
	saveEmpty_ = "~";

	turns_ = 0;				//no turns taken yet
	longestIconSize_ = (((int)empty_.size() - (int)to_string(size_ - 1).size()) > 0)
		? empty_.size() : to_string(size_ - 1).size();

	is1sTurn_ = true;

	iconP1_ = "X";
	iconP2_ = "O";

	try {
		//initiating the game board;
		board_ = new string*[size_];

		ifstream savefile("TicTacToe.save");
		
		//checking to see if the savefile has the proper game data
		string gameType;
		savefile >> gameType;
		
		//if it does, load the last save
		if (gameType == "TicTacToe") {
			
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
		//otherwise start a new game
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

TicTacToe::~TicTacToe() {
	//deallocating board_
	for (int i = 0; i < size_; ++i) {
		delete[] board_[i];
	}

	delete[] board_;
}

ostream & operator<<(ostream & os, const TicTacToe & game) {

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

void TicTacToe::print() {
	cout << *this << endl;
}

bool TicTacToe::done() {

	//checking horizontal mathces
	for (int i = botBound_; i <= topBound_; ++i) {
		bool allSame = true;
		string prev = board_[i][leftBound_];
		int j;

		for (j = leftBound_; j <= rightBound_; ++j) {
			if (board_[i][j] != prev) {
				allSame = false;
				break;
			}
		}
		
		if (allSame && board_[i][j - 1] != empty_) {
			return true;
		}
	}

	//checking vertical mathces
	for (int j = leftBound_; j <= rightBound_; ++j) {
		bool allSame = true;
		string prev = board_[botBound_][j];
		int i;

		for (i = botBound_; i <= topBound_; ++i) {
			if (board_[i][j] != prev) {
				allSame = false;
				break;
			}
		}

		if (allSame && board_[i - 1][j] != empty_) {
			return true;
		}
	}

	//checking positive diagonal
	string prev = board_[botBound_][leftBound_];
	bool allSame = true;

	for (int i = botBound_; i <= topBound_; ++i) {
		if (board_[i][i] != prev) {
			allSame = false;
			break;
		}
		prev = board_[i][i];
	}

	if (allSame && board_[botBound_][leftBound_] != empty_) {
		return true;
	}
	//checking negative diagonal
	prev = board_[topBound_][leftBound_];
	allSame = true;

	for (int i = 0; topBound_ - i >= botBound_; ++i) {
		if (board_[topBound_ - i][i + leftBound_] != prev) {
			allSame = false;
			break;
		}
		prev = board_[topBound_ - i][i + leftBound_];
	}

	if (allSame && board_[topBound_][leftBound_] != empty_) {
		return true;
	}

	return false;

}

bool TicTacToe::draw() {

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

int TicTacToe::turn() {

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
					ofstream savefile("TicTacToe.save");

					savefile << "TicTacToe";	//writing gametype to save

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
					ofstream savefile("TicTacToe.save");
					
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

int TicTacToe::play() {

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
				ofstream savefile("TicTacToe.save");
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
				ofstream savefile("TicTacToe.save");
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