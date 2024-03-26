
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

void DataManager::citiesCapacity() {
    /*
    for (auto &i : reservoirs) {
        auto reservoir = graph.findVertex(i.first);
        cout << reservoir->getInfo() << " can supply ";
        int totalCapacity = 0;
        for (auto &edge : reservoir->getAdj()) {
            totalCapacity += edge->getWeight();
        }
        cout << min(totalCapacity, (int) i.second.getMaxDelivery()) << " to all of its cities" << endl;
    }
    */

    unordered_map<string, int> sites;
    for (auto &city : cities) {
        sites.insert({city.first, 0});
    }
    for (auto &vertex : graph.getVertexSet()) {
        for (auto &edge : vertex->getAdj()) {
            if (edge->getDest()->getType() == CITY) {
                sites[edge->getDest()->getInfo()] += edge->getWeight();
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
}

// TODO: cidades podem variar na sua capacidade (????)
