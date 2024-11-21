#pragma once
#include <SFML/Graphics.hpp>
#include "../../TowerDefense/menues/Button.h"
#include <memory>
#include <vector>
#include "../skillTree/SkillTreeMenu.h"
#include "../skillTree/SkillTree.h"
#include "../inventory/InventoryMenu.h"

class Menu
{
public:
	Menu(sf::RenderWindow& window, SkillTree& skillTree, Inventory& inventory);

	void render(sf::RenderWindow& window);
	void handleMouseClick(const sf::Vector2f& mousePos);
	void updateHover(const sf::Vector2f& mousePos);
	void switchToMenu(const std::string& menuName);

	void update(int crystals, const Inventory& inventory, const SkillTree& skillTree);

private:
	sf::RectangleShape mMenuShape;

	std::vector<Button> mButtons;
	Button skillTreeButton;
	Button inventoryButton;
	Button exitButton;

	std::string mCurrentMenu;
	std::unique_ptr<SkillTreeMenu> mSkillTreeMenu;
	std::unique_ptr<InventoryMenu> mInventoryMenu;
	sf::Font mFont;
	sf::Text mCrystalText;
};

