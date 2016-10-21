#include "OthelloExceptions.h"
#include "OthelloBoard.h"
#include "OthelloView.h"
#include "OthelloMove.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;


int main(int argc, char* argv[]) {
   // Initialization
   OthelloBoard board; // the state of the game board
   OthelloView v(&board); // a View for outputting the board via operator<<
   string userInput; // a string to hold the user's command choice
   vector<OthelloMove *> possMoves; // a holder for possible moves

   
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





   // Main loop
   do {
	   // Print the game board using the OthelloView object

      // Print all possible moves

      // Ask to input a command

      // Command loop:
         // move (r,c)
         // undo n
         // showValue
         // showHistory
         // quit
      
   } while (!board.IsFinished()); 
}