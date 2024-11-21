#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>
#include "../../TowerDefense/menues/Button.h"

class GameManager;
class RPGEngine;

class StartTowerDefenseMenu
{
public:
	StartTowerDefenseMenu(sf::RenderWindow& window, const std::vector<std::string>& availableTowers,
		RPGEngine* gameEngine, GameManager* gameManager, int level, int crystals);

	void render(sf::RenderWindow& window);
	bool handleMouseClick(const sf::Vector2f& mousePos);
	void updateHover(const sf::Vector2f& mousePos);
	void update(int crystals);

private:
	sf::RectangleShape mMenuShape;
	sf::Font mFont;
	Button mStartButton;

	std::vector<sf::RectangleShape> mTowerSlots;
	std::vector<std::string> mSelectedTowers;

	std::vector<std::pair<sf::RectangleShape, std::string>> mAvailableTowers;

	void initializeTowers(const std::vector<std::string>& availableTowers);
	int getEmptySlotIndex() const;
	int getSlotIndex(const sf::Vector2f& mousePos) const;
	int getTowerIndex(const sf::Vector2f& mousePos) const;
	void deselectTower(int slotIndex);
	void selectTower(int towerIndex);

	GameManager* mGameManager;
	RPGEngine* mGameEngine;

	int mLevel;
	int mCrystals;
	std::vector<std::string> mAvailableTowersString;
};

