#include "DataManager.h"
#include "City.h"
#include "Reservoir.h"
#include "Station.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <unordered_map>

using namespace std;

DataManager::DataManager() {
    string option;
    do {
        cout << "Madeira or Portugal?\n"
                "1 - Madeira\n"
                "2 - Portugal\n";

        cin >> option;
    } while (option != "1" && option != "2");
    int optionInt = stoi(option);
    readCities(optionInt);
    readReservoirs(optionInt);
    readStations(optionInt);
    readPipes(optionInt);
}

/**
 * Time complexity: O(n)
 **/
void DataManager::normalizePopulation(string &population) {
    population.erase(remove(population.begin(), population.end(), ','), population.end());
    population.erase(remove(population.begin(), population.end(), '"'), population.end());
}

/**
 * Time complexity: O(n^2)
 **/
void DataManager::readCities(int option) {
    string csv;
    switch (option) {
        case 1:
            csv = "../data/Project1DataSetSmall/Cities_Madeira.csv";
            break;
        case 2:
            csv = "../data/Project1LargeDataSet/Cities.csv";
            break;
        default:
            cerr << "Invalid option" << endl;
            return;
    }
    ifstream file(csv);
    if (!file.is_open()) {
        cerr << "Failed to open csv" << endl;
        return;
    }
    string line;
    getline(file, line); // Skip first line (header)
    while (getline(file, line)) {
        string name, id, code, demand, population;
        istringstream iss(line);
        getline(iss, name, ',');
        getline(iss, id, ',');
        getline(iss, code, ',');
        getline(iss, demand, ',');
        getline(iss, population, '\n');
        normalizePopulation(population);
        City city(name, id, code, stod(demand), stoi(population));
        if (cities.find(code) == cities.end())
            cities.insert({code, city});
    }
}

void DataManager::readReservoirs(int option) {
    string csv;
    switch (option) {
        case 1:
            csv = "../data/Project1DataSetSmall/Reservoirs_Madeira.csv";
            break;
        case 2:
            csv = "../data/Project1LargeDataSet/Reservoir.csv";
            break;
        default:
            cerr << "Invalid option" << endl;
            return;
    }
    ifstream file(csv);
    if (!file.is_open()) {
        cerr << "Failed to open csv" << endl;
        return;
    }
    string line;
    getline(file, line); // Skip first line (header)
    while (getline(file, line)) {
        string name, municipality, id, code, maxDelivery;
        istringstream iss(line);
        getline(iss, name, ',');
        getline(iss, municipality, ',');
        getline(iss, id, ',');
        getline(iss, code, ',');
        getline(iss, maxDelivery, ',');
        Reservoir reservoir(name, municipality, id, code, stod(maxDelivery));
        if (reservoirs.find(code) == reservoirs.end())
            reservoirs.insert({code, reservoir});
    }
}

/**
 * Time complexity: O(n^2)
 **/
void DataManager::readStations(int option) {
    string csv;
    switch (option) {
        case 1:
            csv = "../data/Project1DataSetSmall/Stations_Madeira.csv";
            break;
        case 2:
            csv = "../data/Project1LargeDataSet/Stations.csv";
            break;
        default:
            cerr << "Invalid option" << endl;
            return;
    }
    ifstream file(csv);
    if (!file.is_open()) {
        cerr << "Failed to open csv" << endl;
        return;
    }
    string line;
    getline(file, line); // Skip first line (header)
    while (getline(file, line)) {
        string id, code;
        istringstream iss(line);
        getline(iss, id, ',');
        getline(iss, code, ',');
        if (!id.empty() || !code.empty()) {
            Station station(id, code);
            if (stations.find(code) == stations.end())
                stations.insert({code, station});
        }
    }
}

void DataManager::readPipes(int option) {
    for (auto &city: cities) {
        graph.addVertex(city.first);
    }
    for (auto &reservoir: reservoirs) {
        graph.addVertex(reservoir.first);
    }
    for (auto &station: stations) {
        graph.addVertex(station.first);
    }
    string csv;
    switch (option) {
        case 1:
            csv = "../data/Project1DataSetSmall/Pipes_Madeira.csv";
            break;
        case 2:
            csv = "../data/Project1LargeDataSet/Pipes.csv";
            break;
        default:
            cerr << "Invalid option" << endl;
            return;
    }
    ifstream file(csv);
    if (!file.is_open()) {
        cerr << "Failed to open csv" << endl;
        return;
    }

    string line;
    getline(file, line); // Skip first line (header)
    while (getline(file, line)) {
        string codeA, codeB, capacity, direction;
        istringstream iss(line);
        getline(iss, codeA, ',');
        getline(iss, codeB, ',');
        getline(iss, capacity, ',');
        getline(iss, direction, ',');
        graph.addEdge(codeA, codeB, stod(capacity));
        if (direction == "0") {
            bool hasEdge = false;
            auto dest = graph.findVertex(codeB);
            for (auto &edge: dest->getAdj()) {
                if (edge->getDest()->getInfo() == codeA) {
                    hasEdge = true;
                }
            }
            if (!hasEdge) {
                graph.addEdge(codeB, codeA, stod(capacity));
            }
        }
    }
}


/**
 * Time complexity: O(VE^2)
 **/
unordered_map<string, int> DataManager::citiesCapacity(bool printCitiesWithoutWater) {
    // Create a copy of the graph to avoid modifying the original graph
    Graph<string> graphCopy = getGraph().deepCopy();

    // Create a super source representing all reservoirs
    string superSource = "SuperSource";

    string superSink = "SuperSink";

    // Connect the super source to all reservoirs in the copy of the graph
    connectSuperSourceToReservoirs(superSource, graphCopy);

    connectSuperSinktoCity(superSink,graphCopy);

    edmondsKarp(&graphCopy, superSource, superSink);

    unordered_map<string, int> citiesCapacity;
    unordered_map<string, int> citiesWithoutWater;
    for (auto &city : getCities()) {
        string sink = city.second.getCode(); // Set the current city as the sink
        double demand = city.second.getDemand(); // Get the demand of the current city
        // Retrieve the maximum flow value from the sink city's vertex
        Vertex<string>* sinkVertex = graphCopy.findVertex(sink);
        double maxFlow = 0;
        for (auto& incomingEdge : sinkVertex->getIncoming()) {
            maxFlow += incomingEdge->getFlow();
        }
        citiesCapacity.insert({sink, (int) maxFlow});
        if(demand > maxFlow) {
            citiesWithoutWater[city.first] = (int) demand - (int) maxFlow;
        }
        // Display the maximum flow for the current city
        cout << city.first << " - " << city.second.getName() << ", Demand: " << cities.at(city.first).getDemand() <<", Flow: " << maxFlow << endl;
    }
    if (printCitiesWithoutWater){
        cout << endl << "Cities without enough flow:" << endl;
        for (auto &city: citiesWithoutWater) {
            cout << city.first << " - " << cities.at(city.first).getName() << "-> Demand: "
                 << cities.at(city.first).getDemand() << ", Flow: " << citiesCapacity.at(city.first) << ", Deficit: "
                 << city.second << endl;
        }
    }
    return citiesCapacity;
}

// Function to test the given vertex 'w' and visit it if conditions are met
template <class T>
void testAndVisit(queue<Vertex<T>*> &q, Edge<T> *e, Vertex<T> *w, double residual) {
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
    queue<Vertex<T> *> q;
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
            f = min(f, e->getWeight() - e->getFlow());
            v = e->getOrig();
        } else {
            f = min(f, e->getFlow());
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
/**
 * Time complexity: O(VE^2) -> Every function that calls edmodsKarp has a time complexity of O(VE^2)
 **/
void DataManager::edmondsKarp(Graph<string> *g, const string& source, const string& target) {
    // Find source and target vertices in the graph
    Vertex<string> *s = g->findVertex(source);
    Vertex<string> *t = g->findVertex(target);
    // Validate source and target vertices
    if (s == nullptr || t == nullptr || s == t)
        throw logic_error("Invalid source and/or target vertex");
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

void DataManager::reservoirOutCommission(Reservoir &reservoir, unordered_map<string, int> &oldSites) {
    Graph<string> graphCopy = graph.deepCopy();
    graphCopy.removeVertex(reservoir.getCode());
    // Create a super source representing all reservoirs
    string superSource = "SuperSource";

    string superSink = "SuperSink";

    // Connect the super source to all reservoirs in the copy of the graph
    connectSuperSourceToReservoirs(superSource, graphCopy);

    connectSuperSinktoCity(superSink,graphCopy);

    edmondsKarp(&graphCopy, superSource, superSink);

    unordered_map<string, int> newSites;
    for (auto &city : getCities()) {
        string sink = city.second.getCode(); // Set the current city as the sink

        // Retrieve the maximum flow value from the sink city's vertex
        Vertex<string> *sinkVertex = graphCopy.findVertex(sink);
        double maxFlow = 0;
        for (auto &incomingEdge: sinkVertex->getIncoming()) {
            maxFlow += incomingEdge->getFlow();
        }
        newSites.insert({sink, (int) maxFlow});
    }

    unordered_map<string, int> citiesWithoutWater;
    for (auto &site : newSites) {
        if (cities.find(site.first) != cities.end()) {
            cout << site.first << " - " << cities.at(site.first).getName() << ", Demand: " << cities.at(site.first).getDemand() << ", Flow: " << site.second << endl;
            if (site.second < cities.at(site.first).getDemand()) {
                citiesWithoutWater[site.first] = (int) cities.at(site.first).getDemand() - site.second;
            }
        }
    }
    cout << endl;
    bool changed = false;
    for (const auto& oldSite : oldSites) {
        int newSiteCapacity = newSites.at(oldSite.first);
        if (oldSite.second > newSiteCapacity) {
            cout << oldSite.first << " - " << cities.at(oldSite.first).getName() << " has lost " << oldSite.second - newSiteCapacity << " flow -> Old Flow: " << oldSite.second << " | New Flow: " << newSiteCapacity << endl;
            changed = true;
        }
        else if (oldSite.second < newSiteCapacity) {
            cout << oldSite.first << " - " << cities.at(oldSite.first).getName() << " got " << newSiteCapacity - oldSite.second << " flow -> Old Flow: " << oldSite.second << " | New Flow: " << newSiteCapacity << endl;
            changed = true;
        }
    }
    if (!changed) cout << "There was no changes on the cities' flow" << endl;
}

void DataManager::pumpingStationOutCommission(Station &station, unordered_map<string, int> &oldSites) {
    Graph<string> graphCopy = graph.deepCopy();
    graphCopy.removeVertex(station.getCode());
    // Create a super source representing all reservoirs
    string superSource = "SuperSource";

    string superSink = "SuperSink";

    // Connect the super source to all reservoirs in the copy of the graph
    connectSuperSourceToReservoirs(superSource, graphCopy);

    connectSuperSinktoCity(superSink,graphCopy);

    edmondsKarp(&graphCopy, superSource, superSink);

    unordered_map<string, int> newSites;
    for (auto &city : getCities()) {
        string sink = city.second.getCode(); // Set the current city as the sink
        // Retrieve the maximum flow value from the sink city's vertex
        Vertex<string> *sinkVertex = graphCopy.findVertex(sink);
        double maxFlow = 0;
        for (auto &incomingEdge: sinkVertex->getIncoming()) {
            maxFlow += incomingEdge->getFlow();
        }
        newSites.insert({sink, (int) maxFlow});
    }

    unordered_map<string, int> citiesWithoutWater;
    for (auto &site : newSites) {
        if (cities.find(site.first) != cities.end()) {
            cout << site.first << " - " << cities.at(site.first).getName() << ", Demand: " << cities.at(site.first).getDemand() << ", Flow: " << site.second << endl;
            if (site.second < cities.at(site.first).getDemand()) {
                citiesWithoutWater[site.first] = (int) cities.at(site.first).getDemand() - site.second;
            }
        }
    }
    cout << endl;
    bool changed = false;
    for (const auto& oldSite : oldSites) {
        int newSiteCapacity = newSites.at(oldSite.first);
        if (oldSite.second > newSiteCapacity) {
            cout << oldSite.first << " - " << cities.at(oldSite.first).getName() << " has lost " << oldSite.second - newSiteCapacity << " flow -> Old Flow: " << oldSite.second << " | New Flow: " << newSiteCapacity << endl;
            changed = true;
        }
        else if (oldSite.second < newSiteCapacity) {
            cout << oldSite.first << " - " << cities.at(oldSite.first).getName() << " got " << newSiteCapacity - oldSite.second << " flow -> Old Flow: " << oldSite.second << " | New Flow: " << newSiteCapacity << endl;
            changed = true;
        }
    }
    if (!changed) cout << "There was no changes on the cities' flow" << endl;
}

/**
 * Time complexity: O(V)
 **/
void DataManager::connectSuperSourceToReservoirs(const string &superSource, Graph<string> &graphCopy) const {
    /**
     * Add Vertex time complexity: O(1)
     */
    graphCopy.addVertex(superSource);
    // Iterate over all reservoirs and connect them to the super source in the copy of the graph
    for (auto &reservoir : getReservoirs()) {
        string reservoirCode = reservoir.second.getCode();
        double maxDelivery = reservoir.second.getMaxDelivery();
        // Add an edge from the super source to the reservoir in the copy of the graph
        /**
         * Add Edge time complexity: O(V)
         */
        graphCopy.addEdge(superSource, reservoirCode, maxDelivery);
    }
}

/**
 * Time complexity: O(V)
 **/
void DataManager::connectSuperSinktoCity(const string &supersink, Graph<string> &graphcopy) const {
    graphcopy.addVertex(supersink);
    // Iterate over all reservoirs and connect them to the super source in the copy of the graph
    for (auto &city : getCities()) {
        string cityCode = city.second.getCode();
        double demand = city.second.getDemand();
        // Add an edge from the super source to the reservoir in the copy of the graph
        graphcopy.addEdge(cityCode, supersink, demand);
    }
}

/**
 * Time complexity: O(V^4 * E^3)
 **/
void DataManager::pipelineFailures(unordered_map<string, int> &oldSites) {
    // Create a copy of the graph to avoid modifying the original graph
    Graph<string> graphCopy = graph.deepCopy();

    // Create a super source representing all reservoirs
    string superSource = "SuperSource";
    string superSink = "SuperSink";

    // Connect the super source to all reservoirs in the copy of the graph
    connectSuperSourceToReservoirs(superSource, graphCopy);

    // Connect the super sink to all cities in the copy of the graph
    connectSuperSinktoCity(superSink, graphCopy);

    unordered_map<string, string> pipelineCity;
    // Iterate over each vertex in the graph
    for (auto &vertex : graphCopy.getVertexSet()) {

        if(vertex->getInfo() == "SuperSource") continue;
        // Iterate over each adjacent edge of the current vertex
        for (auto &edge : vertex->getAdj()) {

            if(edge->getDest()->getInfo() == "SuperSink") continue;
            // Simulate the failure of the current pipeline by setting its flow capacity to zero
            double originalWeight = edge->getWeight();
            edge->setWeight(0);
            string pipelineCode = edge->getOrig()->getInfo() + " -> " + edge->getDest()->getInfo();
            cout << "\nWithout pipeline: " << pipelineCode << endl;

            // Run the Edmonds-Karp algorithm to find maximum flow
            edmondsKarp(&graphCopy, superSource, superSink);

            // Calculate new flow capacities after pipeline failure
            unordered_map<string, int> newSites;
            for (auto &city : getCities()) {
                string sink = city.second.getCode(); // Set the current city as the sink

                // Retrieve the maximum flow value from the sink city's vertex
                Vertex<string> *sinkVertex = graphCopy.findVertex(sink);
                double maxFlow = 0;
                for (auto &incomingEdge: sinkVertex->getIncoming()) {
                    maxFlow += incomingEdge->getFlow();
                }
                newSites.insert({sink, (int) maxFlow});
            }

            // Identify cities without enough water due to pipeline failure
            unordered_map<string, int> citiesWithoutWater;
            for (auto &site : newSites) {
                if (cities.find(site.first) != cities.end()) {
                    if (site.second < cities.at(site.first).getDemand()) {
                        citiesWithoutWater[site.first] = (int) cities.at(site.first).getDemand() - site.second;
                    }
                }
            }

            // Display changes in flow capacities for each city
            cout << endl;
            bool changed = false;
            for (const auto& oldSite : oldSites) {
                int newSiteCapacity = newSites.at(oldSite.first);
                if (oldSite.second > newSiteCapacity) {
                    cout << oldSite.first << " - " << cities.at(oldSite.first).getName() << " has lost " << oldSite.second - newSiteCapacity << " flow -> Old Flow: " << oldSite.second << " | New Flow: " << newSiteCapacity << endl;
                    changed = true;
                }
                else if (oldSite.second < newSiteCapacity) {
                    cout << oldSite.first << " - " << cities.at(oldSite.first).getName() << " got " << newSiteCapacity - oldSite.second << " flow -> Old Flow: " << oldSite.second << " | New Flow: " << newSiteCapacity << endl;
                    changed = true;
                }
                if (newSiteCapacity < cities.at(oldSite.first).getDemand()) {
                    pipelineCity[pipelineCode] = oldSite.first;
                }
            }
            if (!changed) cout << "There was no changes on the cities' flow" << endl;

            // Restore the flow capacity of the pipeline for the next iteration
            edge->setWeight(originalWeight);
        }
    }
    cout << "\nCities that without a pipeline can no longer deliver the desired amount of water:" << endl;
    for (const auto& pair : pipelineCity) {
        cout << "Pipeline: " << pair.first << " | City: " << pair.second << endl;
    }
}