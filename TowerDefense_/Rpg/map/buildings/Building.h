#pragma once
#include <SFML/Graphics.hpp>

class Building
{
public:
	Building(const sf::Vector2f& position, const sf::Vector2f& size);

	void render(sf::RenderWindow& window) const;
	sf::FloatRect getBounds() const;

protected:
	sf::RectangleShape mShape;
};

