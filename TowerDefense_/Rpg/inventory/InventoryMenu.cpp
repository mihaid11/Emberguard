#include "InventoryMenu.h"
#include <SFML/Graphics.hpp>

InventoryMenu::InventoryMenu(Inventory& inventory, const sf::Vector2f& position, const sf::Vector2f& slotSize)
    : mInventory(inventory), mPosition(position), mSlotSize(slotSize), mHoveredSlot(-1), mDraggedSlot(-1) {
    int totalSlots = mInventory.getSlotCount();
    mSlots.resize(totalSlots);
    for (int i = 0; i < totalSlots; ++i) {
        mSlots[i].setSize(slotSize);
        mSlots[i].setFillColor(sf::Color(0, 0, 0, 220));
        mSlots[i].setOutlineColor(sf::Color::White);
        mSlots[i].setOutlineThickness(2.0f);
        int row = i / 3;
        int col = i % 3;
        mSlots[i].setPosition(mPosition.x + col * slotSize.x, mPosition.y + row * slotSize.y);
    }
}

void InventoryMenu::render(sf::RenderWindow& window) {
    updateSlotColors();
    
    for (int i = 0; i < mSlots.size(); ++i) {
        window.draw(mSlots[i]);
        if (!mInventory.getItemAt(i).empty()) {
            // Display item icon or text (mocking here with a simple text for demonstration)
            sf::Text itemText(mInventory.getItemAt(i), sf::Font(), 12);
            itemText.setPosition(mSlots[i].getPosition());
            window.draw(itemText);
        }
    }
}

void InventoryMenu::updateHover(const sf::Vector2f& mousePos) {
    mHoveredSlot = getSlotIndexAtPosition(mousePos);
}

void InventoryMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    if (mHoveredSlot != -1) {
        if (mDraggedSlot == -1) {
            mDraggedSlot = mHoveredSlot; // Start dragging
        } else {
            mInventory.swapItems(mDraggedSlot, mHoveredSlot); // Swap items
            mDraggedSlot = -1; // End dragging
        }
    }
}

void InventoryMenu::handleDragAndDrop(const sf::Vector2f& mousePos) {
    if (mDraggedSlot != -1 && mHoveredSlot != -1 && mDraggedSlot != mHoveredSlot) {
        mInventory.swapItems(mDraggedSlot, mHoveredSlot);
        mDraggedSlot = -1;
    }
}

void InventoryMenu::update(const Inventory& inventory)
{
    mInventory = inventory; 
    int totalSlots = mInventory.getSlotCount();

    mSlots.resize(totalSlots);
    for (int i = 0; i < totalSlots; ++i) {
        int row = i / 3;
        int col = i % 3;
        mSlots[i].setPosition(mPosition.x + col * mSlotSize.x, mPosition.y + row * mSlotSize.y);
    }
}

void InventoryMenu::updateSlotColors() {
    for (int i = 0; i < mSlots.size(); ++i) {
        if (i == mHoveredSlot) {
            mSlots[i].setFillColor(sf::Color(120, 120, 120, 210)); // Hover color
        } else {
            mSlots[i].setFillColor(sf::Color(0, 0, 0, 220));
        }
    }
}

int InventoryMenu::getSlotIndexAtPosition(const sf::Vector2f& pos) const {
    for (int i = 0; i < mSlots.size(); ++i) {
        if (mSlots[i].getGlobalBounds().contains(pos)) {
            return i;
        }
    }
    return -1;
}
