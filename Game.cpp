#include "Game.h"
#include "GameBar.h"
#include "Board.h"
#include "Attr.h"

Game::Game() : QMainWindow(nullptr, Qt::MSWindowsFixedSizeDialogHint) {
    auto widget = new QWidget(this);
    setCentralWidget(widget);

    vboxLayout = new QVBoxLayout(widget);
    vboxLayout->setSpacing(20);
    vboxLayout->setContentsMargins(30, 30, 30, 30);

    bar = new GameBar(this);
    bar->setHintPixmap(Pixmap::get("Bulb.png"));
    bar->setRestartButtonVisible(false);
    vboxLayout->addWidget(bar);

    board = new Board(this);
    vboxLayout->addWidget(board);
}

GameBar *Game::getGameBar() {
    return bar;
}

Board *Game::getBoard() {
    return board;
}

void Game::restart() {
    Attr::amounts = {0, 4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4};
    Attr::ended = false;

    bar->setHintPixmap(Pixmap::get("Bulb.png"));
    bar->setRestartButtonVisible(false);

    vboxLayout->removeWidget(board);
    board->deleteLater();

    board = new Board(this);
    vboxLayout->addWidget(board);

    if (Attr::numPlayed % 2) {  
        bar->setHintText("Computer's Turn");
        board->setPlayerTurn(false);
        QTimer::singleShot(600, this, [this] {
            board->moveComputer();
        });
    } else {
        bar->setHintText("Your Turn");
    }
}

void Game::restore() {
    if (!Attr::load()) {
        bar->setHintText("Click a red pocket at the bottom to begin...");
        bar->setHintVisible(Attr::hintVisible);
        return;
    }

    if (Attr::ended) {
        restart();
        return;
    }

    bar->setHintVisible(Attr::hintVisible);
    bar->setHintText("Your Turn");
    board->updateAmounts();
}
