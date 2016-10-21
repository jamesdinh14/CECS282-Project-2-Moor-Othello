#include "OthelloExceptions.h"
#include "OthelloBoard.h"
#include "OthelloView.h"
#include "OthelloMove.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

// Line limit: 80
int main(int argc, char* argv[]) {
	// Initialization
	OthelloBoard board; // the state of the game board
	OthelloView v(&board); // a View for outputting the board via operator<<
	string userInput; // a string to hold the user's command choice
	vector<OthelloMove *> possMoves; // a holder for possible moves

	/*
	// Start with this DEBUGGING CODE to make sure your basic OthelloMove and 
	// OthelloBoard classes work, then remove it when you are ready to work
	// on the real main.
	cout << "Initial board:" << endl;
	cout << v << endl;
	OthelloMove *m = board.CreateMove();
	*m = "(3, 2)";
	cout << "Applying the move " << (string)(*m) << endl;
	board.ApplyMove(m);
	cout << endl << v << endl; // should show a changed board.

	m = board.CreateMove();
	*m = "(4, 2)";
	cout << "Applying the move " << (string)(*m) << endl;
	board.ApplyMove(m);
	cout << endl << v << endl;

	m = board.CreateMove();
	*m = "(5, 2)";
	cout << "Applying the move " << (string)(*m) << endl;
	board.ApplyMove(m);
	cout << endl << v << endl;
	// END OF DEBUGGING CODE
	*/

	// Main loop
	do {
		// Print the game board using the OthelloView object
		cout << v << endl;

		//typedef to make readability easier on the for loops
		typedef vector<OthelloMove *>::iterator vectorItr;
		typedef vector<OthelloMove *>::const_iterator vectorcItr;
		typedef vector<OthelloMove *>::const_reverse_iterator crItr;

      // Ask to input a command
      if (board.GetNextPlayer() == 1)
         cout << "Black's turn." << endl;
      else
         cout << "White's turn." << endl;

		// Print all possible moves
		board.GetPossibleMoves(&possMoves);

		cout << "Possible moves: " << endl;
		for (vectorcItr itr = possMoves.cbegin(); itr != possMoves.cend(); itr++)
			cout << (string)**itr << " ";
		cout << endl;

      // isValid represents a valid command and move
      // prevents possible moves list from displaying too many times
      bool isValid;

		// Command loop:
		do {   // 20 lines

         // Get input
			cout << "Enter a command: ";
			getline(cin, userInput);

         // default loop condition
         isValid = false;

			// move (r,c)
			if (userInput.find("move") == 0) {
				OthelloMove *move = board.CreateMove();
            const int substrStart = 5, substrLength = 5;

            // operator= can throw OthelloException when (r,c) is out of bounds
            // When exception is thrown, free up memory
				try {
               // OthelloMove is assigned to input (r,c)
               *move = userInput.substr(substrStart, substrLength);

					// Check to make sure inputted move is in list of possible moves
               // If it is, it is a valid command and the move is applied
               // else don't apply the move and free memory
					for (vectorcItr itr = possMoves.cbegin();
                  itr != possMoves.cend(); itr++) {
						if (*move == **itr) {
							board.ApplyMove(move);
							isValid = true;
							break;
						}
					}

					if (!isValid) {
						cout << "Invalid move." << endl;
						delete move;
					}
				}
				catch (OthelloException &oe) {
					cout << oe.what() << endl;
					delete move;  // 40 lines
				}
			}

			// undo n
         // Valid command
			if (userInput.find("undo") == 0) {  
				istringstream iss(userInput);
				string buffer;
				int numOfUndos = 0;

            // Obtain the int n from the input
				iss >> buffer >> numOfUndos;

				// Run undo n times
				// If n > move history's size, return to beginning
				while (numOfUndos > 0 && !board.GetMoveHistory()->empty()) {
					board.UndoLastMove();
					numOfUndos--;
				}
            isValid = true;
			}

			// showValue
         // Shows the current value of the board
			if (userInput == "showValue")   // 50 lines
				cout << "\nValue of the board: " << board.GetValue() << endl;

			// showHistory
         // shows the move history, displaying the most recent one first
			if (userInput == "showHistory") {
				char player = board.GetNextPlayer();

				// Display the history backwards (most recent move first)
				for (crItr itr = board.GetMoveHistory()->crbegin();
					itr != board.GetMoveHistory()->crend(); itr++) {
					// last player's move is the opposite of next player
					player = -player;
					cout << (player == 1 ? "Black: " : "White: ") <<
						(string)**itr << endl;
				}
			}

			// quit
         // valid command
			if (userInput == "quit") {
            int passCount = 0;
            const int finishedPassCount = 2;

            // If the previous move was a pass, only apply one pass
            // Else apply two
            if (!board.GetMoveHistory()->empty()) {
               if (board.GetMoveHistory()->back()->IsPass()) {
                  passCount = 1;
               }
            }

            // Apply Pass moves to finish the game
            for (int i = 0; i < (finishedPassCount - passCount); i++)
               board.ApplyMove(board.CreateMove());

            isValid = true;
			}

		} while (!isValid);

      // Free memory from possMoves and clear the list
		for (vectorItr itr = possMoves.begin(); itr != possMoves.end(); itr++)
			delete *itr;   // 70 lines
		possMoves.clear();   

	} while (!board.IsFinished());

   // Determine winner with the board's value
	if (board.GetValue() > 0)
		cout << "\nBlack wins!" << endl;
	else if (board.GetValue() < 0)
		cout << "\nWhite wins!" << endl;
	else
		cout << "\nThe game is a tie!" << endl;

	// Clean up memory
	while (!board.GetMoveHistory()->empty())
		board.UndoLastMove();  // 80 lines
}