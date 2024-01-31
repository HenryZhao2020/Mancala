#pragma once

#include "Pocket.h"

/**
 * @brief Displays the number of stones that red or blue has in total.
 */
class Store : public Pocket {
    Q_OBJECT

public:
    using Pocket::Pocket;
};
