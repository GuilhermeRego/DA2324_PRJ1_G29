//
// Created by guire on 24/03/2024.
//

#include "Menu.h"
#include "DataManager.h"
#include <string>
#include <iostream>

using namespace std;

string input = "0";

bool Menu::isDigit(const string& s) {
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

Menu::Menu() {
    init();
}

void Menu::init() {
    mainMenu();
}

void Menu::mainMenu() {
    cout << "\n\n-------------------------------------------------------------------------------------------------------\n"
            "Welcome to the Water Supply Management!\n"
            "Please choose one of the following options:\n"
            "1: Statistics;\n"
            "9: Exit;\n"
            "-------------------------------------------------------------------------------------------------------\n";
    cin >> input;
    if (!isDigit(input)) {
        cout << "Invalid input, try again\n";
        mainMenu();
        return;
    }
    switch (stoi(input)) {
        case 1:
            statistics();
            break;
        case 9:
            cout << "Goodbye!\n";
            break;
        default:
            cout << "Invalid input, try again\n";
            mainMenu();
            break;
    }
}

void Menu::statistics() {
    bool invalidInput = false;
    cout << "Madeira or Portugal?\n";
    cout << "1: Madeira\n";
    cout << "2: Portugal\n";
    cin >> input;
    if (!isDigit(input)) {
        cout << "Invalid input, try again\n";
        statistics();
        return;
    }

    switch(stoi(input)) {
        case 1:
            cout << "Available statistics for Madeira:\n";
            cout << "1 - Number of cities, reservoirs and stations\n"
                    "2 - Total population\n"
                    "3 - Total demand\n"
                    "4 - Average maximum delivery\n"
                    "5 - Order cities by population\n"
                    "6 - Order cities by demand\n"
                    "7 - Order reservoirs by maximum delivery\n"
                    "8 - Show connections between service points\n"
                    "9 - Return to main menu\n";
            cin >> input;
            if (!isDigit(input)) {
                cout << "Invalid input, try again\n";
                statistics();
                return;
            }
            switch (stoi(input)) {
                case 1: {
                    cout << "Number of cities: " << dataManager.getCities().size() << endl;
                    cout << "Number of reservoirs: " << dataManager.getReservoirs().size() << endl;
                    cout << "Number of stations: " << dataManager.getStations().size() << endl;
                    break;
                }
                case 2: {
                    int totalPopulation = 0;
                    for (auto &city: dataManager.getCities()) {
                        totalPopulation += city.second.getPopulation();
                    }
                    cout << "Total population: " << totalPopulation << endl;
                    break;
                }
                case 3: {
                    double totalDemand = 0;
                    for (auto &city: dataManager.getCities()) {
                        totalDemand += city.second.getDemand();
                    }
                    cout << "Total demand: " << totalDemand << endl;
                    break;
                }
                case 4: {
                    double totalMaxDelivery = 0;
                    for (auto &reservoir: dataManager.getReservoirs()) {
                        totalMaxDelivery += reservoir.second.getMaxDelivery();
                    }
                    cout << "Average maximum delivery: " << totalMaxDelivery / (double) dataManager.getReservoirs().size()
                         << endl;
                    break;
                }
                case 5: {
                    cout << "Cities ordered by population:\n";
                    vector<pair<string, City>> cities;
                    for (auto &city: dataManager.getCities()) {
                        cities.emplace_back(city);
                    }
                    sort(cities.begin(), cities.end(), [](const pair<string, City> &a, const pair<string, City> &b) {
                        return a.second.getPopulation() > b.second.getPopulation();
                    });
                    int i = 1;
                    for (auto &city: cities) {
                        cout << i << ": " << city.second.getName() << " - " << city.second.getPopulation() << endl;
                        i++;
                    }
                    break;
                }
                case 6: {
                    cout << "Cities ordered by demand:\n";
                    vector<pair<string, City>> cities;
                    for (auto &city: dataManager.getCities()) {
                        cities.emplace_back(city);
                    }
                    sort(cities.begin(), cities.end(), [](const pair<string, City> &a, const pair<string, City> &b) {
                        return a.second.getDemand() > b.second.getDemand();
                    });
                    int i = 1;
                    for (auto &city: cities) {
                        cout << i << ": " << city.second.getName() << " - " << city.second.getDemand() << endl;
                        i++;
                    }
                    break;
                }
                case 7: {
                    cout << "Reservoirs ordered by maximum delivery:\n";
                    vector<pair<string, Reservoir>> reservoirs;
                    for (auto &reservoir: dataManager.getReservoirs()) {
                        reservoirs.emplace_back(reservoir);
                    }
                    sort(reservoirs.begin(), reservoirs.end(), [](const pair<string, Reservoir> &a, const pair<string, Reservoir> &b) {
                        return a.second.getMaxDelivery() > b.second.getMaxDelivery();
                    });
                    int i = 1;
                    for (auto &reservoir: reservoirs) {
                        cout << i << ": " << reservoir.second.getName() << " - " << reservoir.second.getMaxDelivery() << endl;
                        i++;
                    }
                    break;
                }
                case 8: {
                    cout << "Connections between service points (In the format \"source -> dest\"):\n";
                    for (auto &vertex : dataManager.getGraph().getVertexSet()) {
                        if (!vertex->getAdj().empty()) {
                            cout << vertex->getInfo() << " -> ";
                            for (auto &edge: vertex->getAdj()) {
                                cout << edge->getDest()->getInfo() << " ";
                            }
                            cout << endl;
                        }
                        else {
                            cout << vertex->getInfo() << " -> No connections\n";
                        }
                    }
                    break;
                }
                case 9:
                    break;
                default:
                    cout << "Invalid input, try again\n";
                    break;
            }
            mainMenu();
            break;
        case 2:
            cout << "Portugal\n";
            mainMenu();
            break;
        default:
            cout << "Invalid input, try again\n";
            statistics();
            break;
    }
}
