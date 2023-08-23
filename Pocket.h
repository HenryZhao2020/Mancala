#ifndef POCKET_H
#define POCKET_H

#include "PCH.h"

class Pocket : public QPushButton {
    Q_OBJECT

public:
    using QPushButton::QPushButton;

    void setAmount(int amount);    

private:
    int increment = 1;

    void zoom(int fontSize = 24);
};

#endif
