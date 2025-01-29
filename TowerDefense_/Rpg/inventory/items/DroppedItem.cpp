#include "DroppedItem.h"

DroppedItem::DroppedItem(const Item* item, const sf::Vector2f& position, int quantity)
    : mItem(item), mPosition(position), mQuantity(quantity), mCanPickUp(false) {
    mItemShape.setRadius(15.f);
    mItemShape.setFillColor(sf::Color(255, 255, 255, 50));
    mItemShape.setOutlineColor(sf::Color::White);
    mItemShape.setOutlineThickness(2.f);
    mItemShape.setPosition(mPosition);
}

void DroppedItem::render(sf::RenderWindow& window) {
    window.draw(mItemShape);
    
    //Draw the icon
    // TODO : Implement item
    sf::RectangleShape icon(sf::Vector2f(20.f, 15.f));
    icon.setPosition(mPosition.x + (mItemShape.getRadius() - icon.getSize().x) / 2, mPosition.y + (mItemShape.getRadius() - icon.getSize().y) / 2);
    window.draw(icon);
}

bool DroppedItem::isPickedUp(const sf::FloatRect& playerBounds) const {
    return mItemShape.getGlobalBounds().intersects(playerBounds);
}

int DroppedItem::getQuantity() const
{
    return mQuantity;
}

const Item* DroppedItem::getItem()
{
    return mItem;
}

const sf::Vector2f& DroppedItem::getPosition() const
{
    return mPosition;
}

bool DroppedItem::getPickUpCap()
{
    return mCanPickUp;
}

void DroppedItem::setPickUpCap(bool can)
{
    mCanPickUp = can;
}
