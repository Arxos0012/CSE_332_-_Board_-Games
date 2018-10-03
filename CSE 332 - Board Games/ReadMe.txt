Jabari Booker

Errors and Warnings
	-warning for signed/unsigned comparisons when comparing integers to size (unsigned) values
	-warning for unreferenced local variables. In each of these case it is an exception that is caught

Design
	-Sudoku's draw() method
		Sudoku is not a competative game. By this I mean that there is one player who plays to solve a puzzle rather than
		two players that are competeing to will. Therefore, the game has no draw state, only a complete and an incomplete
		state. Because of this, for the Sudoku class I defined the draw() method to always return false. It is never used
		by any code in the project, but I kept its definition in the case that someone would want to redefine the method 
		later.
	
	-Sudoku's done() method
		Oddly enough a Sudoku puzzle will only be done (completed) when all of its cells contain a number. Therefore, I
		only check to see that all cells contain a number when check to see game is done.
	
	-Sudoku's prompt() and invalid() methods
		Beacuse the means of play for Sudoku are different from those of TicTacToe and Gomoku, the prompt() function
		needed to be redefined within the Sudoko class.
		
		Here, I designed the prompt function to ask the player for a coordinate in which to place a number and the number
		to place. They are informed that they cannot place the number specified if that number is already present within
		the same column, row, or 3x3 block of that space (as per the rules of Sudoku) and are also told to chose another
		number to place.
		
		The invalid() method is a helper method that does the actual work or determining if that number violate the rules
		of the game. Search a row or column of the board is fairly easy (simply traversing through the row/column until
		you find that number), but search a block was more complicated. What I chose to do was read a row of the block 
		from left to right, from the bottom row to the top. To stay inside the block, I checked if the next space I would
		anaylze would be outside of the block. If so, I would go to the space on the opposite end of that row or column of
		the block. The invalid() method also sets an enumerated type that is used to described exaclty why the player can't
		place their selected number in the space chosen. The method returns true if move is invalid according to the rules
		of Sudoku and false otherwise.
	
	-Dynamically allocating space for the board
		Since there are two types of board, with different sizes, the board cannot be initialized to any predictable
		dimensions (can't declare the board as a 5x5 array of strings for Tic Tac Toe when Gomoku requires more space;
		creating a 19x19 board to used for both games would mean we allocate more space than is necessary for Tic Tac Toe).
		This remedy this, in each derived class's constructors, the board is dynamically allocated to the game's
		specifications and then deleted in the destructors of the classes.

		
**Copy Control Design**
	None of the game classes have copy or move constructors, or  copy-assignment or move-assignment operators. This is
	because I did not see a need to implement any of these since objects of type TicTacToe, Gomoku, and Sudoku do not have
	to be copied or moved in any part of the code.
	
	All of these classes do have a destructor, in which the memory used to store the board is deallocated (in the case of 
	Sudoku, the memory used to keep track of the cells that had numbers at the beginning of the game is also deallocated).
	This was to free up memory on the heap that is no longer needed.
	
		
Possible Error Messages Returned by the Program
	-"Failed to allocate memory for game: <reasoning>"
		Appears when the program fails to allocate memory to create the game board, or if the program fails to
		allocate space for a game object to run initially.
	
	-"Neither player won...with a total of <turns> turns...sad..."
		Appears when all spaces on the board are filled and there are no more vaild moves left
		(TicTacToe and Gomoku only)
		
	-"You(Player <player> has) quit the game! <turns> turns were taken.
	  Would you like to save the game though (yes/no)?"
		Appears when a player has entered "quit" and quits the game before completing it
	
	-"Would you like to save the game though (yes/no)?"
		Appears when a player commits to quitting a game. Gives them a chance to save the game.
	
	-"Failed opening/closing file!"
		Appears when the program fails to open, read, and close a savefile for the game or a start data file (for Sudoku).
	
	-"Failed closing save file! Save not completed!"
		Appears when the program fails to open, write, and close a savefile for the game.
	
	-"One and only one argument must be passed!"
		Appears when the program is run with either no arguments or more than one (ONLY one should be passed!)
	
	-"Invalid arguement passed! Valid arguements are: <valid arguements>"
		Appears when the program is run with 1 argument but it is not a vaild arguement. Valid arguements are also
		printed
	
	-"usage: <program name> <game_arg>"
		Appears anytime the incorrect number of arguements or invalid arguements are run with program

Testing
	General:
		
		-Case 0: "Gomoku" is passed as the program argument
			The program starts a game of Gomoku as it should.
			
		-Case 1: "TicTacToe" is passed as the program argument
			The program starts a game of Tic Tac Toe as it should.
		
		-Case 2: "Sudoku" is passed as the program arguement
			The program starts a game of Sudoku as it should.
		
		-Case 3: An invalid coordinate is entered
			The program will prompt the user to either try entering a valid coordinate again (displaying the correct
			format and bounds for vaild moves) or to quit the game.
		
		-Case 4: A player tries to place a piece in a spot where there already exists one
			The program will tell the user that piece is already there and to either enter a vaild move for another
			spot or quit the game (TicTacToe and Gomoku only).
			
		-Case 5: A player quits
			The game ends, displaying the number of turns taken.
		
		-Case 6: A player enters a valid move
			The game then places a piece at the specified space and continues to the next player's turn
		
		-Case 7: A draw is reached
			In this case, all the spaces on the board are filled. So the game ends, displaying the total number of
			turns taken
		
		-Case 8: A player wins
			The player that won is notified, with the number of turns that were played in the game, and the game
			ends.
		
	TicTacToe
		
		-Case 0: A full row, column, or diagonal is matched by one of the players
			That player wins the game (see General Case 7).
		
		-Case 1: There are no matching rows, columns, or diagonals, yet there are no more valid moves left
			The game ends in a draw (see General Case 6).
		
	Gomoku:
		
		-Case 0: A row, column, or diagonal of 5 is matched by one of the players somewhere on the board
			That player wins the game (see General Case 7).
		
		-Case 1: There are no matching rows, columns, or diagonals of 5 anywhere on the board, yet there are 
		         no more valid moves left
			The game ends in a draw (see General Case 6).
	
	Sudoku:
		
		-Case 0: An invalid value to place in a cell is entered
			The game tells the player the range in which the number must be and prompts them to try again with a 
			different value.
		
		-Case 1: The player tries to place a number in a row/column/block where that number already exists
			The game tells the player that number already exists in that row/column/block and prompts the user to try 
			again.
		
		-Case 2: The player tries to enter a coordinate where a number has existed since the start of the gamae
			The game tells the player that the pre-existing number cannot be changed (by the rules of Sudoku) and
			to try again.
			