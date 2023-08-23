#include "Attr.h"

QList<int> Attr::amounts = {0, 4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4};
bool Attr::ended = false;

int Attr::numPlayed = 0;
int Attr::playerScore = 0;
int Attr::computerScore = 0;
int Attr::numTied = 0;

bool Attr::animated = true;
bool Attr::hintVisible = true;

void Attr::save() {
    QFile file("Mancala_Data");
    file.open(QFile::WriteOnly);

    QDataStream stream(&file);
    stream << amounts;
    stream << ended;

    stream << numPlayed;
    stream << playerScore;
    stream << computerScore;
    stream << numTied;

    stream << animated;
    stream << hintVisible;

    file.close();
}

bool Attr::load() {
    QFile file("Mancala_Data");
    if (!file.open(QFile::ReadOnly)) {
        return false;
    }

    QDataStream stream(&file);
    stream >> amounts;
    stream >> ended;

    stream >> numPlayed;
    stream >> playerScore;
    stream >> computerScore;
    stream >> numTied;

    stream >> animated;
    stream >> hintVisible;

    file.close();
    return true;
}
