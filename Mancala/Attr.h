#pragma once

/**
 * @brief Contains the names of available languages.
 */
namespace Lang {
    const QString ENGLISH = "English";
    const QString CHINESE_SIMPLIFIED = "简体中文";
}

/**
 * @brief Contains shared attributes for saving and loading game state.
 */
class Attr {
public:
    // Game Progress
    /**
     * @brief Stores the number of stones in each pocket (and two stores).
     */
    QList<int> stones;
    /**
     * @brief Whether it is red's turn to pick a pocket.
     */
    bool redTurn;
    /**
     * @brief Whether the current round has ended.
     */
    bool ended;

    // Settings
    /**
     * @brief Whether the player is playing against AI.
     */
    bool playAI;
    /**
     * @brief Whether animations are enabled.
     */
    bool animated;
    /**
     * @brief Whether the hint on the left of the game bar is visible.
     */
    bool hinted;
    /**
     * @brief Displayed language.
     */
    QString lang;

    // Statistics
    /**
     * @brief Total number of wins that red has.
     */
    int redWins;
    /**
     * @brief Total number of wins that blue has.
     */
    int blueWins;
    /**
     * @brief Total number of ties between red and blue.
     */
    int ties;

    // Calculated properties based on the existing attributes
    /**
     * @brief Returns the total number of gameplays.
     * @return Total number of gameplays.
     */
    int getTotalPlays();

    // Reset attributes
    /**
     * @brief Resets the game progress.
     */
    void resetProgress();
    /**
     * @brief Resets all settings to their default values.
     */
    void resetSettings();
    /**
     * @brief Resets all statistics to zero.
     */
    void resetStats();

    // Save & Load
    /**
     * @brief Saves all attributes to the game folder.
     */
    void save();
    /**
     * @brief Loads all attributes from the game folder.
     * @return true if successful; false otherwise.
     */
    bool load();

    // Singleton access
    /**
     * @brief Provides access to the singleton instance.
     * @return The singleton instance.
     */
    static Attr &get();

private:
    // Private constructor to prevent external instantiation
    Attr();
    ~Attr();
};
