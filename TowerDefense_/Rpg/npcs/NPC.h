#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <chrono>
#include "../dialogueSystem/DIalogueManager.h"

class NPC
{
public:
    NPC(const sf::Vector2f& position);

    void advanceDialogue();
    void update(float dt);

    void setDialogue(const std::string& key, const Dialogue& dialogue);
    void setActiveDialogue(const std::string& key);
    Dialogue getActiveDialogue();

    std::string getCurrentDialogue();
    bool hasMoreDialogue() const;
    void resetDialogue();

    void resumeMovement();
    void pauseMovement();
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
    std::string getActiveDialogueKey() const;
    void setPosition(const sf::Vector2f& position);

    void render(sf::RenderWindow& window);
    void drawIndicator(sf::RenderWindow& window);
    bool isPlayerClose(const sf::Vector2f& playerPosition) const;

    int getCurrentWaypoint() const;
    void setCurrentWaypoint(int waypoint);

    sf::Color getColor() const;

protected:
    sf::CircleShape mShape;
    float mInteractionRadius;
    sf::CircleShape mInteractionIndicator;

    std::map<std::string, Dialogue> mDialogues;
    std::string mActiveDialogueKey;
    DialogueManager mDialogueManager;

    bool mPaused;
    float mSpeed;
    std::vector<sf::Vector2f> mPath;
    size_t mCurrentWaypoint;

    bool mIsPaused;
    float mPauseDuration;
    float mPauseTimer;
    std::chrono::steady_clock::time_point mPauseStartTime;

    void startPause(float duration);
    void updatePause(float dt);
};

