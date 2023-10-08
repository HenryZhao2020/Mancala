#pragma once

#include "PCH.h"

namespace Attr {
    extern QList<int> amounts;
    extern bool ended;

    extern int numPlayed;
    extern int playerScore;
    extern int cpuScore;
    extern int numTied;

    extern bool animated;
    extern bool hintVisible;

    void save();
    bool load();
}
