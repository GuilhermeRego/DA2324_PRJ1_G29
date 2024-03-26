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
    cout << "\n\n-------------------------------------------------------------------------------------------------------\n"
            "Welcome to the Water Supply Management!\n"
            "Please choose one of the following options:\n"
            "1: Statistics;\n"
            "2: Determine maximum water reach for each city using Edmonds Karp;\n"
            "3: Can an existing network configuration meet the water needs of its customer?\n"
            "4: Reliability and Sensitivity to Failures;\n"
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
        case 2:
            maxWaterReach();
            break;
        case 3:
            dataManager.citiesCapacity();
            mainMenu();
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

// Function to test the given vertex 'w' and visit it if conditions are met
template <class T>
void testAndVisit(std::queue<Vertex<T>*> &q, Edge<T> *e, Vertex<T> *w, double residual) {
    // Check if the vertex 'w' is not visited and there is residual capacity
    if (!w->isVisited() && residual > 0) {
        // Mark 'w' as visited, set the path through which it was reached, and enqueue it
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

// Function to find an augmenting path using Breadth-First Search
template <class T>
bool findAugmentingPath(Graph<T> *g, Vertex<T> *s, Vertex<T> *t) {
    // Mark all vertices as not visited
    for (auto v : g->getVertexSet()) {
        v->setVisited(false);
    }
    // Mark the source vertex as visited and enqueue it
    s->setVisited(true);
    std::queue<Vertex<T> *> q;
    q.push(s);
    // BFS to find an augmenting path
    while (!q.empty() && !t->isVisited()) {
        auto v = q.front();
        q.pop();
        // Process outgoing edges
        for (auto e : v->getAdj()) {
            testAndVisit(q, e, e->getDest(), e->getWeight() - e->getFlow());
        }
        // Process incoming edges
        for (auto e : v->getIncoming()) {
            testAndVisit(q, e, e->getOrig(), e->getFlow());
        }
    }
    // Return true if a path to the target is found, false otherwise
    return t->isVisited();
}

// Function to find the minimum residual capacity along the augmenting path
template <class T>
double findMinResidualAlongPath(Vertex<T> *s, Vertex<T> *t) {
    double f = INF;
    // Traverse the augmenting path to find the minimum residual capacity
    for (auto v = t; v != s;) {
        auto e = v->getPath();
        if (e->getDest() == v) {
            f = std::min(f, e->getWeight() - e->getFlow());
            v = e->getOrig();
        } else {
            f = std::min(f, e->getFlow());
            v = e->getDest();
        }
    }
    // Return the minimum residual capacity
    return f;
}

// Function to augment flow along the augmenting path with the given flow value
template <class T>
void augmentFlowAlongPath(Vertex<T> *s, Vertex<T> *t, double f) {
    // Traverse the augmenting path and update the flow values accordingly
    for (auto v = t; v != s;) {
        auto e = v->getPath();
        double flow = e->getFlow();
        if (e->getDest() == v) {
            e->setFlow(flow + f);
            v = e->getOrig();
        } else {
            e->setFlow(flow - f);
            v = e->getDest();
        }
    }
}

// Main function implementing the Edmonds-Karp algorithm
template <class T>
void edmondsKarp(Graph<T> *g, string source, string target) {
    // Find source and target vertices in the graph
    Vertex<T> *s = g->findVertex(source);
    Vertex<T> *t = g->findVertex(target);
    // Validate source and target vertices
    if (s == nullptr || t == nullptr || s == t)
        throw std::logic_error("Invalid source and/or target vertex");
    // Initialize flow on all edges to 0
    for (auto v : g->getVertexSet()) {
        for (auto e : v->getAdj()) {
            e->setFlow(0);
        }
    }
    // While there is an augmenting path, augment the flow along the path
    while (findAugmentingPath(g, s, t)) {
        double f = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s, t, f);
    }
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
        Graph<std::string> graphCopy = dataManager.getGraph();

        // Call the edmondsKarp function to calculate the maximum flow
        edmondsKarp(&graphCopy, source, sink);

        // Retrieve the maximum flow value from the sink city's vertex
        Vertex<string>* sinkVertex = graphCopy.findVertex(sink);
        double maxFlow = 0;
        for (auto& incomingEdge : sinkVertex->getIncoming()) {
            maxFlow += incomingEdge->getFlow();
        }

        // Display the maximum flow in the console
        cout << "Maximum flow from reservoir to city: " << maxFlow << endl;

        // Write the maximum flow to a file
        ofstream outputFile("max_flow_reservoir_to_city.txt");
        if (outputFile.is_open()) {
            outputFile << "Maximum flow from reservoir to city: " << maxFlow << endl;
            outputFile.close();
            cout << "Results written to max_flow_reservoir_to_city.txt" << endl;
        } else {
            cout << "Unable to open file for writing" << endl;
        }

        // Ask the user if they want to perform another calculation
        cout << "\n"<< "Do you want to calculate maximum flow again? (y/n): ";
        char choice;
        cin >> choice;
        if (choice != 'y' && choice != 'Y') {
            break;
        }
        cout << "\n";
    }
}

