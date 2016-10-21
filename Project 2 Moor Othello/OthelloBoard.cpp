#include "OthelloBoard.h"

// Line limit: 10 
// Default constructor that initializes the starting state of the board
OthelloBoard::OthelloBoard() : mValue(0), mNextPlayer(BLACK) {
	const int INITIAL_COORD_1 = 3, INITIAL_COORD_2 = 4;

	// initialize the board with empty spaces
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			mBoard[i][j] = EMPTY;

	// Initialize first four pieces on the board
	mBoard[INITIAL_COORD_1][INITIAL_COORD_1] = WHITE;
	mBoard[INITIAL_COORD_2][INITIAL_COORD_2] = WHITE;
	mBoard[INITIAL_COORD_1][INITIAL_COORD_2] = BLACK;
	mBoard[INITIAL_COORD_2][INITIAL_COORD_1] = BLACK;
}

// Line limit: 30 
// Fill the list with valid moves that the user must enter
void OthelloBoard::GetPossibleMoves(std::vector<OthelloMove *> *list) const {

	// Step through the board to determine possible moves
	for (int row = 0; row < BOARD_SIZE; row++) {
	   for (int col = 0; col < BOARD_SIZE; col++) {

			// All possible moves will be on currently empty spots
	      if (mBoard[row][col] == EMPTY) {

				// Boolean variable to mark if a move is valid or not
				bool isValid = false;

				// 8-directional checking
				for (int rowDelta = -1; rowDelta <= 1; rowDelta++) {
					for (int colDelta = -1; colDelta <= 1; colDelta++) {

						// If the check goes out-of-bounds, don't run
						if (InBounds(row + rowDelta, col + colDelta)) {

							// Check for enemy piece one space away
							// If the deltas = 0, don't need to run
							if (mBoard[row + rowDelta][col + colDelta] == -mNextPlayer
								&& !(rowDelta == 0 && colDelta == 0)) {

								// Move over to the enemy piece
								// Resets value of i and j on future checks
								int i = row + rowDelta;
								int j = col + colDelta;   // 11 lines

								// As long as there is an enemy piece
								// and the check remains in bounds
								// Keep moving in that direction
								do {
									i += rowDelta;
									j += colDelta;

									// When an ally piece is found,
									// This is a valid move
									if (mBoard[i][j] == mNextPlayer && InBounds(i, j)) {
										isValid = true;
										break;
									}
								} while (mBoard[i][j] == -mNextPlayer);   // 18 lines
							}
						}
					}
				}

				// As long as the move flips at least one piece in at least one
				// direction, it is a valid move, so add it to the list
				// Doing it outside the 8-direction check block prevents duplicates
				if (isValid) {
					list->push_back(new OthelloMove(row, col));
				}
			}
		}
	}

   // If the list is empty, the list should contain only a "Pass"
   if (list->empty())
      list->push_back(CreateMove());   // 22 lines
}

// Line limit: 30 
// Run ApplyMove if "pass" or one of possible moves
// Place Othello piece and flip pieces where necessary
void OthelloBoard::ApplyMove(OthelloMove *move) {

   // Save applied move in history
   mHistory.push_back(move);

	// If move is pass, don't run the apply
   // and increment pass count
   // Else, reset pass count to 0
   if (move->IsPass()) 
      mPassCount++;
   else {
      mPassCount = 0;

      // place piece
      mBoard[move->mRow][move->mCol] = mNextPlayer;

      // totalValueDelta will keep a running total of the change in value
      // initialized to 1 because of the initial move
      // valueDelta represents the value change every time a piece is flipped
      int totalValueDelta = 1;
      int valueDelta = 2;

      // Delta's allow for 8-directional check
      for (int rowDelta = -1; rowDelta <= 1; rowDelta++) {
         for (int colDelta = -1; colDelta <= 1; colDelta++) {

            // Move over to the enemy piece
            // Resets value of i and j on future checks
            int i = move->mRow + rowDelta;
            int j = move->mCol + colDelta;

            // If the check goes out-of-bounds, don't run
            // Check for enemy piece one space away
            // If the deltas = 0, don't need to run
            if (InBounds(i, j) && mBoard[i][j] == -mNextPlayer
               && !(rowDelta == 0 && colDelta == 0)) {

               // As long as there is an enemy piece
               // and the check remains in bounds
               // Keep moving in that direction
               do {
                  i += rowDelta;
                  j += colDelta;

                  // flipcount keeps track of number of pieces flipped
                  char flipCount = 0;

                  // When an ally piece is found,
                  // Backtrack and flip pieces
                  if (mBoard[i][j] == mNextPlayer && InBounds(i, j)) {

                     // Flip the pieces
                     // Each piece flipped changes the board's value by 2
                     while (mBoard[i -= rowDelta][j -= colDelta]
                        == -mNextPlayer) {
                        mBoard[i][j] = mNextPlayer;
                        flipCount++;
                        totalValueDelta += valueDelta;
                     }

                     // Save the FlipSet into the passed OthelloMove
                     move->AddFlipSet(OthelloMove::
                        FlipSet(flipCount, rowDelta, colDelta));
                  }
               } while (mBoard[i][j] == -mNextPlayer && InBounds(i, j));

            }
         }
      }

      // valueDelta keeps a running total of the initial placement
      // and all the flips that occurred afterwards
      // If the current player is White, change totalValueDelta to negative
      if (mNextPlayer == WHITE)
         totalValueDelta = -totalValueDelta;

      // Update value
      mValue += totalValueDelta;
   }

	// Change player turns
	mNextPlayer = -mNextPlayer; // 30 lines
}

// Line limit: 22
// Undoes a move and returns the board to its previous state
void OthelloBoard::UndoLastMove() {

   // Get the most recent move (the one that will be undone)
   OthelloMove *undoMove = mHistory.back();

   // Remove the piece that was placed
   mBoard[undoMove->mRow][undoMove->mCol] = EMPTY;

   // totalValueDelta will keep a running total of the change in value
   // initialized to 1 because of the initial move
   // valueDelta represents the value change every time a piece is flipped
   int totalValueDelta = 1;
   int valueDelta = 2;

   // Use FlipSet to return all flipped pieces of the move
   while (!undoMove->mFlips.empty()) {
      // direction variables
      int i = undoMove->mRow + undoMove->mFlips.back().rowDelta;
      int j = undoMove->mCol + undoMove->mFlips.back().colDelta;

      // Move in direction specified by (rowDelta,colDelta)
      // and flip pieces equal to number of switched
      for (int count = 0; count < undoMove->mFlips.back().switched; count++) {
         mBoard[i][j] = mNextPlayer;

         i += undoMove->mFlips.back().rowDelta;
         j += undoMove->mFlips.back().colDelta;

         totalValueDelta += valueDelta;
      } 

      // After one FlipSet is done, remove it from the vector and continue
      undoMove->mFlips.pop_back();
   }

   // Adjust value
   // If black is undoing his move aka next player is White
   // totalValueDelta should be negative
   if (mNextPlayer == WHITE)
      totalValueDelta = -totalValueDelta;

   // Update value
   mValue += totalValueDelta;

   // Switch nextPlayer
   mNextPlayer = -mNextPlayer;

   // free move's memory
   delete undoMove;

   // remove the move from board's move history
   mHistory.pop_back();   // 19 lines
}