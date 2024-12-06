#include "StartTowerDefenseMenu.h"
#include <iostream>
#include "../../GameManager.h"
#include "../gamengine/RPGEngine.h"

StartTowerDefenseMenu::StartTowerDefenseMenu(sf::RenderWindow& window, const std::vector<std::string>& availableTowers,
    RPGEngine* gameEngine, GameManager* gameManager, int level, int crystals)
    : mStartButton(sf::Vector2f(1000.0f, 575.0f), sf::Vector2f(80.0f, 30.0f), "Start"),
    mGameManager(gameManager), mAvailableTowersString(availableTowers), mCrystals(crystals),
    mLevel(level), mGameEngine(gameEngine)
{
    if (!mFont.loadFromFile("../assests/fonts/gameFont.ttf")) {
        std::cout << "Couldn't load font from file" << std::endl;
    }

    mMenuShape.setSize(sf::Vector2f(window.getSize().x * 3.0f / 4.0f, window.getSize().y * 3.0f / 4.0f));
    mMenuShape.setFillColor(sf::Color(50, 50, 50, 255));
    mMenuShape.setPosition(sf::Vector2f((window.getSize().x - mMenuShape.getSize().x) / 2.0f,
        (window.getSize().y - mMenuShape.getSize().y) / 2.0f));

    // Initialize the slots for selected towers
    for (int i = 0; i < 6; ++i) {
        sf::RectangleShape slot(sf::Vector2f(50.0f, 50.0f));
        slot.setFillColor(sf::Color(100, 100, 100, 255));
        slot.setPosition(340.0f + i * 60.0f, 200.0f);
        mTowerSlots.push_back(slot);
        mSelectedTowers.push_back("");
    }

    // Initialize available towers
    initializeTowers(mAvailableTowersString);

    mStartButton.setCallback([&]() {
        mGameEngine->saveGame();
        mGameManager->switchToTowerDefense(mCrystals, mLevel, mAvailableTowersString);
        });
}

void StartTowerDefenseMenu::initializeTowers(const std::vector<std::string>& availableTowers)
{
    mAvailableTowers.clear();
    for (size_t i = 0; i < availableTowers.size(); ++i) {
        sf::RectangleShape towerSlot(sf::Vector2f(50.0f, 50.0f));
        towerSlot.setFillColor(sf::Color::White);
        towerSlot.setOutlineThickness(2.0f);
        towerSlot.setOutlineColor(sf::Color::Black);
        towerSlot.setPosition(340.0f + (i % 3) * 60.0f, 300.0f + (i / 3) * 60.0f);

        sf::CircleShape towerIcon(20.0f);
        towerIcon.setPosition(towerSlot.getPosition().x + 5.0f, towerSlot.getPosition().y + 5.0f);

        if (availableTowers[i] == "LaserTower") {
            towerIcon.setFillColor(sf::Color::Green);
        }
        else if (availableTowers[i] == "FlameTurret") {
            towerIcon.setFillColor(sf::Color(50, 50, 50, 255));
        }

        mAvailableTowers.emplace_back(towerSlot, availableTowers[i]);
    }
}

void StartTowerDefenseMenu::render(sf::RenderWindow& window)
{
    window.draw(mMenuShape);

    for (const auto& slot : mTowerSlots) {
        window.draw(slot);
    }

    for (const auto& tower : mAvailableTowers) {
        window.draw(tower.first);
        //window.draw(tower.second);
    }

    mStartButton.render(window);
}

bool StartTowerDefenseMenu::handleMouseClick(const sf::Vector2f& mousePos)
{
    int slotIndex = getSlotIndex(mousePos);
    if (slotIndex != -1) {
        deselectTower(slotIndex);
        return true;
    }

    int towerIndex = getTowerIndex(mousePos);
    if (towerIndex != -1) {
        selectTower(towerIndex);
        return true;
    }

    if (mStartButton.isMouseOver(mousePos)) {
        mStartButton.onClick();
        return true;
    }

    return false;
}

void StartTowerDefenseMenu::updateHover(const sf::Vector2f& mousePos)
{
    mStartButton.updateHover(mousePos);
}

int StartTowerDefenseMenu::getEmptySlotIndex() const
{
    for (int i = 0; i < mSelectedTowers.size(); ++i) {
        if (mSelectedTowers[i].empty()) {
            return i;
        }
    }
    return -1;
}

int StartTowerDefenseMenu::getSlotIndex(const sf::Vector2f& mousePos) const
{
    for (int i = 0; i < mTowerSlots.size(); ++i) {
        if (mTowerSlots[i].getGlobalBounds().contains(mousePos)) {
            return i;
        }
    }
    return -1;
}

int StartTowerDefenseMenu::getTowerIndex(const sf::Vector2f& mousePos) const
{
    for (int i = 0; i < mAvailableTowers.size(); ++i) {
        if (mAvailableTowers[i].first.getGlobalBounds().contains(mousePos)) {
            return i;
        }
    }
    return -1;
}

void StartTowerDefenseMenu::deselectTower(int slotIndex)
{
    if (!mSelectedTowers[slotIndex].empty()) {
        for (auto& tower : mAvailableTowers) {
            if (tower.second == mSelectedTowers[slotIndex]) {
                tower.first.setFillColor(sf::Color::Blue); 
                break;
            }
        }
        mSelectedTowers[slotIndex] = "";
    }
}

void StartTowerDefenseMenu::selectTower(int towerIndex)
{
    int emptySlot = getEmptySlotIndex();
    if (emptySlot != -1) {
        mSelectedTowers[emptySlot] = mAvailableTowers[towerIndex].second;
        mAvailableTowers[towerIndex].first.setFillColor(sf::Color(50, 50, 50));
    }
}

void StartTowerDefenseMenu::update(int crystals)
{
    mCrystals = crystals;
    mStartButton.setCallback([&]() {
        mGameEngine->saveGame();
        mGameManager->switchToTowerDefense(mCrystals, mLevel, mAvailableTowersString);
        });
}
