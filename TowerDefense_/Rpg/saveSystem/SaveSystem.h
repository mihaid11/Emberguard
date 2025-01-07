#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class SaveSystem
{
public:
    SaveSystem(const std::string& saveFilePath);

    void save(const sf::Vector2f& playerPosition, const std::vector<sf::Vector2f>& npcPositions, const std::vector<int>& npcWaypoints,
        const int& crystals, const int& year, const int& day, const int& hour,
        const int& minute, const int& bankBalance, const int& hasBorrowActive, 
        const int& penalty, const int& interest, const int& amountToRepay,
        const int& repayDay);
    bool load(sf::Vector2f& playerPosition, std::vector<sf::Vector2f>& npcPositions, std::vector<int>& npcWaypoints,
        int& crystals, int& year, int& day, int& hour, int& minute, int& bankBalance,
        int& hasBorrowActive, int& penalty, int& interest, int& amountToRepay,
        int& repayDay);

private:
    std::string mSaveFilePath;
};
