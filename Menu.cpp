//
// Created by guire on 24/03/2024.
//

#include "Menu.h"
#include "DataManager.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

string input = "0";

bool Menu::isDigit(const string& s) {
    if (!all_of(s.begin(), s.end(), ::isdigit)) return false;
    return true;
}

Menu::Menu() {
    init();
}

void Menu::init() {
    mainMenu();
}

void Menu::mainMenu() {
    cout << "\n-------------------------------------------------------------------------------------------------------\n"
            "Welcome to the Water Supply Management!\n"
            "Please choose one of the following options:\n"
            "1 - Statistics;\n"
            "2 - Basic Service Metrics;\n"
            "3 - Reliability and Sensitivity to Failures;\n"
            "8 - Configurations;\n"
            "9 - Exit;\n"
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
        case 2:
            basicServiceMetrics();
            break;
        case 3:
            //TODO
            break;
        case 8:
            configurations();
            break;
        case 9:
            cout << "Goodbye!\n";
            exit(0);
        default:
            cout << "Invalid input, try again\n";
            mainMenu();
            break;
    }
}

void Menu::statistics() {
        cout << "Available statistics:\n"
                "1 - Number of cities, reservoirs and stations\n"
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
                statistics();
                break;
        }
        mainMenu();
}


void Menu::basicServiceMetrics() {
    cout << "Basic Service Metrics: \n"
            "1 - Determine maximum water reach for each city;\n"
            "2 - Can an existing network configuration meet the water needs of its customers?;\n"
            "9 - Return to main menu;\n";
    cin >> input;
    if (!isDigit(input)) {
        cout << "Invalid input, try again\n";
        basicServiceMetrics();
        return;
    }
    switch (stoi(input)) {
        case 1:
            maxWaterReach();
            break;
        case 2:
            dataManager.citiesCapacity();
            mainMenu();
            break;
        case 9:
            break;
        default:
            cout << "Invalid input, try again\n";
            basicServiceMetrics();
            break;
    }
    mainMenu();
}

void Menu::maxWaterReach() {
    while (true) {
        // Display reservoir options for the user to choose as the source
        cout << "Select the water reservoir as the source:" << endl;
        int i = 1;
        unordered_map<int, string> reservoirChoices; // Map numerical choices to reservoir names
        for (auto &reservoir : dataManager.getReservoirs()) {
            cout << i << ": " << reservoir.second.getName() << endl;
            reservoirChoices[i] = reservoir.second.getCode(); // Store numerical choice and corresponding reservoir name
            i++;
        }
        int reservoirChoice;
        cin >> reservoirChoice;
        // Validate the user's input
        if (reservoirChoice < 1 || reservoirChoice > reservoirChoices.size()) {
            cout << "Invalid reservoir choice. Please try again." << endl;
            continue; // Restart the loop to prompt the user again
        }
        // Set the selected reservoir as the source
        string source = reservoirChoices[reservoirChoice]; // Get the reservoir name based on user's numerical choice

        // Display city options for the user to choose as the sink
        cout << "Select the city as the sink:" << endl;
        i = 1;
        unordered_map<int, string> cityChoices; // Map numerical choices to city names
        for (auto &city : dataManager.getCities()) {
            cout << i << ": " << city.second.getName() << endl;
            cityChoices[i] = city.second.getCode(); // Store numerical choice and corresponding city name
            i++;
        }
        int cityChoice;
        cin >> cityChoice;
        // Validate the user's input
        if (cityChoice < 1 || cityChoice > cityChoices.size()) {
            cout << "Invalid city choice. Please try again." << endl;
            continue; // Restart the loop to prompt the user again
        }
        // Set the selected city as the sink
        string sink = cityChoices[cityChoice]; // Get the city name based on user's numerical choice

        // Create a copy of the graph to avoid modifying the original graph
        Graph<string> graphCopy = dataManager.getGraph();

        // Call the edmondsKarp function to calculate the maximum flow
        DataManager::edmondsKarp(&graphCopy, source, sink);

        // Retrieve the maximum flow value from the sink city's vertex
        Vertex<string>* sinkVertex = graphCopy.findVertex(sink);
        double maxFlow = 0;
        for (auto& incomingEdge : sinkVertex->getIncoming()) {
            maxFlow += incomingEdge->getFlow();
        }

        // Display the maximum flow in the console
        cout << "Maximum flow from reservoir " << dataManager.getReservoirs().at(source).getName() << " to city " << dataManager.getCities().at(sink).getName() <<  ": " << maxFlow << endl;

        // Write the maximum flow to a file
        ofstream outputFile("max_flow_reservoir_to_city.txt", ios::app);
        if (outputFile.is_open()) {
            outputFile << "Maximum flow from reservoir " << dataManager.getReservoirs().at(source).getName() << " to city " << dataManager.getCities().at(sink).getName() <<  ": " << maxFlow << endl;
            outputFile.close();
            cout << "Results written to max_flow_reservoir_to_city.txt" << endl;
        }
        else {
            cout << "Unable to open file for writing" << endl;
        }

        break;
    }
}

void Menu::configurations() {
    cout << "Configurations:\n"
            "1 - Print max_flow_reservoir_to_city.txt\n"
            "2 - Clear max_flow_reservoir_to_city.txt\n"
            "9 - Return to main menu;\n";
    cin >> input;
    if (!isDigit(input)) {
        cout << "Invalid input, try again\n";
        configurations();
        return;
    }
    switch (stoi(input)) {
        case 1: {
            ifstream file("max_flow_reservoir_to_city.txt");
            if (file.is_open()) {
                string line;
                while (getline(file, line)) {
                    cout << line << endl;
                }
                file.close();
            }
            else {
                cout << "Unable to open file for reading" << endl;
            }
            mainMenu();
            break;
        }
        case 2: {
            ofstream outputFile("max_flow_reservoir_to_city.txt");
            if (outputFile.is_open()) {
                outputFile << "";
                outputFile.close();
                cout << "max_flow_reservoir_to_city.txt cleared" << endl;
            }
            else {
                cout << "Unable to open file for writing" << endl;
            }
            mainMenu();
            break;
        }
        case 9:
            mainMenu();
            break;
        default:
            cout << "Invalid input, try again\n";
            configurations();
            break;
    }
}

