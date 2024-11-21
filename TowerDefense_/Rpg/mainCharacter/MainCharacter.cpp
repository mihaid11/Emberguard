#include "MainCharacter.h"

MainCharacter::MainCharacter(const sf::Vector2f& position, GameMap& map)
    : mPosition(position), mSpeed(150.0f), mMap(map) {

    mShape.setRadius(15);
    mShape.setFillColor(sf::Color::Green);
    mShape.setPosition(position);
}

void MainCharacter::update(float dt)
{
    sf::Vector2f originalPosition = mPosition;

    // Move in the X direction
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        mPosition.x -= mSpeed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        mPosition.x += mSpeed * dt;

    mShape.setPosition(mPosition);

    // Check for X direction collision
    if (mMap.checkCollision(mShape.getGlobalBounds())) {
        mPosition.x = originalPosition.x;  // Revert to the original X position if there's a collision
        mShape.setPosition(mPosition);
    }

    // Move in the Y direction
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        mPosition.y -= mSpeed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        mPosition.y += mSpeed * dt;

    mShape.setPosition(mPosition);

    // Check for Y direction collision
    if (mMap.checkCollision(mShape.getGlobalBounds())) {
        mPosition.y = originalPosition.y;  // Revert to the original Y position if there's a collision
        mShape.setPosition(mPosition);
    }
}

void MainCharacter::render(sf::RenderWindow& window)
{
    window.draw(mShape);
}

void MainCharacter::setPosition(const sf::Vector2f& position)
{
    mShape.setPosition(position);
    mPosition = position;
}

sf::Vector2f MainCharacter::getPosition() const
{
    return mShape.getPosition();
}

sf::FloatRect MainCharacter::getBounds() const
{
    return mShape.getGlobalBounds();
}
