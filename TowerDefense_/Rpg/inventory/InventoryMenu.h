#pragma once
#include <SFML/Graphics.hpp>
#include "Inventory.h"

class InventoryMenu {
public:
    InventoryMenu(Inventory& inventory, const sf::Vector2f& position, const sf::Vector2f& slotSize);

    void render(sf::RenderWindow& window);
    void handleMouseClick(const sf::Vector2f& mousePos);
    void updateHover(const sf::Vector2f& mousePos);
    void handleDragAndDrop(const sf::Vector2f& mousePos);

    void update(const Inventory& inventory);

private:
    Inventory& mInventory;
    std::vector<sf::RectangleShape> mSlots;
    sf::Vector2f mSlotSize;
    sf::Vector2f mPosition;
    int mHoveredSlot;
    int mDraggedSlot;

    void updateSlotColors();
    int getSlotIndexAtPosition(const sf::Vector2f& pos) const;
};
