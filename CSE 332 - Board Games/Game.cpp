#include "Game.h"
#include "TicTacToe.h"
#include "Gomoku.h"
#include "Sudoku.h"

shared_ptr<Game> Game::game_ptr = nullptr;

int Game::prompt(unsigned int & row, unsigned int & col) {
	string entry;

	cout << "Enter coordinates \"x,y\"; or \"quit\" to end game: ";
	cin >> entry;
	cout << endl;

	int x = 0, y = 0;

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
					<< ", and " << botBound_ <<" <= y <= " << topBound_ << "." << endl;
				reenter = true;
			}
			else {
				break;
			}
		}

		if (reenter) {
			cout << "Entry invalid! Try Again, or enter \"quit\" to end game: ";
			cin >> entry;
			cout << endl;
			reenter = false;
		}
	}

	//returning coordinates
	row = x;
	col = y;

	//updating longestIconSize_ accordingly
	if (is1sTurn_ && (iconP1_.size() > longestIconSize_)) {
		longestIconSize_ = iconP1_.size();
	}
	else if (!is1sTurn_ && (iconP2_.size() > longestIconSize_)) {
		longestIconSize_ = iconP2_.size();
	}

	//adding move to player's set
	playerMove m;
	m.x = x;
	m.y = y;

	if (is1sTurn_) {
		p1Moves.push_back(m);
	}
	else {
		p2Moves.push_back(m);
	}

	return SUCCESS;
}

void Game::getGame(int len, char* args[]) {
	
	if (game_ptr != nullptr) {
		throw GAME_PTR_WRONGLY_SET;
	}

	try {
		//if program was run with 1 argument
		if (len == 2) {
			string gameType = args[GAME_TYPE];

			//creating new Tic Tac Toe game
			if (gameType == "TicTacToe") {
				game_ptr = make_shared<TicTacToe>();
			}
			//creating new Gomoku game
			else if (gameType == "Gomoku") {
				game_ptr = make_shared<Gomoku>();
			}
			//creating new Sudoku game
			else if (gameType == "Sudoku") {
				game_ptr = make_shared<Sudoku>();
			}
			else {
				throw usage(args[PROGRAM_NAME], INVALID_ARGUEMENT);
			}
		}
		else {
			throw usage(args[PROGRAM_NAME], NEED_ONE_ARG);
		}
	}
	catch (bad_alloc & e) {
		throw e;
	}
	catch (int error) {
		throw error;
	}
}

shared_ptr<Game> Game::instance() {
	//check if pointer is null
	if (game_ptr == nullptr) {
		throw GAME_PTR_NULL;	//if so throw this exception
	}

	//Otherwise return pointer
	return game_ptr;
}