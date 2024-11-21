#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class SaveSystem
{
public:
    SaveSystem(const std::string& saveFilePath);

    void save(const sf::Vector2f& playerPosition, const std::vector<sf::Vector2f>& npcPositions, const std::vector<int>& npcWaypoints,
        const int& crystals);
    bool load(sf::Vector2f& playerPosition, std::vector<sf::Vector2f>& npcPositions, std::vector<int>& npcWaypoints,
        int& crystals);

private:
    std::string mSaveFilePath;
};
