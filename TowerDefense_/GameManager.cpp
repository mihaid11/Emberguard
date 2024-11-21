#include "GameManager.h"
#include <iostream>

GameManager::GameManager() : mCurrentState(GameState::RPG), mWindow(sf::VideoMode(1280, 720), "TowerDefense"),
mRpgEngine(mWindow, this), mTowerDefenseEngine(mWindow, this)
{
}

void GameManager::run() {
    // Restart option 
    //mRpgEngine.resetSaveGame();

    while (mWindow.isOpen()) {
        update();
        render();
    }
}

void GameManager::update() {
    switch (mCurrentState) {
    case GameState::RPG:
        mRpgEngine.processEvents();
        mRpgEngine.update();
        break;
    case GameState::TowerDefense:
        mTowerDefenseEngine.processEvents();
        mTowerDefenseEngine.update();
        break;
    case GameState::Transition:
        // Logic for transitioning, if necessary
        break;
    }
}

void GameManager::render() {
    switch (mCurrentState) {
    case GameState::RPG:
        mRpgEngine.render();
        break;
    case GameState::TowerDefense:
        mTowerDefenseEngine.render();
        break;
    case GameState::Transition:
        // Render transition screen or effects, if any
        break;
    }
}

void GameManager::switchToRPG(int crystals) {
    mCurrentState = GameState::RPG;
    mRpgEngine.resume(crystals);
}

void GameManager::switchToTowerDefense(int crystals, int level, const std::vector<std::string>& availableTowers) {
    mCurrentState = GameState::TowerDefense;
    mTowerDefenseEngine.init(level);
}

bool GameManager::isLevelCompleted(int level) const
{
    return mTowerDefenseEngine.isLevelCompleted(level);
}

/*QuestManager& GameManager::getQuestManager()
{
    return mQuestManager;
}*/

GameEngine& GameManager::getGame()
{
    return mTowerDefenseEngine;
}

RPGEngine& GameManager::getGameEngine()
{
    return mRpgEngine;
}
