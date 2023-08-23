#ifndef ATTR_H
#define ATTR_H

#include "PCH.h"

class Attr {
public:
    static QList<int> amounts;
    static bool ended;

    static int numPlayed;
    static int playerScore;
    static int computerScore;
    static int numTied;

    static bool animated;
    static bool hintVisible;

    static void save();
    static bool load();
};

#endif
