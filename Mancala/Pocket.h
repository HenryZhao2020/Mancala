#pragma once

#include "PCH.h"

class Pocket : public QPushButton {
    Q_OBJECT

public:
    using QPushButton::QPushButton;

    void setAmount(int amount);    

private:
    int fontSize;
    int increment;

    void zoom();
};
