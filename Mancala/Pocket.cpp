#include "Pocket.h"
#include "Attr.h"

void Pocket::setAmount(int amount) {
    setText(QString::number(amount));

    if (Attr::animated) {
        zoom();
    }
}

void Pocket::zoom() {
    fontSize = 24;
    increment = 1;

    auto timer = new QTimer(this);
    timer->setInterval(6);
    connect(timer, &QTimer::timeout, this, [this, timer] {
        if (fontSize < 24) {
            timer->deleteLater();
            return;
        }

        if (fontSize > 48) {
            increment = -1;
        }

        fontSize += increment;
        setStyleSheet("font-size: " + QString::number(fontSize) + "px");
    });
    timer->start();
}
