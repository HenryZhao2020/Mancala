#ifndef BOARD_H
#define BOARD_H

#include "PCH.h"

class Game;
class GameBar;
class Pocket;

class Board : public QFrame {
    Q_OBJECT

public:
    Board(Game *game);

    void movePlayer(int start);
    void moveComputer();
    void setPlayerTurn(bool turn);
    void updateAmounts();

private:
    Game *game;
    GameBar *gameBar;
    Pocket *pockets[14];
    bool playerTurn;

    int nextComputerIndex();
    bool isPlayerEnded();
    bool isComputerEnded();
    void makeEnded();
};

#endif
