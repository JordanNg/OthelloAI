/*
 * MinimaxPlayer.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: wong
 */
#include <iostream>
#include <string>
#include <assert.h>
#include <vector>
#include <algorithm>
#include "MinimaxPlayer.h"

using std::vector;
int maxValue(OthelloBoard b, char currentSymbol, char rootSymbol);
int minValue(OthelloBoard b, char currentSymbol, char rootSymbol);
int utility(OthelloBoard b, char rootSymbol);
void minimax_Decision(OthelloBoard *b, char rootSymbol, int& col, int& row);
vector<OthelloBoard> nextActions(OthelloBoard b, char symbol);
vector<OthelloBoard> nextMoves(OthelloBoard b, char symbol, struct move *cords);
char nextPlayerSym(char curSymbol);

struct move {
    int col;
    int row;
};

MinimaxPlayer::MinimaxPlayer(char symb) :
		Player(symb) {

}

MinimaxPlayer::~MinimaxPlayer() {

}

void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {

    // If this function is called that means it is currently its turn
    // Wants to maximize current symbol even if second
    char rootSymbol = get_symbol(); 
    
    // Then recursively generate the successors until only terminal states are reached
    // Call minimax_Decision()
    minimax_Decision(b, rootSymbol, col, row);

    // Choose the optimal play
}

void minimax_Decision(OthelloBoard *b, char rootSymbol, int& col, int& row) {
    // Make a copy of the current Othello Board so we don't make a move when iterating
    OthelloBoard newBoard = *b;

    struct move *cords = new struct move[16];

    // Generate a list of actions that can currently be taken
    vector<OthelloBoard> newBoards = nextMoves(newBoard, rootSymbol, cords);
 
    int greatestUtilPos = -1;
    int greatestUtil = -1;

    // For each of the actions possible determine which will yeild the highest value
    for (int i = 0; i < newBoards.size(); i++) {
        int result = minValue(newBoards.at(i), rootSymbol, rootSymbol);
        if (result > greatestUtil) {
            greatestUtil = result;
            greatestUtilPos = i;
        }
    }
    
    // Set the col and row values
    col = cords[greatestUtilPos].col;
    row = cords[greatestUtilPos].row;    
}

int maxValue(OthelloBoard b, char currentSymbol, char rootSymbol) {
    // Test if this is a terminal state
    // If the current piece has no moves remianing then return the value
    if (!b.has_legal_moves_remaining(currentSymbol)) {
        //return utility(b, rootSymbol); 
        char checkNext = nextPlayerSym(currentSymbol);
        if (!b.has_legal_moves_remaining(checkNext)) { // Then in a terminal state
            return utility(b, rootSymbol); 
        }
    }
        
    int v = -10000; 

    char nextSymbol = nextPlayerSym(currentSymbol);

    // A vector of OthelloBoards with corresponding actions
    vector<OthelloBoard> boards = nextActions(b, nextSymbol); 
    if (boards.empty()) {
        boards.push_back(b);
    }
    
    // For each action possible 
    for (int i = 0; i < boards.size(); i++) {
        // Set v to the min between the current v and the max v possible when taking an action
        v = std::max(v, minValue(boards.at(i), nextSymbol, rootSymbol));
    }

    return v; 
}

int minValue(OthelloBoard b, char currentSymbol, char rootSymbol) {
    // Test if this is a terminal state
    // If the current piece has no moves remianing then return the value
    if (!b.has_legal_moves_remaining(currentSymbol)) {
        //return utility(b, rootSymbol); 
        char checkNext = nextPlayerSym(currentSymbol);
        if (!b.has_legal_moves_remaining(checkNext)) { // In a terminal state
            return utility(b, rootSymbol); 
        }
    }
        
    int v = 10000; 

    char nextSymbol = nextPlayerSym(currentSymbol);

    // A vector of OthelloBoards with corresponding actions
    vector<OthelloBoard> boards = nextActions(b, nextSymbol); 
    if (boards.empty()) {
        boards.push_back(b);
    }
    
    // For each action possible 
    for (int i = 0; i < boards.size(); i++) {
        // Set v to the min between the current v and the max v possible when taking an action
        v = std::min(v, maxValue(boards.at(i), nextSymbol, rootSymbol));
    }

    return v; 
}

// Function to return the utility of a OthelloBoard 
/*int utility(OthelloBoard b, char rootSymbol) {
    // Count the current number of a symbol and return it
    int rootScore = b.count_score(rootSymbol);
    char nextSym = nextPlayerSym(rootSymbol);
    int otherScore = b.count_score(nextSym);

    return rootScore - otherScore;
}*/

// Function to return the utility of a OthelloBoard 
int utility(OthelloBoard b, char rootSymbol) {
    // Count the current number of a symbol and return it
    return b.count_score(rootSymbol);
}

// Helper function to enumerate all of the possible actions from a certain board
vector<OthelloBoard> nextActions(OthelloBoard b, char symbol) {
    vector<OthelloBoard> actions;
    // Enumerate all possible actions on current OthelloBoard by the next player
    int col = 0;
    int row = 0;
    for (col = 0; col < b.get_num_cols(); col++) {
        for (row = 0; row < b.get_num_rows(); row++) {
            // If in this cell the move by the current 
            if (b.is_legal_move(col, row, symbol)) {
                OthelloBoard newBoard = b;
                newBoard.play_move(col, row, symbol); 
                // Last col and row work here
                newBoard.set_lastCol(col);
                newBoard.set_lastRow(row);
                actions.push_back(newBoard);
            }
        }
    }

    return actions; 
}

// Helper function to enumerate all of the possible actions from a certain board
vector<OthelloBoard> nextMoves(OthelloBoard b, char symbol, struct move *cords) {
    vector<OthelloBoard> actions;
    // Enumerate all possible actions on current OthelloBoard by the next player
    int col = 0;
    int row = 0;
    int i = 0;
    for (col = 0; col < b.get_num_cols(); col++) {
        for (row = 0; row < b.get_num_rows(); row++) {
            // If in this cell the move by the current 
            if (b.is_legal_move(col, row, symbol)) {
                OthelloBoard newBoard = b;
                newBoard.play_move(col, row, symbol); 
                // Last col and row work here
                newBoard.set_lastCol(col);
                newBoard.set_lastRow(row);
                actions.push_back(newBoard);

                // Try an array
                cords[i].col = col;
                cords[i].row = row;
                i++;
            }
        }
    }

    return actions; 
}

char nextPlayerSym(char curSymbol) {
    char nextSymbol;
    // Determine the next move's symbol
    if (curSymbol == 'X') {
        nextSymbol = 'O';
    } else {
        nextSymbol = 'X';
    }
    return nextSymbol;
}

MinimaxPlayer* MinimaxPlayer::clone() {
	MinimaxPlayer* result = new MinimaxPlayer(symbol);
	return result;
}
