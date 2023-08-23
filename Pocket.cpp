#include "Pocket.h"
#include "Attr.h"

void Pocket::setAmount(int amount) {
    setText(QString::number(amount));

    if (Attr::animated) {
        zoom();
    }
}

void Pocket::zoom(int fontSize) {
    if (fontSize < 24) {
        increment = 1;
        return;
    }

    if (fontSize > 48) {
        increment = -1;
    }

    setStyleSheet("font-size: " + QString::number(fontSize) + "px");
    QTimer::singleShot(6, this, [this, fontSize] {
        zoom(fontSize + increment);
    });
}
