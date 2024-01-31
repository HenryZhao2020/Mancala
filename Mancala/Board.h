#pragma once

// Forward declarations
class Game;
class GameBar;
class Pocket;

/**
 * @brief Displays a Mancala board with stores on both sides
 * and pockets on the top and bottom.
 */
class Board : public QFrame {
    Q_OBJECT

public:
    /**
     * @brief Initializes a new 'Board' instance.
     * @param game The parent 'Game' instance.
     */
    Board(Game *game);
    ~Board();

    /**
     * @brief Distributes red stones from the specified starting index.
     * @param i The starting index.
     */
    void moveRed(int i);

    /**
     * @brief Distributes blue stones from the specified starting index.
     * @param i The starting index.
     */
    void moveBlue(int i = -1);

    /**
     * @brief Displays the updated number of stones in each pocket and store.
     */
    void updateStones();

    /**
     * @brief Configures the board to enable or disable AI opponents.
     * @param playAI Whether to play against AI.
     */
    void setPlayAI(bool playAI);

    /**
     * @brief Sets whether the board is interactive
     * with keyboard and mouse.
     * @param interactive Whether the board is interactive
     * with keyboard and mouse.
     */
    void setInteractive(bool interactive);

    /**
     * @brief Returns whether the board is interactive
     * with keyboard and mouse.
     * @return Whether the board is interactive
     * with keyboard and mouse.
     */
    bool isInteractive();

private:
    Game *game;
    GameBar *gameBar;

    // Store all pockets
    Pocket *pockets[14];
    // Whether the board is interactive with keyboard and mouse
    bool interactive = true;

    /**
     * @brief Returns blue's next move calculated by AI.
     * @return The index location of the next blue pocket.
     */
    int getNextBlue();

    /**
     * @brief Simulates the stone distribution from a blue pocket.
     * @param i The starting index location.
     * @param stones Simulates the number of stones in each pocket
     * after distribution without modifying the original list.
     * @return The index location of the pocket the last stone will land in.
     */
    int getBlueEndIndex(int i, QList<int> &stones);

    /**
     * @brief Checks if all red pockets are empty.
     * @return 'true' if all red pockets are empty; 'false' otherwise.
     */
    bool isRedEnded();

    /**
     * @brief Checks if all blue pockets are empty.
     * @return 'true' if all blue pockets are empty; 'false' otherwise.
     */
    bool isBlueEnded();

    /**
     * @brief Checks the winner after a round ends.
     */
    void checkWinner();

    /**
     * @brief Ends the game and updates the UI.
     * @param icon The hint icon to be displayed on the game bar.
     * @param text The hint text to be displayed on the game bar.
     */
    void end(const QIcon &icon, const QString &text);

    /**
     * @brief Handles the end of game if red wins.
     */
    void redWins();

    /**
     * @brief Handles the end of game if blue wins.
     */
    void blueWins();

    /**
     * @brief Handles the end of game if red and blue tie.
     */
    void tie();
};
