
//
// Created by guire on 07/03/2024.
//

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
    readCities();
    readReservoirs();
    readStations();
    readPipes();
}

void DataManager::normalizePopulation(string &population) {
    population.erase(remove(population.begin(), population.end(), ','), population.end());
    population.erase(remove(population.begin(), population.end(), '"'), population.end());
}

void DataManager::readCities() {
    ifstream file("../Project1DataSetSmall/Cities_Madeira.csv");
    if (!file.is_open()) {
        cerr << "Failed to open Cities_Madeira.csv" << endl;
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
        // cout << "City: " << name << " ID: " << id << " Code: " << code << " Demand: " << demand << " Population: " << population << endl;

        City city(name, id, code, stod(demand), stoi(population));
        if (cities.find(code) == cities.end())
            cities.insert({code, city});
    }
}

void DataManager::readReservoirs() {
    ifstream file("../Project1DataSetSmall/Reservoirs_Madeira.csv");
    if (!file.is_open()) {
        cerr << "Failed to open Reservoirs_Madeira.csv" << endl;
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

        // cout << "Reservoir: " << name << " Municipality: " << municipality << " ID: " << id << " Code: " << code << " Max Delivery: " << maxDelivery << endl;

        Reservoir reservoir(name, municipality, id, code, stod(maxDelivery));
        if (reservoirs.find(code) == reservoirs.end())
            reservoirs.insert({code, reservoir});
    }
}

void DataManager::readStations() {
    ifstream file("../Project1DataSetSmall/Stations_Madeira.csv");
    if (!file.is_open()) {
        cerr << "Failed to open Stations_Madeira.csv" << endl;
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
            // cout << "Station: " << id << " Code: " << code << endl;
            Station station(id, code);
            if (stations.find(code) == stations.end())
                stations.insert({code, station});
        }
    }
}

void DataManager::readPipes() {
    for (auto &city: cities) {
        // cout << city.second.getName() << endl;
        graph.addVertex(city.first);
    }
    for (auto &reservoir: reservoirs) {
        // cout << reservoir.second.getName() << endl;
        graph.addVertex(reservoir.first);
    }
    for (auto &station: stations) {
        // cout << station.second.getCode() << endl;
        graph.addVertex(station.first);
    }
    ifstream file("../Project1DataSetSmall/Pipes_Madeira.csv");
    if (!file.is_open()) {
        cerr << "Failed to open Pipes_Madeira.csv" << endl;
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
        // cout << "CodeA: " << codeA << " CodeB: " << codeB << " Capacity: " << capacity << " Direction: " << direction << endl;
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
    /*
    for (auto &vertex: graph.getVertexSet()) {
        cout << vertex->getInfo() << "->";
        for (auto &edge: vertex->getAdj()) {
            cout << edge->getDest()->getInfo() << " ";
        }
        cout << endl;
    }
    */
}


unordered_map<string, int> DataManager::citiesCapacity() {
    unordered_map<string, int> sites;
    for (auto &city : cities) {
        sites.insert({city.first, 0});
    }
    for (auto &vertex : graph.getVertexSet()) {
        for (auto &edge : vertex->getAdj()) {
            switch(edge->getDest()->getInfo()[0]) {
                case 'C':
                    sites[edge->getDest()->getInfo()] += edge->getWeight();
                    break;
            }
        }
    }
    unordered_map<string, int> citiesWithoutWater;
    for (auto &site : sites) {
        if (cities.find(site.first) != cities.end()) {
            cout << "City " << cities.at(site.first).getName() << " has a demand of " << cities.at(site.first).getDemand() << " and could receive a capacity of " << site.second << endl;
            if (site.second < cities.at(site.first).getDemand()) {
                citiesWithoutWater[site.first] = cities.at(site.first).getDemand() - site.second;
            }
        }
    }
    cout << endl;
    if (!citiesWithoutWater.empty()) {
        cout << "Cities without enough capacity:" << endl;
        for (auto &city: citiesWithoutWater) {
            cout << "City " << cities.at(city.first).getName() << " is missing " << city.second << " capacity" << endl;
        }
    }
    return sites;
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
void DataManager::edmondsKarp(Graph<string> *g, string source, string target) {
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
    unordered_map<string, int> newSites;
    for (auto &city : cities) {
        newSites.insert({city.first, 0});
    }
    for (auto &vertex : graphCopy.getVertexSet()) {
        for (auto &edge : vertex->getAdj()) {
            switch(edge->getDest()->getInfo()[0]) {
                case 'C':
                    newSites[edge->getDest()->getInfo()] += edge->getWeight();
                    break;
            }
        }
    }
    unordered_map<string, int> citiesWithoutWater;
    for (auto &site : newSites) {
        if (cities.find(site.first) != cities.end()) {
            cout << "City " << cities.at(site.first).getName() << " has a demand of " << cities.at(site.first).getDemand() << " and could receive a capacity of " << site.second << endl;
            if (site.second < cities.at(site.first).getDemand()) {
                citiesWithoutWater[site.first] = cities.at(site.first).getDemand() - site.second;
            }
        }
    }
    cout << endl;
    if (!citiesWithoutWater.empty()) {
        cout << "Cities without enough capacity:" << endl;
        for (auto &city: citiesWithoutWater) {
            cout << "City " << cities.at(city.first).getName() << " is missing " << city.second << " capacity" << endl;
        }
    }
}

void DataManager::connectSuperSourceToReservoirs(const string &superSource, Graph<string> &graphCopy) const {
    graphCopy.addVertex(superSource);
    // Iterate over all reservoirs and connect them to the super source in the copy of the graph
    for (auto &reservoir : getReservoirs()) {
        string reservoirCode = reservoir.second.getCode();
        double maxDelivery = reservoir.second.getMaxDelivery();
        // Add an edge from the super source to the reservoir in the copy of the graph
        graphCopy.addEdge(superSource, reservoirCode, maxDelivery);
    }
}
