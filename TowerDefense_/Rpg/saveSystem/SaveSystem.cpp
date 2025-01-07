#include "SaveSystem.h"
#include <fstream>
#include <iostream>

SaveSystem::SaveSystem(const std::string& saveFilePath)
    : mSaveFilePath(saveFilePath) {}

void SaveSystem::save(const sf::Vector2f& playerPosition, const std::vector<sf::Vector2f>& npcPositions, const std::vector<int>& npcWaypoints,
    const int& crystals, const int& year, const int& day, const int& hour,
    const int& minute, const int& bankBalance, const int& hasBorrowActive, 
    const int& penalty, const int& interest, const int& amountToRepay,
    const int& repayDay) {
    std::ofstream outFile(mSaveFilePath);
    if (!outFile) {
        std::cerr << "Error opening save file for writing: " << mSaveFilePath << std::endl;
        return;
    }

    outFile << playerPosition.x << " " << playerPosition.y << std::endl;
    outFile << npcPositions.size() << std::endl;

    for (size_t i = 0; i < npcPositions.size(); ++i) {
        outFile << npcPositions[i].x << " " << npcPositions[i].y << " " << npcWaypoints[i] << std::endl;
    }

    outFile << crystals << std::endl;

    outFile << year << " " << day << " " << hour << " " << minute << std::endl;

    outFile << bankBalance << std::endl;

    outFile << hasBorrowActive << " " << penalty << " " << interest << " " << amountToRepay
        << " " << repayDay << std::endl;

    outFile.close();
}

bool SaveSystem::load(sf::Vector2f& playerPosition, std::vector<sf::Vector2f>& npcPositions, std::vector<int>& npcWaypoints,
    int& crystals, int& year, int& day, int& hour, int& minute, int& bankBalance, 
    int& hasBorrowActive, int& penalty, int& interest, int& amountToRepay, int& repayDay) {
    std::ifstream inFile(mSaveFilePath);
    if (!inFile) {
        std::cerr << "Error opening save file for reading: " << mSaveFilePath << std::endl;
        return false;
    }

    // Load player position
    if (!(inFile >> playerPosition.x >> playerPosition.y)) {
        std::cerr << "Error reading player position from save file." << std::endl;
        return false;
    }

    // Load NPC positions and waypoints
    size_t npcCount;
    if (!(inFile >> npcCount)) {
        std::cerr << "Error reading number of NPCs from save file." << std::endl;
        return false;
    }

    npcPositions.clear();
    npcWaypoints.clear();
    for (size_t i = 0; i < npcCount; ++i) {
        sf::Vector2f pos;
        int waypoint;
        if (!(inFile >> pos.x >> pos.y >> waypoint)) {
            std::cerr << "Error reading NPC data from save file." << std::endl;
            return false;
        }
        npcPositions.push_back(pos);
        npcWaypoints.push_back(waypoint);
    }

    if (!(inFile >> crystals)) {
        std::cerr << "Error reading number of crystals from save file." << std::endl;
        return false;
    }
    if (!(inFile >> year)) {
        std::cerr << "Error reading year from save file." << std::endl;
        return false;
    }
    if (!(inFile >> day)) {
        std::cerr << "Error reading day from save file." << std::endl;
        return false;
    }
    if (!(inFile >> hour)) {
        std::cerr << "Error reading hour from save file." << std::endl;
        return false;
    }
    if (!(inFile >> minute)) {
        std::cerr << "Error reading minute from save file." << std::endl;
        return false;
    }
    if (!(inFile >> bankBalance)) {
        std::cerr << "Error reading bank balance from save file." << std::endl;
        return false;
    }
    if (!(inFile >> hasBorrowActive)) {
        std::cerr << "Error reading active borrow from save file." << std::endl;
        return false;
    }
    if (!(inFile >> penalty)) {
        std::cerr << "Error reading penalty from save file." << std::endl;
        return false;
    }
    if (!(inFile >> interest)) {
        std::cerr << "Error reading interest from save file." << std::endl;
        return false;
    }
    if (!(inFile >> amountToRepay)) {
        std::cerr << "Error reading amount to repay from save file." << std::endl;
        return false;
    }
    if (!(inFile >> repayDay)) {
        std::cerr << "Error reading repay day from save file." << std::endl;
        return false;
    }

    inFile.close();
    return true;
}
