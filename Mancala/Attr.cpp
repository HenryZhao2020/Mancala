#include "Attr.h"

Attr::Attr() {
    // Reset all attributes to their default values
    resetProgress();
    resetSettings();
    resetStats();
}

Attr::~Attr() {

}

int Attr::getTotalPlays() {
    return redWins + blueWins + ties;
}

void Attr::resetProgress() {
    stones = {0, 4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4};
    redTurn = (getTotalPlays() % 2 == 0);
    ended = false;
}

void Attr::resetSettings() {
    playAI = true;
    animated = true;
    hinted = true;
    lang = Lang::ENGLISH;
}

void Attr::resetStats() {
    redWins = 0;
    blueWins = 0;
    ties = 0;
}

void Attr::save() {
    QFile file("../Saved");
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }

    QDataStream out(&file);
    out << stones;
    out << redTurn;
    out << ended;

    out << playAI;
    out << animated;
    out << hinted;
    out << lang;

    out << redWins;
    out << blueWins;
    out << ties;

    file.close();
}

bool Attr::load() {
    QFile file("../Saved");
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }

    QDataStream in(&file);
    in >> stones;
    in >> redTurn;
    in >> ended;

    in >> playAI;
    in >> animated;
    in >> hinted;
    in >> lang;

    in >> redWins;
    in >> blueWins;
    in >> ties;

    file.close();
    return true;
}

Attr &Attr::get() {
    static Attr attr;
    return attr;
}
