#include "Pocket.h"
#include "Board.h"
#include "Attr.h"

Pocket::Pocket(Board *board, int i, const Color &color)
    : QPushButton(QString::number(Attr::get().stones[i]), board), index(i) {
    setObjectName((color == RED) ? "red" : "blue");
}

Pocket::~Pocket() {
    // Ensure all pointers are deleted
    if (zoomTimer != nullptr) {
        zoomTimer->deleteLater();
    }
}

void Pocket::updateStone(int delay) {
    // Hold the number of stones to prevent any unexpected changes
    int stone = Attr::get().stones[index];

    // Display the updated number of stones after the delay
    QTimer::singleShot(delay, this, [this, stone] {
        setText(QString::number(stone));

        // Apply the animation if enabled
        if (Attr::get().animated) {
            zoom();
        }
    });
}

void Pocket::zoom() {
    // Stop the animation if it is currently running
    if (zoomTimer != nullptr) {
        zoomTimer->deleteLater();
    }

    // Reset the font size to 24px
    fontSize = 24;
    // Zoom in the text initially
    zoomingIn = true;

    // Set up the animation
    zoomTimer = new QTimer(this);
    connect(zoomTimer, &QTimer::timeout, this, [this] {
        // Stop the animation after returning to the original font size
        if (fontSize < 24) {
            zoomTimer->deleteLater();
            zoomTimer = nullptr;
            return;
        }

        // Start returning to the original font size after reaching 40px
        if (fontSize >= 48) {
            zoomingIn = false;
        }

        // Update font size
        setStyleSheet(QString("font-size: %0px")
                          .arg(zoomingIn ? ++fontSize : fontSize--));
    });

    // Start the animation
    zoomTimer->start(5);
}
