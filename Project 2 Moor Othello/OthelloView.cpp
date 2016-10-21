#include "OthelloView.h"

using namespace std;

// Method to print a human-understandable Othello Board
void OthelloView::PrintBoard(ostream &out) const {

	// Labeling of the columns on top
   out << " -  0  1  2  3  4  5  6  7  " << endl;

   for (int row = 0; row < BOARD_SIZE; row++) {

		// Labeling of rows on left side
      out << " " << row << " ";

		for (int col = 0; col < BOARD_SIZE; col++) {

			// Replace 0, 1, -1 w/ game markers
			if (mOthelloBoard->mBoard[row][col] == OthelloBoard::Player::BLACK)
            out << " B ";

			if (mOthelloBoard->mBoard[row][col] == OthelloBoard::Player::WHITE)
            out << " W ";

			if (mOthelloBoard->mBoard[row][col] == OthelloBoard::Player::EMPTY)
            out << " . ";
		}

      // Make it look better
      out << endl;
	}
}

// Line limit: 2
// Overloaded << operator to enable "cout << OthelloView" statement
ostream& operator<< (ostream &out, const OthelloView &view) {
	view.PrintBoard(out);
	return out;
}