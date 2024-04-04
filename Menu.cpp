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
            reliabilityAndSensitivity();
            break;
        case 8:
            configurations();
            break;
        case 9:
            cout << "\nGoodbye!\n";
            exit(0);
        default:
            cout << "Invalid input, try again\n";
            mainMenu();
            break;
    }
}

void Menu::statistics() {
        cout << "\nAvailable statistics:\n"
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
                cout << "\nNumber of cities: " << dataManager.getCities().size() << endl;
                cout << "Number of reservoirs: " << dataManager.getReservoirs().size() << endl;
                cout << "Number of stations: " << dataManager.getStations().size() << endl;
                break;
            }
            case 2: {
                int totalPopulation = 0;
                for (auto &city: dataManager.getCities()) {
                    totalPopulation += city.second.getPopulation();
                }
                cout << "\nTotal population: " << totalPopulation << endl;
                break;
            }
            case 3: {
                double totalDemand = 0;
                for (auto &city: dataManager.getCities()) {
                    totalDemand += city.second.getDemand();
                }
                cout << "\nTotal demand: " << totalDemand << endl;
                break;
            }
            case 4: {
                double totalMaxDelivery = 0;
                for (auto &reservoir: dataManager.getReservoirs()) {
                    totalMaxDelivery += reservoir.second.getMaxDelivery();
                }
                cout << "\nAverage maximum delivery: " << totalMaxDelivery / (double) dataManager.getReservoirs().size()
                     << endl;
                break;
            }
            case 5: {
                cout << "\nCities ordered by population:\n";
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
                cout << "\nCities ordered by demand:\n";
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
                cout << "\nReservoirs ordered by maximum delivery:\n";
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
                cout << "\nConnections between service points (In the format \"source -> dest\"):\n";
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
                mainMenu();
                break;
            default:
                cout << "Invalid input, try again\n";
                statistics();
                break;
        }
        cout << "Press any key to continue..." << endl;
        cin.ignore();
        getchar();
        mainMenu();
}


void Menu::basicServiceMetrics() {
    cout << "\nBasic Service Metrics: \n"
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
        case 2:
            cout << endl;
            dataManager.citiesCapacity();
            mainMenu();
            break;
        case 9:
            mainMenu();
            break;
        default:
            cout << "Invalid input, try again\n";
            basicServiceMetrics();
            break;
    }
}

void Menu::maxWaterReach() {
    while (true) {
        // Create a copy of the graph to avoid modifying the original graph
        Graph<string> graphCopy = dataManager.getGraph().deepCopy();

        // Create a super source representing all reservoirs
        string superSource = "SuperSource";

        string superSink = "SuperSink";

        // Connect the super source to all reservoirs in the copy of the graph
        dataManager.connectSuperSourceToReservoirs(superSource, graphCopy);

        dataManager.connectSuperSinktoCity(superSink,graphCopy);

        DataManager::edmondsKarp(&graphCopy, superSource, superSink);

        // Display options for the user to choose
        cout << "Choose an option:" << endl;
        cout << "1 - Calculate maximum flow from reservoirs to each city" << endl;
        cout << "2 - Calculate overall maximum flow from reservoirs to all cities" << endl;
        cout << "3 - Exit" << endl;
        int option;
        cin >> option;

        switch (option) {
            case 1: {
                // Display city options for the user to choose as the sink
                cout << "Select the city as the sink:" << endl;
                int i = 1;
                unordered_map<int, string> cityChoices; // Map numerical choices to city names
                unordered_map<int, double> cityDemands; // Map numerical choices to city demands
                for (auto &city: dataManager.getCities()) {
                    cout << i << ": " << city.second.getName() << endl;
                    cityChoices[i] = city.second.getCode(); // Store numerical choice and corresponding city name
                    cityDemands[i] = city.second.getDemand(); // Store numerical choice and corresponding city demand
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
                double demand = cityDemands[cityChoice]; // Get the city demand based on user's numerical choice
                // Call the edmondsKarp function to calculate the maximum flow from the super source to the selected city

                // Retrieve the maximum flow value from the sink city's vertex
                Vertex<string> *sinkVertex = graphCopy.findVertex(sink);
                double maxFlow = 0;
                for (auto &incomingEdge: sinkVertex->getIncoming()) {
                    maxFlow += incomingEdge->getFlow();
                }

                // Display the maximum flow in the console
                cout << "Maximum flow from reservoirs to city " << dataManager.getCities().at(sink).getName() << ": " << maxFlow << endl;

                // Write the maximum flow to a file
                ofstream outputFile("max_flow_reservoir_to_city.txt", ios::app);
                if (outputFile.is_open()) {
                    outputFile << "Maximum flow from reservoirs to city " << dataManager.getCities().at(sink).getName() << ": " << min(demand, maxFlow) << endl;
                    outputFile.close();
                    cout << "Results written to max_flow_reservoir_to_city.txt" << endl;
                } else {
                    cout << "Unable to open file for writing" << endl;
                }
                mainMenu();
                break;
            }

            case 2: {
                // Display header for the results
                cout << "Maximum flow for each city:" << endl;

                double maxflowoverall = 0.0;

                // Iterate over all cities in the network
                for (auto &city : dataManager.getCities()) {
                    string sink = city.second.getCode(); // Set the current city as the sink
                    double demand = city.second.getDemand(); // Get the demand of the current city
                    // Call the edmondsKarp function to calculate the maximum flow for the current city


                    // Retrieve the maximum flow value from the sink city's vertex
                    Vertex<string>* sinkVertex = graphCopy.findVertex(sink);
                    double maxFlow = 0;
                    for (auto& incomingEdge : sinkVertex->getIncoming()) {
                        maxFlow += incomingEdge->getFlow();
                    }
                    maxflowoverall += maxFlow;
                    // Display the maximum flow for the current city
                    cout << city.first << " - " << city.second.getName() << ", Max Flow: " << maxFlow << endl;
                }
                cout << "Max Flow Overall: " << maxflowoverall   << endl;
                cout << "\n";
                mainMenu();
                break;
            }
            case 3: {
                mainMenu();
                break;
            }
            default: {
                cout << "Invalid option. Please try again." << endl;
                maxWaterReach();
            }
        }
    }
}

void Menu::checkPipelineFailures() {
    // Display header for the results
    cout << "Effect of Pipeline Failures:" << endl;

    // Create a copy of the graph to avoid modifying the original graph
    Graph<string> graphCopy = dataManager.getGraph().deepCopy();

    string superSource = "SuperSource";

    string superSink = "SuperSink";

    // Connect the super source to all reservoirs in the copy of the graph
    dataManager.connectSuperSourceToReservoirs(superSource, graphCopy);

    dataManager.connectSuperSinktoCity(superSink,graphCopy);

    DataManager::edmondsKarp(&graphCopy, superSource, superSink);

    // Iterate over each vertex (city or reservoir) in the network
    for (auto &vertex: graphCopy.getVertexSet()) {

        if (vertex->getInfo() == superSource) {
            continue;
        }
        // Iterate over the outgoing edges of the current vertex
        for (auto &edge: vertex->getAdj()) {
            string pipelineCode = edge->getOrig()->getInfo() + "-" +
                                  edge->getDest()->getInfo(); // Get a unique identifier for the pipeline

            // Temporarily set the flow capacity of the pipeline to zero to simulate a rupture
            double originalCapacity = edge->getWeight(); // Store the original capacity
            edge->setWeight(0); // Set capacity to zero

            // Display the name of the pipeline being examined
            cout << "Pipeline: " << pipelineCode << endl;

            // Flag to track if any city is affected
            bool anyAffected = false;

            // Iterate over each city in the network
            for (auto &city: dataManager.getCities()) {
                string cityCode = city.second.getCode(); // Get the city code
                double requiredRate = city.second.getDemand(); // Get the required water rate for the city

                // Retrieve the maximum flow value from the city's vertex
                Vertex<string> *cityVertex = graphCopy.findVertex(cityCode);
                double maxFlow = 0;
                for (auto &incomingEdge: cityVertex->getIncoming()) {
                    maxFlow += incomingEdge->getFlow();
                }
                // Call the Edmonds-Karp algorithm to calculate the flow from reservoirs to the city
                double deliveredCapacity = maxFlow;

                // Compare the delivered capacity with the required rate
                if (deliveredCapacity < requiredRate) {
                    // Calculate the deficit
                    double deficit = requiredRate - deliveredCapacity;
                    // Display the affected city along with its deficit
                    cout << "City: " << city.second.getName() << ", Deficit: " << deficit << endl;
                    anyAffected = true; // Set flag indicating that at least one city is affected
                }
            }

            // Display a message if no city is affected by the pipeline failure
            if (!anyAffected) {
                cout << "No city is affected." << endl;
            }

            // Reset the flow capacity of the pipeline for the next iteration
            edge->setWeight(originalCapacity);

            // Display separator between different pipelines
            cout << "------------------------" << endl;
        }
    }
    cout << "Press any key to continue..." << endl;
    cin.ignore();
    getchar();
    mainMenu();
}

void Menu::configurations() {
    cout << "\nConfigurations:\n"
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

void Menu::reliabilityAndSensitivity() {
    cout << "\nReliability and Sensitivity to Failures: \n"
            "1 - Evaluate what happens in terms of the delivery capacity of the network if one specific water reservoir is out of commission;\n"
            "2 - Evaluate the effect of removing a single pumping station on the network's delivery capacity to all citites;\n"
            "3 - Check the effect of pipeline failures;\n"
            "9 - Return to main menu;\n";
    cin >> input;
    if (!isDigit(input)) {
        cout << "Invalid input, try again\n";
        reliabilityAndSensitivity();
        return;
    }
    switch (stoi(input)) {
        case 1 : {
            cout << "\nWrite the code of the water reservoir to be out of commission:\n";
            cin >> input;
            if (dataManager.getReservoirs().find(input) == dataManager.getReservoirs().end()) {
                cout << "Invalid reservoir code, try again\n";
                reliabilityAndSensitivity();
                return;
            }
            cout << "\nWith the reservoir:" << endl;
            unordered_map<string, int> sites = dataManager.citiesCapacity();
            cout << endl << "\nWithout the reservoir:" << endl;
            dataManager.reservoirOutCommission(dataManager.getReservoirs().at(input), sites);
            break;
        }
        case 2 : {
            cout << "\nWrite the code of the pumping station to be out of commission:\n";
            cin >> input;
            if (dataManager.getStations().find(input) == dataManager.getStations().end()) {
                cout << "Invalid reservoir code, try again\n";
                reliabilityAndSensitivity();
                return;
            }
            cout << "\nWith the Pumping Station:" << endl;
            unordered_map<string, int> sites = dataManager.citiesCapacity();
            cout << endl << "\nWithout the Pumping Station:" << endl;
            dataManager.pumpingStationOutCommission(dataManager.getStations().at(input), sites);
            break;
        }
        case 3:
            checkPipelineFailures();
            break;
        case 9:
            mainMenu();
            break;
    }
    cout << "Press any key to continue..." << endl;
    cin.ignore();
    getchar();
    mainMenu();
}
