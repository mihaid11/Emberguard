#pragma once
#include <SFML/Graphics.hpp>
#include "../map/GameMap.h"

class MainCharacter
{
public:
	MainCharacter(const sf::Vector2f& position, GameMap& map);

	void update(float dt);
	void render(sf::RenderWindow& window);

	void setPosition(const sf::Vector2f& position);
	sf::Vector2f getPosition() const;
	const sf::Vector2f& getActualPosition() const;
	sf::FloatRect getBounds() const;

private:
	sf::CircleShape mShape;
	sf::Vector2f mPosition;
	float mSpeed;
	GameMap& mMap;
};

