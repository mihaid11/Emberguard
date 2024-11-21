#include "Menu.h"
#include <iostream>

Menu::Menu(sf::RenderWindow& window, SkillTree& skillTree, Inventory& inventory) 
	: mCurrentMenu("None"),
inventoryButton(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "Inventory"),
skillTreeButton(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "SkillTree"),
exitButton(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "Exit")
{
	mMenuShape.setSize(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
	mMenuShape.setFillColor(sf::Color(50, 50, 50, 255));
	mMenuShape.setPosition(
		(window.getSize().x - mMenuShape.getSize().x) / 2,
		(window.getSize().y - mMenuShape.getSize().y) / 2
	);

	mSkillTreeMenu = std::make_unique<SkillTreeMenu>(sf::Vector2f(mMenuShape.getPosition().x + 10.0f, mMenuShape.getPosition().y + 100.0f),
		sf::Vector2f(mMenuShape.getSize().x - 20.0f, mMenuShape.getSize().y - 110.0f), skillTree);
	mInventoryMenu = std::make_unique<InventoryMenu>(inventory, sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x / 2, mMenuShape.getPosition().y + mMenuShape.getSize().y / 3),
		sf::Vector2f(70.0f, 70.0f));

	sf::Vector2f buttonSize(80.0f, 35.0f);
	float startX = mMenuShape.getPosition().x + 20.0f;
	float startY = mMenuShape.getPosition().y + 10.0f;
	float gap = 20.0f;

	inventoryButton.setPosition(sf::Vector2f(startX, startY));
	skillTreeButton.setPosition(sf::Vector2f(startX + buttonSize.x + gap, startY));
	exitButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 
		mMenuShape.getSize().x - buttonSize.x - gap, startY));

	inventoryButton.setCallback([&]() {
		switchToMenu("Inventory");
		});

	skillTreeButton.setCallback([&]() {
		switchToMenu("SkillTree");
		});

	exitButton.setCallback([&]() {
		window.close();
		});

	mButtons.push_back(inventoryButton);
	mButtons.push_back(skillTreeButton);
	mButtons.push_back(exitButton);

	if (!mFont.loadFromFile("../assests/fonts/gameFont.ttf"))
		std::cout << "Couldn't load font" << std::endl;

	mCrystalText.setFont(mFont);
	mCrystalText.setCharacterSize(14);
	mCrystalText.setFillColor(sf::Color::White);
	mCrystalText.setPosition(mMenuShape.getPosition().x + mMenuShape.getSize().x - mCrystalText.getGlobalBounds().width - 230.0f,
		mMenuShape.getPosition().y + 15.0f);
}

void Menu::handleMouseClick(const sf::Vector2f& mousePos)
{
	for (auto& button : mButtons) {
		if (button.isMouseOver(mousePos)) {
			button.onClick();
		}
	}

	if (mCurrentMenu == "SkillTree") {
		mSkillTreeMenu->handleMouseClick(mousePos);
	}

	if (mCurrentMenu == "Inventory") {
		mInventoryMenu->handleMouseClick(mousePos);
	}
}

void Menu::updateHover(const sf::Vector2f& mousePos) {
	for (auto& button : mButtons) {
		button.updateHover(mousePos);
	}

	if (mCurrentMenu == "SkillTree") {
		mSkillTreeMenu->updateHover(mousePos);
	}

	if (mCurrentMenu == "Inventory") {
		mInventoryMenu->updateHover(mousePos);
	}
}

void Menu::switchToMenu(const std::string& menuName)
{
	mCurrentMenu = menuName;
	std::cout << "Switched to " << menuName << " menu." << std::endl;
}

void Menu::update(int crystals, const Inventory& inventory, const SkillTree& skillTree)
{
	mCrystalText.setString("Crystals " + std::to_string(crystals));

	mInventoryMenu->update(inventory);
	mSkillTreeMenu->update(skillTree);
}

void Menu::render(sf::RenderWindow& window)
{
	window.draw(mMenuShape);
	window.draw(mCrystalText);
	for (auto& button : mButtons) {
		button.render(window);
	}

	if (mCurrentMenu == "SkillTree") {
		mSkillTreeMenu->render(window);
	}
	else if (mCurrentMenu == "Inventory") {
		mInventoryMenu->render(window);
	}
}
