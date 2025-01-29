#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../inventory/items/DroppedItem.h"
#include "../inventory/Inventory.h"

class SaveSystem
{
public:
    SaveSystem(const std::string& saveFilePath);

    void save(const sf::Vector2f& playerPosition, const std::vector<sf::Vector2f>& npcPositions, const std::vector<int>& npcWaypoints,
        const int& crystals, const int& year, const int& day, const int& hour,
        const int& minute, const int& bankBalance, const int& hasBorrowActive, 
        const int& penalty, const int& interest, const int& amountToRepay,
        const int& repayDay, const std::vector<int>& inventoryItemId,
        const std::vector<int>& inventoryItemQuantity, const std::vector<int>& droppedItemId,
        const std::vector<float>& droppedItemXPos, const std::vector<float>& droppedItemYPos,
        const std::vector<int>& droppedItemQuantity);
    bool load(sf::Vector2f& playerPosition, std::vector<sf::Vector2f>& npcPositions, std::vector<int>& npcWaypoints,
        int& crystals, int& year, int& day, int& hour, int& minute, int& bankBalance,
        int& hasBorrowActive, int& penalty, int& interest, int& amountToRepay, int& repayDay, 
        std::vector<int>& inventoryItemId, std::vector<int>& inventoryItemQuantity, 
        std::vector<int>& droppedItemId, std::vector<float>& droppedItemXPos, 
        std::vector<float>& droppedItemYPos, std::vector<int>& droppedItemQuantity);

private:
    std::string mSaveFilePath;
};
