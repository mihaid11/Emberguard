#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>

struct InventoryItem {
    std::string itemID;
    int quantity;
};

class Inventory 
{
public:
    Inventory(int rows = 2, int cols = 3);

    void addItem(const std::string& itemName, int quantity);
    void swapItems(int slot1, int slot2);
    int getItemQuantityAt(int slot) const;
    const std::string& getItemAt(int slot) const;
    int getSlotCount() const;
    void resize(int newRows, int newCols);

private:
    std::vector<std::pair<std::string, int>> mSlots;
    int mRows;
    int mCols;
    
    int getFirstEmptySlot() const;
    int findSlotWithItem(const std::string& item) const;
};
