#pragma once
#include "Rpg/gamengine/RPGEngine.h"
#include "TowerDefense/gamengine/GameEngine.h"
#include "Rpg/dialogueSystem/DIalogueManager.h"

enum class GameState {
    RPG,
    TowerDefense,
    Transition
};

class GameManager
{
public:
    GameManager();
    void run();

    void switchToTowerDefense(int crystals, int level, const std::vector<std::string>& availableTowers);
    void switchToRPG(int crystals);
    bool isLevelCompleted(int level) const;

    GameEngine& getGame();
    RPGEngine& getGameEngine();

private:
    void update();
    void render();

    sf::RenderWindow mWindow;
    GameState mCurrentState;
    RPGEngine mRpgEngine;
    GameEngine mTowerDefenseEngine;

    DialogueManager mDialogueManager;
};



