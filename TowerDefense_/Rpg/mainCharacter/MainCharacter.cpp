#include "MainCharacter.h"

MainCharacter::MainCharacter(const sf::Vector2f& position, GameMap& map)
    : mPosition(position), mSpeed(150.0f), mMap(map) {

    mShape.setRadius(15);
    mShape.setFillColor(sf::Color::Green);
    mShape.setPosition(position);
}

void MainCharacter::update(float dt)
{
    sf::Vector2f direction(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        direction.x -= 1.15f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        direction.x += 1.15f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        direction.y -= 1.15f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        direction.y += 1.15f;

    if (direction.x != 0.f || direction.y != 0.f)
    {
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= length;  // Normalize the vector
    }

    sf::Vector2f originalPosition = mPosition;
    mPosition += direction * mSpeed * dt;

    mShape.setPosition(mPosition);

    // Check for collision and revert if necessary
    if (mMap.checkCollision(mShape.getGlobalBounds())) {
        mPosition = originalPosition;
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

const sf::Vector2f& MainCharacter::getActualPosition() const
{
    return mShape.getPosition();
}

sf::FloatRect MainCharacter::getBounds() const
{
    return mShape.getGlobalBounds();
}
