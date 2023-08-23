#include "Board.h"
#include "Game.h"
#include "GameBar.h"
#include "Pocket.h"
#include "Store.h"
#include "Attr.h"

Board::Board(Game *game) : QFrame(game) {
    this->game = game;
    gameBar = game->getGameBar();
    playerTurn = true;

    auto gridLayout = new QGridLayout(this);
    gridLayout->setHorizontalSpacing(10);
    gridLayout->setVerticalSpacing(120);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    pockets[0] = new Store(QString::number(Attr::amounts[0]), this);
    pockets[0]->setObjectName("computer");
    gridLayout->addWidget(pockets[0], 0, 0, 0, 1);

    for (int i = 8; i < 14; i++) {
        pockets[i] = new Pocket(QString::number(Attr::amounts[i]), this);
        pockets[i]->setObjectName("computer");
        gridLayout->addWidget(pockets[i], 0, 14 - i);
    }

    for (int i = 1; i < 7; i++) {
        pockets[i] = new Pocket(QString::number(Attr::amounts[i]), this);
        pockets[i]->setObjectName("player");
        pockets[i]->setCursor(Qt::PointingHandCursor);
        connect(pockets[i], &QPushButton::clicked, this, [this, i] {
            movePlayer(i);
        });
        gridLayout->addWidget(pockets[i], 1, i);
    }

    pockets[7] = new Store(QString::number(Attr::amounts[7]), this);
    pockets[7]->setObjectName("player");
    gridLayout->addWidget(pockets[7], 0, 7, 0, 1);
}

void Board::movePlayer(int start) {
    if (!playerTurn || Attr::amounts[start] == 0) {
        return;
    }

    int end = start + Attr::amounts[start] + 1;
    int count = 0;
    int i;

    playerTurn = false;
    gameBar->setHintVisible(false);

    Attr::amounts[start] = 0;
    pockets[start]->setAmount(0);

    do {
        for (i = start + 1; i < std::min(end, 14); i++) {
            int amount = ++Attr::amounts[i];
            QTimer::singleShot(++count * 300, this, [this, i, amount] {
                pockets[i]->setAmount(amount);
            });
        }

        start = 0;
        end -= 13;
    } while (end >= 0);

    i--;
    if (i < 7 && Attr::amounts[i] == 1 && Attr::amounts[14 - i] > 0) {
        Attr::amounts[7] += Attr::amounts[14 - i] + 1;
        Attr::amounts[i] = 0;
        Attr::amounts[14 - i] = 0;

        QTimer::singleShot(count * 300 + 300, this, [this, i] {
            pockets[7]->setAmount(Attr::amounts[7]);
            pockets[i]->setAmount(Attr::amounts[i]);
            pockets[14 - i]->setAmount(Attr::amounts[14 - i]);
        });
    }

    if (isPlayerEnded() || isComputerEnded()) {
        QTimer::singleShot(count * 300 + 600, this, &Board::makeEnded);
        return;
    }

    QTimer::singleShot(count * 300 + 600, this, [this, i] {
        gameBar->setHintVisible(Attr::hintVisible);

        if (i == 7) {
            playerTurn = true;
            gameBar->setHintText("Your Turn Again");
        } else {
            gameBar->setHintText("Computer's Turn");
        }
    });

    if (i != 7) {
        QTimer::singleShot(count * 300 + 1200, this, &Board::moveComputer);
    }
}

void Board::moveComputer() {
    int start = nextComputerIndex();
    int end = start + Attr::amounts[start] + 1;
    int count = 0;
    int i;

    Attr::amounts[start] = 0;
    pockets[start]->setAmount(0);

    do {
        for (i = start + 1; i < std::min(end, 14); i++) {            
            if (i == 7) {
                end++;
                continue;
            }

            int amount = ++Attr::amounts[i];
            QTimer::singleShot(++count * 300, this, [this, i, amount] {
                pockets[i]->setAmount(amount);
            });
        }

        start = -1;
        end -= 14;
    } while (end >= 0);

    if (--i == -1) {
        i = 13;
    }

    if (i > 7 && Attr::amounts[i] == 1 && Attr::amounts[14 - i] > 0) {
        Attr::amounts[0] += Attr::amounts[14 - i] + 1;
        Attr::amounts[i] = 0;
        Attr::amounts[14 - i] = 0;

        QTimer::singleShot(count * 300 + 300, this, [this, i] {
            pockets[0]->setAmount(Attr::amounts[0]);            
            pockets[i]->setAmount(Attr::amounts[i]);
            pockets[14 - i]->setAmount(Attr::amounts[14 - i]);
        });
    }

    if (isPlayerEnded() || isComputerEnded()) {
        QTimer::singleShot(count * 300 + 600, this, &Board::makeEnded);
        return;
    }

    QTimer::singleShot(count * 300 + 600, this, [this, i] {
        gameBar->setHintVisible(Attr::hintVisible);

        if (i == 0) {
            gameBar->setHintText("Computer's Turn Again");
        } else {
            playerTurn = true;
            gameBar->setHintText("Your Turn");
        }
    });

    if (i == 0) {
        QTimer::singleShot(count * 300 + 1200, this, &Board::moveComputer);
    }
}

void Board::setPlayerTurn(bool turn) {
    playerTurn = turn;
}

void Board::updateAmounts() {
    for (int i = 0; i < 14; i++) {
        pockets[i]->setAmount(Attr::amounts[i]);
    }
}

int Board::nextComputerIndex() {
    auto getEndIndex = [] (int start) {
        int end = start + Attr::amounts[start] + 1;
        int i;

        do {
            for (i = start + 1; i < std::min(end, 14); i++) {            
                if (i == 7) {
                    end++;
                }
            }

            start = -1;
            end -= 14;
        } while (end >= 0);

        if (--i == -1) {
            return 13;
        }
        return i;
    };

    QList<int> indexes;

    for (int j = 8; j < 14; j++) {
        if (Attr::amounts[j] == 0) {
            continue;
        }

        int i = getEndIndex(j);
        if (i == 0) {
            return j;
        }
        if (i > 7 && Attr::amounts[i] == 0 && Attr::amounts[14 - i] > 0) {
            return j;
        }

        indexes.append(j);
    }

    return indexes[QRandomGenerator::global()->bounded(indexes.size())];
}

bool Board::isPlayerEnded() {
    for (int i = 1; i < 7; i++) {
        if (Attr::amounts[i] != 0) {
            return false;
        }
    }
    return true;
}

bool Board::isComputerEnded() {
    for (int i = 8; i < 14; i++) {
        if (Attr::amounts[i] != 0) {
            return false;
        }
    }
    return true;
}

void Board::makeEnded() {
    for (int i = 8; i < 14; i++) {
        Attr::amounts[0] += Attr::amounts[i];
        Attr::amounts[i] = 0; 
    }

    for (int i = 1; i < 7; i++) {
        Attr::amounts[7] += Attr::amounts[i];
        Attr::amounts[i] = 0;
    }

    Attr::numPlayed++;
    Attr::ended = true;

    gameBar->setHintVisible(Attr::hintVisible);
    gameBar->setRestartButtonVisible(true);
    setEnabled(false);
    updateAmounts();

    if (Attr::amounts[0] > Attr::amounts[7]) {
        Attr::computerScore++;
        gameBar->setHintPixmap(Pixmap::get("Computer.png"));
        gameBar->setHintText("Computer Won!");
    } else if (Attr::amounts[0] < Attr::amounts[7]) {
        Attr::playerScore++;
        gameBar->setHintPixmap(Pixmap::get("Confetti.png"));
        gameBar->setHintText("You Won!");
    } else {
        Attr::numTied++;
        gameBar->setHintPixmap(Pixmap::get("Handshake.png"));
        gameBar->setHintText("Tie!");
    }
}
