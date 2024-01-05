#pragma once

class Board;

/**
 * @brief Displays the number of stones in this pocket.
 */
class Pocket : public QPushButton {
    Q_OBJECT

public:
    /**
     * @brief Colors of a pocket.
     */
    enum Color {
        RED,
        BLUE,
    };

    /**
     * @brief Initializes a new 'Pocket' instance.
     * @param board The parent 'Board' instance.
     * @param i The assigned index location.
     */
    Pocket(Board *board, int i, const Color &color);
    ~Pocket();

    /**
     * @brief Displays the updated number of stones in this pocket.
     * @param delay The delay for the update in milliseconds.
     */
    void updateStone(int delay = 0);

    /**
     * @brief Zooms in the displayed text,
     * then zooms out to the original size.
     */
    void zoom();

private:
    // Assigned index location
    int index;

    // Used for zoom effect
    QTimer *zoomTimer = nullptr;
    // Store the font size
    int fontSize;
    // Whether the displayed text is zooming in
    bool zoomingIn;
};
