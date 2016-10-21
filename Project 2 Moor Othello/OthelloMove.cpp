#include "OthelloMove.h"
#include "OthelloBoard.h"
#include "OthelloExceptions.h"
#include <sstream>

using namespace std;

int OthelloMove::mOnHeap = 0;

// Default constructor
// Initializes move as a pass
OthelloMove::OthelloMove() : mRow(-1), mCol(-1) {

}

// Constructor with two arguments
// Initializes move with a row and col
OthelloMove::OthelloMove(int row, int col) : mRow(row), mCol(col) {

}

// Line limit: 11
// Assigns a string (r,c) to an OthelloMove object
OthelloMove& OthelloMove::operator=(const string& input) {

	// if pass, don't do anything
	if (input == "Pass" || input == "pass") {
		mRow = -1, mCol = -1;
	}
	else {
		// use istringstream to parse the input string into two ints
		istringstream inputReader(input);
		char buffer;

		// Parse the string for the two integers
		inputReader >> buffer >> mRow >> buffer >> mCol;

		// If the move is not in bounds, throw exception
		if (!OthelloBoard::InBounds(mRow, mCol))
			throw OthelloException("Out of bounds.");
	}

	return *this;   // 9 lines
}

// Overloaded Comparison operator
bool operator==(const OthelloMove &lhs, const OthelloMove &rhs) {
	return (lhs.mRow == rhs.mRow) && (lhs.mCol == rhs.mCol);
}

// Line limit: 5
// string casting overloaded operator
OthelloMove::operator std::string() const {
	if (IsPass())
		return "pass";

	return "(" + to_string(mRow) + "," + to_string(mCol) + ")";   // 3 lines
}