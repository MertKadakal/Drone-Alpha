#include "RoutePlanner.h"
#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <istream>

using namespace std;

// Array to help you out with name of the cities in order
const std::string cities[81] = { 
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik", 
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne", 
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta", 
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya", 
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya", 
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat", 
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir", 
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce" 
};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {

    // TO DO:
    // Load map data from file
    // Mark all provinces as unvisited initially

    // Load priority provinces
    // Load restricted provinces

    loadPriorityProvinces(priority_data);
    loadWeatherRestrictedProvinces(restricted_data);
    map.loadDistanceData(distance_data);
}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    // TODO: Your code here
    std::ifstream file(filename);
    std::string line;
    int currentIndex = 0;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::regex regex("\\((\\d+)\\)");
            std::smatch match;

            if (std::regex_search(line, match, regex)) {
                priorityProvinces[currentIndex++] = std::stoi(match.str(1));
            }
        }
        numPriorityProvinces = currentIndex;
        file.close();
    }
}

// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    // TODO: Your code here
    std::ifstream file(filename);
    std::string line;
    int currentIndex = 0;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::regex regex("\\((\\d+)\\)");
            std::smatch match;

            if (std::regex_search(line, match, regex)) {
                weatherRestrictedProvinces[currentIndex++] = std::stoi(match.str(1));
            }
        }
        numWeatherRestrictedProvinces = currentIndex;
        file.close();
    }
}

// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    for (int i = 0; i < numPriorityProvinces; i++) {
        if (priorityProvinces[i] == province) {
            return true;
        }
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    for (int i = 0; i < numWeatherRestrictedProvinces; i++) {
        if (weatherRestrictedProvinces[i] == province) {
            return true;
        }
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(const int startingCity) {

    stack.push(startingCity);
    route.push_back(startingCity);
    map.markAsVisited(startingCity);

    //check all cities if it's ok to add to the queue
    for (int i = 0; i < 81; i++) {
        if (map.distanceMatrix[startingCity][i] > 0 && map.distanceMatrix[startingCity][i] < maxDistance && !map.isVisited(i)) {
            enqueueNeighbors(i);
        }
    }

    //start exploring from the front-province of the queue
    exploreFromProvince(queue.dequeue());
}

// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(const int province) {
    stack.push(province);
    route.push_back(province);
    map.markAsVisited(province);

    //this loop is being executed until the exploration is completed
    while (!isExplorationComplete()) {
        int currentProvince = stack.peek();

        //check all cities
        queue.resetQueue();
        for (int i = 0; i < 81; i++) {
            if (map.distanceMatrix[currentProvince][i] > 0 &&
                map.distanceMatrix[currentProvince][i] < maxDistance &&
                !map.isVisited(i)) {

                    if (isWeatherRestricted(i)) {
                        cout << "Province " << cities[i] << " is weather-restricted. Skipping." << endl;
                    } else {
                        totalDistanceCovered += map.distanceMatrix[currentProvince][i];
                        enqueueNeighbors(i);
                    }

                }
        }

        //if there's at least one province to go, keep exploring; otherwise backtrack
        if (queue.isEmpty()) {
            backtrack();
        } else {
            exploreFromProvince(queue.dequeue());
        }
    }
}

void RoutePlanner::backtrack() {

    stack.pop();

    if (isExplorationComplete()) {
        displayResults();
    } else {
        if (stack.top > 1) {
            totalDistanceCovered -= map.distanceMatrix[stack.data[stack.top - 1]][stack.peek()];
        }
    }

}

void RoutePlanner::enqueueNeighbors(int province) {
    if (isPriorityProvince(province)) {
        queue.enqueuePriority(province);
    } else {
        queue.enqueue(province);
    }
}

bool RoutePlanner::isExplorationComplete() const {
    return stack.isEmpty();
}

void RoutePlanner::displayResults() const {
    // TODO: Your code here
    // Display "Journey Completed!" message
    // Display the total number of provinces visited
    // Display the total distance covered
    // Display the route in the order visited
    // Priority Province Summary

    std:cout << "----------------------------\n"
                "Journey Completed!\n"
                "----------------------------\n"
                "Total Number of Provinces Visited: " << route.size() << "\n"
                "Total Distance Covered: " << totalDistanceCovered << "\n"
                "Route Taken:\n" <<
                    route.at(0) << " -> " << route.at(1) << " -> " << route.at(2) << " -> " << route.at(3) << " -> " << route.at(4) << " -> ...\n"
                "Priority Provinces Status:\n" << std::endl;

    int numOfVisitedPriorties = 0;
    for (int i = 0; i<numPriorityProvinces; i++) {
        if (map.isVisited(priorityProvinces[i])) {
            cout << "- " << cities[priorityProvinces[i]] << " (Visited)" << endl;
            numOfVisitedPriorties++;
        } else {
            cout << "- " << cities[priorityProvinces[i]] << " (Not Visited)" << endl;
        }
    }
    cout << endl;
    cout << "Total Priority Provinces Visited: " << numOfVisitedPriorties << " out of " << numPriorityProvinces << endl;
    if (numOfVisitedPriorties == numPriorityProvinces) {
        cout << "Success: All priority provinces were visited." << endl;
    } else {
        cout << "Warning: Not all priority provinces were visited." << endl;
    }
}