#ifndef STORE_H
#define STORE_H

#include "PCH.h"
#include "Pocket.h"

class Store : public Pocket {
    Q_OBJECT

public:
    using Pocket::Pocket;
};

#endif
