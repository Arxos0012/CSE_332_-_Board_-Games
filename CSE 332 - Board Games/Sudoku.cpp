#include "Sudoku.h"

Sudoku::Sudoku() {
	size_ = 9;
	blockSize_ = 3;

	topBound_ =  size_ - 1;
	rightBound_ = size_ - 1;

	leftBound_ = 0;
	botBound_ = 0;

	empty_ = " ";
	saveEmpty_ = "0";

	exSaveMarker_ = "X";
	nonExSaveMarker_ = "O";

	turns_ = 0;
	longestIconSize_ = (((int)empty_.size() - (int)to_string(size_ - 1).size()) > 0)
		? empty_.size() : to_string(size_ - 1).size();

	try{
		//initalizing the game board and "preexisting" array
		board_ = new string*[size_];
		preexisting_ = new bool*[size_];
		for (int i = 0; i < size_; ++i) {
			board_[i] = new string[size_];
			preexisting_[i] = new bool[size_];
		}

		ifstream savefile("Sudoku.save");

		//checking to see if the savefile has the proper game data
		string gameType;
		savefile >> gameType;

		//if it does, load the last save
		if (gameType == "Sudoku") {

			savefile >> turns_; //reading number of turns
			savefile >> longestIconSize_;	//getting size of longest icon

			//reading in the board
			string num;
			for (int i = 0; i < size_; ++i) {
				for (int j = 0; j < size_; ++j) {
					savefile >> num;
					board_[i][j] = (num == saveEmpty_) ? empty_ : num;
				}
			}
			
			//reading in data for which cells had numbers sinces the beginning
			string exMarker;
			for (int i = 0; i < size_; ++i) {
				for (int j = 0; j < size_; ++j) {
					savefile >> exMarker;
					preexisting_[i][j] = exMarker == exSaveMarker_;
				}
			}

			savefile.close();
		}
		//otherwise start a new game based on starting data in sudoku0.txt
		else {
			savefile.close();

			ifstream startfile("sudoku0.txt");
			string num;
			
			//initializing/allocating board
			for (int i = 0; i < size_; ++i) {
				for (int j = 0; j < size_; ++j) {
					startfile >> num;

					preexisting_[i][j] = num != saveEmpty_;
					board_[i][j] = (num == saveEmpty_) ? empty_ : num;
				}
			}

			startfile.close();
		}
	}
	catch (bad_alloc & e) {
		throw e;
	}
	catch (ofstream::failure & e) {
		cout << "Failed opening/closing file!" << endl;
		throw FILE_READ_ERROR;
	}
}

Sudoku::~Sudoku() {
	for (int i = 0; i < size_; ++i) {
		delete[] board_[i];
		delete[] preexisting_[i];
	}

	delete[] preexisting_;
	delete[] board_;
}

ostream & operator<<(ostream & os, const Sudoku & game) {
	int largestIndexSize = to_string(game.size_ - 1).size();

	int rowCount = 0, colCount = 0;

	for (int i = game.size_ - 1; i >= 0; --i) {
		//row labelling
		os << setw(largestIndexSize) << i << "||";
		++rowCount;

		//print out contents of the board by row
		for (int j = 0; j < game.size_; ++j) {
			++colCount;
			os << setw(game.longestIconSize_) << game.board_[i][j] << "|";

			if (colCount % game.blockSize_ == 0) {
				os << "|";
			}
		}

		os << "\n";

		if (rowCount % game.blockSize_ != 0) {
			for (int x = 0; x < game.longestIconSize_; ++x) {
				os << "-";
			}

			os << "||";

			for (int j = 0; j < game.size_; ++j) {
				++colCount;
				for (int x = 0; x < game.longestIconSize_; ++x) {
					os << "-";
				}

				if (colCount % game.blockSize_ == 0) {
					os << "||";
				}
				else {
					os << "|";
				}
			}
		}
		else {
			for (int x = 0; x < game.longestIconSize_; ++x) {
				os << "=";
			}

			os << "||";

			for (int j = 0; j < game.size_; ++j) {
				++colCount;
				for (int x = 0; x < game.longestIconSize_; ++x) {
					os << "=";
				}

				if (colCount % game.blockSize_ == 0) {
					os << "||";
				}
				else {
					os << "|";
				}
			}
		}

		os << "\n";
	}

	os << setw(largestIndexSize) << "#" << "||";

	for (int x = 0; x < game.size_; ++x) {
		++colCount;
		os << setw(game.longestIconSize_) << x << "|";

		if (colCount % game.blockSize_ == 0) {
			os << "|";
		}
	}

	os << "\n";

	return os;
}

void Sudoku::print() {
	cout << *this << endl;
}

bool Sudoku::draw() {
	//there is never a draw since you don't play against another person
	return false;
}

bool Sudoku::done() {

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

	return noMoves;
}

bool Sudoku::invalid(int x, int y, int choice, Invalidity_Type & type) {

	//first check row
	for (int i = 1; i < size_ - 1; ++i) {
		int n;
		istringstream iss(board_[y][(x + i) % size_]);
		iss >> n;
		if (n == choice) {
			type = IN_ROW;
			return true;
		}
	}

	//now check column
	for (int i = 1; i < size_ - 1; ++i) {
		int n;
		istringstream iss(board_[(y + i) % size_][x]);
		iss >> n;
		if (n == choice) {
			type = IN_COLUMN;
			return true;
		}
	}

	//checking block
	int newY = y;
	int newX = x;

	for (int j = 0; j < blockSize_; ++j) {
		for (int i = 0; i < blockSize_; ++i) {
			if (newX != x || newY != y) {
				int n;
				istringstream iss(board_[newY][newX]);
				iss >> n;
				if (n == choice) {
					type = IN_BLOCK;
					return true;
				}
			}
			++newX;
			if (newX % blockSize_ == 0) {
				newX -= blockSize_;
			}
		}

		++newY;
		if (newY % blockSize_ == 0) {
			newY -= blockSize_;
		}
	}

	type = NONE;
	return false;
}

int Sudoku::prompt(unsigned int & row, unsigned int & col, unsigned int & choice) {

	//reading input
	string entry;

	cout << "Enter coordinates \"x,y\"; or \"quit\" to end game: ";
	cin >> entry;
	cout << endl;

	int x = 0, y = 0, val = 1;

	while (true) {
		bool reenter = false;

		if (entry == "quit") {
			return USER_QUIT;
		}

		size_t comPlace = entry.find(',');
		if (comPlace == string::npos) {
			reenter = true;
		}
		else {
			entry[comPlace] = ' ';

			istringstream iss(entry);
			iss >> x >> y;

			if ((x < leftBound_ || x > rightBound_) || (y < botBound_ || y > topBound_)) {
				cout << "Entry needs to be with bounds " << leftBound_ << " <= x <= " << rightBound_
					<< ", and " << botBound_ << " <= y <= " << topBound_ << "." << endl;
				reenter = true;
			}
			else if (preexisting_[y][x]) {
				cout << "You can't change this one! It was here from the start. Try Again!: ";
				cin >> entry;
				cout << endl;
			}
			else {
				//getting number to place in cell
				//coincidentally a number in the interval (botBound_, size]

				cout << "Enter a number place in this cell; or \"quit\" to end game: ";
				cin >> entry;
				cout << endl;

				while (true) {

					if (entry == "quit") {
						return USER_QUIT;
					}

					istringstream iss2(entry);
					iss2 >> val;


					if (val <= botBound_ || val > size_) {
						cout << "Number placed must be within bounds " << botBound_ << " < n <= " << size_
							<< ". Try again!: ";
						cin >> entry;
						cout << endl;
					}
					else {

						Invalidity_Type type;

						if (invalid(x, y, val, type)) {
							string sameWhat;

							switch (type) {
							case IN_ROW:
								sameWhat = "row";
								break;
							case IN_COLUMN:
								sameWhat = "column";
								break;
							case IN_BLOCK:
								sameWhat = "block";
							}

							cout << "You can't place that number. It's already in that " << sameWhat << ". Try Again!: ";
							cin >> entry;
							cout << endl;
						}
						else {
							break;
						}
					}

				}

				break;
			}

			if (reenter) {
				cout << "Entry invalid! Try Again, or enter \"quit\" to end game: ";
				cin >> entry;
				cout << endl;
				reenter = false;
			}
		}
	}

	//returning coordinates and choice
	row = x;
	col = y;
	choice = val;

	//updating longestIconSize_
	if (to_string(choice).size() > longestIconSize_) {
		longestIconSize_ = to_string(choice).size();
	}

	return SUCCESS;
}

int Sudoku::turn() {

	unsigned int choiceX, choiceY, choiceVal;

	if (prompt(choiceX, choiceY, choiceVal) == USER_QUIT) {
		cout << "You quit the game! " << turns_ << " turns were taken. " << endl;

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
				ofstream savefile("Sudoku.save");

				savefile << "Sudoku";	//writing gametype to save

				savefile << "\n";

				//writing game data

				savefile << turns_ << " ";	//number of turns taken
				savefile << longestIconSize_;	//size of longest icon
					
				savefile << "\n";

				//writing board state to file
				for (int x = 0; x < size_; ++x) {
					for (int y = 0; y < size_; ++y) {
						string in = board_[x][y];
						in = (in == empty_) ? saveEmpty_ : in;
						savefile << in << " ";
					}
				}

				savefile << "\n";

				//writing markers for preexisting numbers
				for (int x = 0; x < size_; ++x) {
					for (int y = 0; y < size_; ++y) {
						bool yes = preexisting_[x][y];
						string in = (yes) ? exSaveMarker_ : nonExSaveMarker_;
						savefile << in << " ";
					}
				}

				savefile.close();
			}
			else {
				ofstream savefile("Sudoku.save");

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

	board_[choiceY][choiceX] = to_string(choiceVal);

	++turns_;

	return SUCCESS;
}

int Sudoku::play() {
	bool playing = true;

	while (playing) {
		print();

		if (turn() == USER_QUIT) {
			return USER_QUIT;
		}

		if (done()) {
			print();
			cout << "You has won! Congrats!" << endl;

			//creating new save with no game data
			try {
				ofstream savefile("Sudoku.save");
				savefile << "NO DATA";
				savefile.close();
			}
			catch (ofstream::failure & e) {
				cout << "Failed closing save file! Save not completed!" << endl;
				return FILE_WRITE_ERROR;
			}

			break;
		}
	}

	return SUCCESS;
}