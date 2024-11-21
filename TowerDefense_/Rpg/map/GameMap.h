#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "buildings/Building.h"
//#include "SeraphinaTent.h"

class GameMap
{
public:
	GameMap();

	void addBuilding(const sf::Vector2f& position, const sf::Vector2f& size);
	void render(sf::RenderWindow& window);
	bool checkCollision(const sf::FloatRect& objectBounds);

private:
	std::vector<Building> mBuildings;
	sf::Vector2f mSize;

	//SeraphinaTent tent;
};

