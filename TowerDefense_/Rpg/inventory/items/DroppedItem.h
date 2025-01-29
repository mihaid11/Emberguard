#pragma once
#include <SFML/Graphics.hpp>
#include "Item.h"

class DroppedItem {
public:
    DroppedItem(const Item* item, const sf::Vector2f& position, int quantity);

    void render(sf::RenderWindow& window);
    bool isPickedUp(const sf::FloatRect& playerBounds) const;

    int getQuantity() const;
    const Item* getItem();
    const sf::Vector2f& getPosition() const;
    sf::CircleShape getItemShape() const { return mItemShape; }
    bool getPickUpCap();
    void setPickUpCap(bool can);

private:
    const Item* mItem;
    sf::CircleShape mItemShape;
    sf::Vector2f mPosition;
    int mQuantity;
    bool mCanPickUp;
};
