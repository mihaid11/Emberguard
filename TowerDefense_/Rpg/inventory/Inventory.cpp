#include "Inventory.h"

Inventory::Inventory(int rows, int cols) : mRows(rows), mCols(cols) {
    mSlots.resize(rows * cols, { "", 0 });
}

void Inventory::addItem(const std::string& item, int quantity) {
    int slotWithItem = findSlotWithItem(item);

    if (slotWithItem != -1) {
        mSlots[slotWithItem].second += quantity;
    }
    else {
        int emptySlot = getFirstEmptySlot();
        if (emptySlot != -1) {
            mSlots[emptySlot] = { item, quantity };
        }
    }
}

void Inventory::swapItems(int slot1, int slot2) {
    if (slot1 >= 0 && slot1 < mSlots.size() && slot2 >= 0 && slot2 < mSlots.size()) {
        std::swap(mSlots[slot1], mSlots[slot2]);
    }
}

const std::string& Inventory::getItemAt(int slot) const {
    if (slot >= 0 && slot < mSlots.size()) {
        return mSlots[slot].first;
    }
    static std::string empty = "";
    return empty;
}

int Inventory::getItemQuantityAt(int slot) const {
    if (slot >= 0 && slot < mSlots.size()) {
        return mSlots[slot].second;
    }
    return 0;
}

int Inventory::getSlotCount() const {
    return mRows * mCols;
}

void Inventory::resize(int newRows, int newCols) {
    mRows = newRows;
    mCols = newCols;
    mSlots.resize(newRows * newCols, { "", 0 });
}

int Inventory::getFirstEmptySlot() const {
    for (int i = 0; i < mSlots.size(); ++i) {
        if (mSlots[i].second == 0) {
            return i;
        }
    }
    return -1;
}

int Inventory::findSlotWithItem(const std::string& item) const {
    for (int i = 0; i < mSlots.size(); ++i) {
        if (mSlots[i].first == item) {
            return i;
        }
    }
    return -1;
}
