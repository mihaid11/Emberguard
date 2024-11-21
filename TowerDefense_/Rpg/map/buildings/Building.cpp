#include "Building.h"

Building::Building(const sf::Vector2f& position, const sf::Vector2f& size) {
	mShape.setPosition(position);
	mShape.setSize(size);
	mShape.setFillColor(sf::Color::Blue);
}

void Building::render(sf::RenderWindow& window) const
{
	window.draw(mShape);
}

sf::FloatRect Building::getBounds() const
{
	return mShape.getGlobalBounds();
}


