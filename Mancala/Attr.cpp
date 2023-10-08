#include "Attr.h"
using namespace Attr;

QList<int> Attr::amounts = {0, 4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4};
bool Attr::ended;

int Attr::numPlayed;
int Attr::playerScore;
int Attr::cpuScore;
int Attr::numTied;

bool Attr::animated = true;
bool Attr::hintVisible = true;

void Attr::save() {
    QFile file("../Saved");
    file.open(QFile::WriteOnly);

    QDataStream out(&file);
    out << amounts;
    out << ended;

    out << numPlayed;
    out << playerScore;
    out << cpuScore;
    out << numTied;

    out << animated;
    out << hintVisible;

    file.close();
}

bool Attr::load() {
    QFile file("../Saved");
    if (!file.open(QFile::ReadOnly)) {
        return false;
    }

    QDataStream in(&file);
    in >> amounts;
    in >> ended;

    in >> numPlayed;
    in >> playerScore;
    in >> cpuScore;
    in >> numTied;

    in >> animated;
    in >> hintVisible;

    file.close();
    return true;
}
