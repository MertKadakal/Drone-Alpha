#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>

Map::Map() {
    // TODO: Your code here
    // Initialize all distances to a value representing no direct connection
    // Initialize all provinces as unvisited
    resetVisited();
}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    // TODO: Your code here
    // Read each line in the CSV file
    // Read each cell separated by a comma
    // Convert cell to an integer and store in distanceMatrix

    std::ifstream file(filename);
    std::string line;

    if (file.is_open()) {
        int row = 0;
        while (std::getline(file, line) && row < 81) {
            std::stringstream ss(line);
            std::string cell;
            int col = 0;

            while (std::getline(ss, cell, ',') && col < 81) {
                distanceMatrix[row][col] = std::stoi(cell);
                col++;
            }

            row++;
        }
        file.close();
    }
}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    // TODO: Your code here
    return distanceMatrix[provinceA][provinceB] <= maxDistance;
}

// Marks a province as visited
void Map::markAsVisited(int province) {
    // TODO: Your code here
    visited[province] = true;
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    // TODO: Your code here
    return visited[province];
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    // TODO: Your code here
    for (bool & i : visited) {
        i = false;
    }
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    // TODO: Your code here
    int count = 0;
    for (bool i : visited) {
        if (i) {
            count++;
        }
    }
    return count;
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    // TODO: Your code here
    return distanceMatrix[provinceA][provinceB];
}