#include "GameMap.h"

GameMap::GameMap() : mSize(sf::Vector2f(10000.0f, 10000.0f))
//,tent(sf::Vector2f(0.0f, 1000.0f), sf::Vector2f(150.0f, 70.0f)) 
{
    //mBuildings.push_back(tent);
}

void GameMap::addBuilding(const sf::Vector2f& position, const sf::Vector2f& size)
{
    Building newBuilding(position, size);
    mBuildings.push_back(newBuilding);
}

void GameMap::render(sf::RenderWindow& window)
{
    for (const auto& building : mBuildings) {
        building.render(window);
    }
}

bool GameMap::checkCollision(const sf::FloatRect& objectBounds) {
    for (const auto& building : mBuildings) {
        if (building.getBounds().intersects(objectBounds)) {
            return true;  // Collision detected
        }
    }
    return false;  // No collision
}
