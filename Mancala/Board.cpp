#include "Board.h"
#include "Game.h"
#include "GameBar.h"
#include "Pocket.h"
#include "Store.h"
#include "Attr.h"

Board::Board(Game *game)
    : QFrame(game), game(game), gameBar(game->getGameBar()) {
    // Set up the layout
    auto gridLayout = new QGridLayout(this);
    gridLayout->setHorizontalSpacing(10);
    gridLayout->setVerticalSpacing(120);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    // Blue store
    pockets[0] = new Store(this, 0, Pocket::BLUE);
    gridLayout->addWidget(pockets[0], 0, 0, 0, 1);

    // Blue pockets
    for (int i = 8; i <= 13; ++i) {
        pockets[i] = new Pocket(this, i, Pocket::BLUE);
        gridLayout->addWidget(pockets[i], 0, 14 - i);
    }

    // Red pockets
    for (int i = 1; i <= 6; ++i) {
        pockets[i] = new Pocket(this, i, Pocket::RED);
        pockets[i]->setCursor(Qt::PointingHandCursor);
        connect(pockets[i], &Pocket::clicked, this, [this, i] {
            moveRed(i);
        });
        gridLayout->addWidget(pockets[i], 1, i);
    }

    // Red store
    pockets[7] = new Store(this, 7, Pocket::RED);
    gridLayout->addWidget(pockets[7], 0, 7, 0, 1);

    // Configure the board to enable or disable AI opponents
    setPlayAI(Attr::get().playAI);
}

Board::~Board() {

}

void Board::updateStones() {
    for (const auto &pocket : pockets) {
        pocket->updateStone();
    }
}

void Board::setPlayAI(bool playAI) {
    for (int i = 8; i < 14; ++i) {
        setCursor(playAI ? Qt::ArrowCursor : Qt::PointingHandCursor);

        if (playAI) {
            disconnect(pockets[i], &Pocket::clicked, nullptr, nullptr);
        } else {
            connect(pockets[i], &Pocket::clicked, this, [this, i] {
                moveBlue(i);
            });
        }
    }
}

void Board::setInteractive(bool interactive) {
    this->interactive = interactive;
}

bool Board::isInteractive() {
    return interactive;
}

void Board::moveRed(int i) {
    // If the board is not interactive, or if it is blue's turn,
    // or if the pocket is empty, exit the function
    if (!isInteractive() || !Attr::get().redTurn ||
        Attr::get().stones[i] == 0) {
        return;
    }

    // Make the board not interactive
    setInteractive(false);

    // Starting number of stones
    int stone = Attr::get().stones[i];
    // The time duration in milliseconds
    int duration = 0;

    // Empty the starting pocket
    Attr::get().stones[i] = 0;
    pockets[i]->updateStone();

    // Distribute stones counterclockwise
    while (stone > 0) {
        // Decrease the number of stones
        stone--;

        // Increase the index location of the current pocket
        i++;
        // Ensure that the index location is less than 13
        i %= 14;
        // Skip the blue store
        if (i == 0) {
            i = 1;
        }

        // Distribute a stone
        Attr::get().stones[i]++;
        // Update the pocket after 300ms
        pockets[i]->updateStone(duration += 300);
    }

    // If the last stone lands in an empty red pocket,
    // take all the stones from the opposite blue pocket if there is any
    if (i >= 1 && i <= 6 && Attr::get().stones[i] == 1 &&
        Attr::get().stones[14 - i] > 0) {
        Attr::get().stones[7] += Attr::get().stones[14 - i] + 1;
        Attr::get().stones[i] = 0;
        Attr::get().stones[14 - i] = 0;

        pockets[7]->updateStone(duration + 500);
        pockets[i]->updateStone(duration + 500);
        pockets[14 - i]->updateStone(duration + 500);
    }

    // If the round has ended after red's turn, exit the function
    if (isRedEnded() || isBlueEnded()) {
        QTimer::singleShot(duration + 500, this, &checkWinner);
        return;
    }

    // If the last stone lands in the red store, then red goes again
    Attr::get().redTurn = (i == 7);

    // Update the game bar
    gameBar->setHintVisible(false);
    QTimer::singleShot(duration + 500, this, [this] {
        // Make the board interactive again
        setInteractive(true);
        gameBar->setHintText(Attr::get().redTurn ? tr("Red's turn again")
                                                 : tr("Blue's turn"));
    });

    // If it is blue's turn next, and the player is playing against AI,
    // the AI will take its turn after 1s
    if (!Attr::get().redTurn && Attr::get().playAI) {
        QTimer::singleShot(duration + 1000, this, [this] {
            moveBlue();
        });
    }
}

void Board::moveBlue(int i) {
    // AI calculating O's next move
    if (i == -1) {
        i = getNextBlue();
    }

    // If the board is not interactive, or if it is red's turn,
    // or if the pocket is empty, exit the function
    if (!isInteractive() || Attr::get().redTurn ||
        Attr::get().stones[i] == 0) {
        return;
    }    

    // Make the board not interactive
    setInteractive(false);

    // Starting number of stones
    int stone = Attr::get().stones[i];
    // The time duration in milliseconds
    int duration = 0;

    // Empty the starting pocket
    Attr::get().stones[i] = 0;
    pockets[i]->updateStone();

    // Distribute stones counterclockwise
    while (stone > 0) {
        // Decrease the number of stones
        stone--;

        // Index location of the current pocket
        i++;
        // Ensure that the index location is less than 13
        i %= 14;
        // Skip the red store
        if (i == 7) {
            i = 8;
        }

        // Distribute a stone
        Attr::get().stones[i]++;
        // Update the pocket after 300ms
        pockets[i]->updateStone(duration += 300);
    }

    // If the last stone lands in an empty blue pocket,
    // take all the stones from the opposite red pocket if there is any
    if (i >= 8 && i <= 13 && Attr::get().stones[i] == 1 &&
        Attr::get().stones[14 - i] > 0) {
        Attr::get().stones[0] += Attr::get().stones[14 - i] + 1;
        Attr::get().stones[i] = 0;
        Attr::get().stones[14 - i] = 0;

        pockets[0]->updateStone(duration + 500);
        pockets[i]->updateStone(duration + 500);
        pockets[14 - i]->updateStone(duration + 500);
    }

    // If the round has ended after blue's turn, exit the function
    if (isRedEnded() || isBlueEnded()) {
        QTimer::singleShot(duration + 500, this, &checkWinner);
        return;
    }

    // If the last stone lands in the blue store, then blue goes again
    Attr::get().redTurn = (i != 0);

    // Update the game bar
    gameBar->setHintVisible(Attr::get().hinted && Attr::get().playAI);
    QTimer::singleShot(duration + 500, this, [this] {
        // Make the board interactive again
        setInteractive(true);
        gameBar->setHintText(Attr::get().redTurn ? tr("Red's turn")
                                                 : tr("Blue's turn again"));
    });

    // If it is blue's turn again, and the player is playing against AI,
    // the AI will take its turn after 1s
    if (!Attr::get().redTurn && Attr::get().playAI) {
        QTimer::singleShot(duration + 1000, this, [this] {
            moveBlue();
        });
    }
}

int Board::getNextBlue() {
    // Store the index locations of all non-empty blue pockets
    QList<int> indexes;

    for (int i = 8; i <= 13; ++i) {
        // Ignore all empty squares
        if (Attr::get().stones[i] == 0) {
            continue;
        }

        // The index location of the pocket the last stone will land in
        QList<int> stones = Attr::get().stones;
        int end = getBlueEndIndex(i, stones);

        // Good if the last stone will land in the blue store
        if (end == 0) {
            return i;
        }

        // Good if the last stone is an empty blue pocket
        if (end >= 8 && end <= 13 &&
            stones[end] == 1 && stones[14 - end] > 0) {
            return i;
        }

        indexes.append(i);
    }

    // Otherwise, pick a random non-empty blue pocket
    return indexes[QRandomGenerator::global()->bounded(indexes.size())];
}

int Board::getBlueEndIndex(int i, QList<int> &stones) {
    // Starting number of stones
    int stone = Attr::get().stones[i];

    // Empty the starting pocket
    stones[i] = 0;

    // Distribute stones counterclockwise
    while (stone > 0) {
        // Decrease the number of stones
        stone--;

        // Index location of the current pocket
        i++;
        // Ensure that the index location is less than 13
        i %= 14;
        // Skip the red store
        if (i == 7) {
            i = 8;
        }

        // Put a stone in the pocket
        stones[i]++;
    }

    // Return the final index location
    return i;
}

bool Board::isRedEnded() {
    for (int i = 1; i <= 6; ++i) {
        if (Attr::get().stones[i] > 0) {
            return false;
        }
    }
    return true;
}

bool Board::isBlueEnded() {
    for (int i = 8; i <= 13; ++i) {
        if (Attr::get().stones[i] > 0) {
            return false;
        }
    }
    return true;
}

void Board::checkWinner() {
    // If all red or blue pockets are empty, the round has ended
    // Distribute all stones from red pockets to the red store
    for (int i = 1; i <= 6; ++i) {
        Attr::get().stones[7] += Attr::get().stones[i];
        Attr::get().stones[i] = 0;
    }
    // Distribute all stones from blue pockets to the blue store
    for (int i = 8; i <= 13; ++i) {
        Attr::get().stones[0] += Attr::get().stones[i];
        Attr::get().stones[i] = 0;
    }

    // If the red store contains more stones than the blue store, red wins
    if (Attr::get().stones[0] < Attr::get().stones[7]) {
        redWins();
        // If the blue store contains more stones than the red store, blue wins
    } else if (Attr::get().stones[0] > Attr::get().stones[7]) {
        blueWins();
        // Otherwise, the game ends in a tie
    } else {
        tie();
    }
}

void Board::end(const QIcon &icon, const QString &text) {
    // Update the game bar
    gameBar->setHintIcon(icon);
    gameBar->setHintText(text);
    gameBar->setRestartVisible(true);

    // Update the board
    updateStones();
    setEnabled(false);

    // Update the game progress
    Attr::get().ended = true;
}

void Board::redWins() {
    end(Icon::load("Win.svg"), tr("Red wins!"));

    // Update the statistics
    Attr::get().redWins++;
}

void Board::blueWins() {
    end(Icon::load(Attr::get().playAI ? "AI.svg" : "Win.svg"),
        tr("Blue wins!"));

    // Update the statistics
    Attr::get().blueWins++;
}

void Board::tie() {
    end(Icon::load("Tie.svg"), tr("Tie!"));

    // Update the statistics
    Attr::get().ties++;
}
