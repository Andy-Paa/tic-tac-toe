#include "TicTacToe.h"

// -----------------------------------------------------------------------------
// TicTacToe.cpp
// -----------------------------------------------------------------------------
// This file is intentionally *full of comments* and gentle TODOs that guide you
// through wiring up a complete Tic‑Tac‑Toe implementation using the game engine’s
// Bit / BitHolder grid system.
//
// Rules recap:
//  - Two players place X / O on a 3x3 grid.
//  - Players take turns; you can only place into an empty square.
//  - First player to get three-in-a-row (row, column, or diagonal) wins.
//  - If all 9 squares are filled and nobody wins, it’s a draw.
//
// Notes about the provided engine types you'll use here:
//  - Bit              : a visual piece (sprite) that belongs to a Player
//  - BitHolder        : a square on the board that can hold at most one Bit
//  - Player           : the engine’s player object (you can ask who owns a Bit)
//  - Game options     : let the mouse know the grid is 3x3 (rowX, rowY)
//  - Helpers you’ll see used: setNumberOfPlayers, getPlayerAt, startGame, etc.
//
// I’ve already fully implemented PieceForPlayer() for you. Please leave that as‑is.
// The rest of the routines are written as “comment-first” TODOs for you to complete.
// -----------------------------------------------------------------------------

const int AI_PLAYER   = 1;      // index of the AI player (O)
const int HUMAN_PLAYER= 0;      // index of the human player (X)

TicTacToe::TicTacToe()
{
}

TicTacToe::~TicTacToe()
{
}
//here I listed 2 new method for simulated prediction instead of using the real checking one
int TicTacToe::checkWinnerFromState(const std::string &state) {
    //8 ways to win
    const int winningCombos[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8}, //rows
        {0,3,6}, {1,4,7}, {2,5,8}, //cols
        {0,4,8}, {2,4,6}           //diagonals
    };

    for (int i = 0; i < 8; i++) {
        int a = winningCombos[i][0];
        int b = winningCombos[i][1];
        int c = winningCombos[i][2];

        if (state[a] != '0' && state[a] == state[b] && state[a] == state[c]) {
            //translate the index
            return (state[a] - '1'); 
        }
    }
    return -1; //draw
}

bool TicTacToe::isDrawState(const std::string &state) {
    //if '0' still exists, not draw
    for (int i = 0; i < 9; i++) {
        if (state[i] == '0') return false;
    }
    //no '0' and no winner
    return (checkWinnerFromState(state) == -1);
}

int TicTacToe::negamax(const std::string &state, int depth, int index) {
    int winner = checkWinnerFromState(state);
    if (winner == AI_PLAYER) return index * 1;
    if (winner == HUMAN_PLAYER) return index * -1;
    if (isDrawState(state)) return 0;

    int bestScore = -9999;//set a low score
    for (int i = 0; i < 9; i++) {
        if (state[i] == '0') {
            std::string newState = state;
            newState[i] = (index == 1 ? (AI_PLAYER+1+'0') : (HUMAN_PLAYER+1+'0'));
            int score = -negamax(newState, depth+1, -index);//negate the score
            bestScore = std::max(bestScore, score);
        }
    }
    return bestScore;
}

// -----------------------------------------------------------------------------
// make an X or an O
// -----------------------------------------------------------------------------
// DO NOT CHANGE: This returns a new Bit with the right texture and owner
Bit* TicTacToe::PieceForPlayer(const int playerNumber)
{
    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit *bit = new Bit();
    bit->LoadTextureFromFile(playerNumber == 1 ? "x.png" : "o.png");
    bit->setOwner(getPlayerAt(playerNumber));
    return bit;
}

//
// setup the game board, this is called once at the start of the game
//
void TicTacToe::setUpBoard()
{
    // here we should call setNumberOfPlayers to 2 and then set up the game options so the mouse knows to draw a 3x3 grid
    setNumberOfPlayers(2);
    // _gameOptions has a rowX and rowY property we should set to 3
    _gameOptions.rowX = 3;
    _gameOptions.rowY = 3;
    // then we need to setup our 3x3 array in _grid with the correct position of the square, and load the "square.png" sprite for each square
    // we will use the initHolder function on each square to do this
    for (int y = 0; y < 3; y++) 
    {
        for (int x = 0; x < 3; x++) 
        {
            ImVec2 position = ImVec2(70 + x * 100, 70 + y * 100);//position
            _grid[y][x].initHolder(position, "square.png", 3, 3);//initHolder
        }
    }
    // finally we should call startGame to get everything going
    startGame();
}

//
// about the only thing we need to actually fill out for tic-tac-toe
//
bool TicTacToe::actionForEmptyHolder(BitHolder *holder)
{
    // 1) Guard clause: if holder is nullptr, fail fast.
    //    (Beginner hint: always check pointers before using them.)
    if (!holder) return false;
    
    // 2) Is it actually empty?
    //    Ask the holder for its current Bit using the bit() function.
    //    If there is already a Bit in this holder, return false.
    if (holder->bit() != nullptr) return false;

    // 3) Place the current player's piece on this holder:
    //    - Figure out whose turn it is (getCurrentPlayer()->playerNumber()).
    //    - Create a Bit via PieceForPlayer(currentPlayerIndex).
    //    - Position it at the holder's position (holder->getPosition()).
    //    - Assign it to the holder: holder->setBit(newBit);
    int currentPlayerIndex = getCurrentPlayer()->playerNumber();
    Bit* newBit = PieceForPlayer(currentPlayerIndex);
    newBit->moveTo(holder->getPosition());
    holder->setBit(newBit);

    // 4) Return whether we actually placed a piece. true = acted, false = ignored.
    return true; // replace with true if you complete a successful placement    
}

bool TicTacToe::canBitMoveFrom(Bit *bit, BitHolder *src)
{
    // you can't move anything in tic tac toe
    return false;
}

bool TicTacToe::canBitMoveFromTo(Bit* bit, BitHolder*src, BitHolder*dst)
{
    // you can't move anything in tic tac toe
    return false;
}

//
// free all the memory used by the game on the heap
//
void TicTacToe::stopGame()
{
    // clear out the board
    // loop through the 3x3 array and call destroyBit on each square
    for (int y = 0; y < 3; y++) 
    {
        for (int x = 0; x < 3; x++) 
        {
            _grid[y][x].destroyBit();//clear existing bit删掉现有的棋
        }
    }
}

//
// helper function for the winner check
//
Player* TicTacToe::ownerAt(int index ) const
{
    // index is 0..8, convert to x,y using:
    // y = index / 3
    // x = index % 3 
    // if there is no bit at that location (in _grid) return nullptr
    // otherwise return the owner of the bit at that location using getOwner()
    int y = index / 3;
    int x = index % 3;

    const BitHolder& holder = _grid[y][x];//get holder
    Bit* bit = holder.bit();

    if (bit) {
        return bit->getOwner();//get owner
    }
    return nullptr;
}

Player* TicTacToe::checkForWinner()
{
    // check all the winning triples
    // if any of them have the same owner return that player
    // winning triples are:
    // 0,1,2
    // 3,4,5
    // 6,7,8
    // 0,3,6
    // 1,4,7
    // 2,5,8
    // 0,4,8
    // 2,4,6
    // you can use the ownerAt helper function to get the owner of a square
    // for example, ownerAt(0) returns the owner of the top-left square
    // if there is no bit in that square, it returns nullptr
    // if you find a winning triple, return the player who owns that triple
    // otherwise return nullptr

    // Hint: Consider using an array to store the winning combinations
    // to avoid repetitive code
    const int winningCombos[8][3] = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8},
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8},
        {0, 4, 8},
        {2, 4, 6}
    };
    for (int i = 0; i < 8; ++i) {//winning combos
        int a = winningCombos[i][0];
        int b = winningCombos[i][1];
        int c = winningCombos[i][2];

        Player* ownerA = ownerAt(a);
        Player* ownerB = ownerAt(b);
        Player* ownerC = ownerAt(c);

        if (ownerA && ownerA == ownerB && ownerA == ownerC) {
            return ownerA;
        }
    }
    return nullptr;
}

bool TicTacToe::checkForDraw()
{
    // is the board full with no winner?
    // if any square is empty, return false
    // otherwise return true
    for (int i = 0; i < 9; ++i) {
        if (ownerAt(i) == nullptr) { //empty square
            return false;
        }
    }
    return true;
}

//
// state strings
//
std::string TicTacToe::initialStateString()
{
    return "000000000";
}

//
// this still needs to be tied into imguis init and shutdown
// we will read the state string and store it in each turn object
//
std::string TicTacToe::stateString() const
{
    // return a string representing the current state of the board
    // the string should be 9 characters long, one for each square
    // each character should be '0' for empty, '1' for player 1 (X), and '2' for player 2 (O)
    // the order should be left-to-right, top-to-bottom
    // for example, the starting state is "000000000"
    // if player 1 has placed an X in the top-left and player 2 an O in the center, the state would be "100020000"
    // you can build the string using a loop and the to_string function
    // for example, to convert an integer to a string, you can use std::to_string(1) which returns "1"
    // you can get the bit at each square using _grid[y][x].bit()
    // if the bit is not null, you can get its owner using bit->getOwner()->playerNumber()
    // remember that player numbers are zero-based, so add 1 to get '1' or '2'
    // if the bit is null, add '0' to the string
    // finally, return the constructed string
    std::string state = "";
    for (int i = 0; i < 9; ++i) {
        Player* owner = ownerAt(i);
        if (owner) {
            state += std::to_string(owner->playerNumber() + 1);
        } else {
            state += "0";
        }
    }
    return state;
    // return "000000000";
}

//
// this still needs to be tied into imguis init and shutdown
// when the program starts it will load the current game from the imgui ini file and set the game state to the last saved state
//
void TicTacToe::setStateString(const std::string &s)
{
    // set the state of the board from the given string
    // the string will be 9 characters long, one for each square
    // each character will be '0' for empty, '1' for player 1 (X), and '2' for player 2 (O)
    // the order will be left-to-right, top-to-bottom
    // for example, the starting state is "000000000"
    // if player 1 has placed an X in the top-left and player 2 an O in the center, the state would be "100020000"
    // you can loop through the string and set each square in _grid accordingly
    // for example, if s[0] is '1', you would set _grid[0][0] to have player 1's piece
    // if s[4] is '2', you would set _grid[1][1] to have player 2's piece
    // if s[8] is '0', you would set _grid[2][2] to be empty
    // you can use the PieceForPlayer function to create a new piece for a player
    // remember to convert the character to an integer by subtracting '0'
    // for example, int playerNumber = s[index] - '0';
    // if playerNumber is 0, set the square to empty (nullptr)
    // if playerNumber is 1 or 2, create a piece for that player and set it in the square
    // finally, make sure to position the piece at the holder's position
    // you can get the position of a holder using holder->getPosition()
    // loop through the 3x3 array and set each square accordingly
    // the string should always be valid, so you don't need to check its length or contents
    // but you can assume it will always be 9 characters long and only contain '0', '1', or '2'
    for (int i = 0; i < 9; ++i) {
        int playerNumber = s[i] - '0';
        int y = i / 3;
        int x = i % 3;
        BitHolder& holder = _grid[y][x];
        holder.destroyBit(); //clear existing bit删掉现有的棋
        if (playerNumber == 1 || playerNumber == 2) {
            Bit* newBit = PieceForPlayer(playerNumber - 1);
            newBit->moveTo(holder.getPosition());//把棋子放到格子里
            holder.setBit(newBit);
        }
    }
}


//
// this is the function that will be called by the AI
//
void TicTacToe::updateAI() 
{
    if (getCurrentPlayer()->playerNumber() != AI_PLAYER) return; //double check

    std::string state = stateString();
    int bestScore = -9999;
    int bestMove = -1;

    for (int i = 0; i < 9; i++) {
        if (state[i] == '0') {
            std::string newState = state;
            //state for AI
            newState[i] = (AI_PLAYER + 1 + '0');  

            int score = -negamax(newState, 0, -1); //next turn
            if (score > bestScore) {
                bestScore = score;
                bestMove = i;
            }
        }
    }

    //do the move
    if (bestMove != -1) {
        actionForEmptyHolder(&_grid[bestMove/3][bestMove%3]);
        endTurn();
    }
    
}