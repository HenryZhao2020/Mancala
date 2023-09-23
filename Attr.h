#pragma once

#include "PCH.h"

namespace Attr {
    inline QList<int> amounts = {0, 4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4};
    inline bool ended;

    inline int numPlayed;
    inline int playerScore;
    inline int computerScore;
    inline int numTied;

    inline bool animated = true;
    inline bool hintVisible = true;

    void save();
    bool load();
}
