#include "Attr.h"

void Attr::save() {
    QFile file("Mancala_Data");
    file.open(QFile::WriteOnly);

    QDataStream out(&file);
    out << amounts;
    out << ended;

    out << numPlayed;
    out << playerScore;
    out << computerScore;
    out << numTied;

    out << animated;
    out << hintVisible;

    file.close();
}

bool Attr::load() {
    QFile file("Mancala_Data");
    if (!file.open(QFile::ReadOnly)) {
        return false;
    }

    QDataStream in(&file);
    in >> amounts;
    in >> ended;

    in >> numPlayed;
    in >> playerScore;
    in >> computerScore;
    in >> numTied;

    in >> animated;
    in >> hintVisible;

    file.close();
    return true;
}
