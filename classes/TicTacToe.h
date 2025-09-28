#pragma once
#include "Game.h"
#include "Square.h"

//
// the classic game of tic tac toe
//

//
// the main game class
//
class TicTacToe : public Game
{
public:
    TicTacToe();
    ~TicTacToe();
    //nagemax algorithm
    int negamax(const std::string &state,int depth, int color);

    // set up the board
    void        setUpBoard() override;
    //new method for simulation by using state
    bool isDrawState(const std::string &state);
    int checkWinnerFromState(const std::string &state);

    Player*     checkForWinner() override;
    bool        checkForDraw() override;
    std::string initialStateString() override;
    std::string stateString() const override;
    void        setStateString(const std::string &s) override;
    bool        actionForEmptyHolder(BitHolder *holder) override;
    bool        canBitMoveFrom(Bit*bit, BitHolder *src) override;
    bool        canBitMoveFromTo(Bit* bit, BitHolder*src, BitHolder*dst) override;
    void        stopGame() override;

	void        updateAI() override;
    bool        gameHasAI() override { return true; }
    BitHolder &getHolderAt(const int x, const int y) override { return _grid[y][x]; }
private:
    Bit *       PieceForPlayer(const int playerNumber);
    Player*     ownerAt(int index ) const;

    Square      _grid[3][3];
};

