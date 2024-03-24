//
// Created by guire on 07/03/2024.
//

#include "DataManager.h"
#include "City.h"
#include "Pipe.h"
#include "Reservoir.h"
#include "Station.h"
#include "Graph.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

void DataManager::readCities() {
    ifstream file("Project1DataSetSmall/Cities_Madeira.csv");
    string line;
    getline(file, line); // Skip first line (header)
    while (getline(file, line)) {
        string name, id, code, demand, population;
        istringstream iss(line);
        getline(iss, name, ',');
        getline(iss, id, ',');
        getline(iss, code, ',');
        getline(iss, demand, ',');
        getline(iss, population, ',');
        City city(id, code, name, stod(demand), stoi(population));
        if (cities.find(id) == cities.end())
            cities.insert({id, city});
    }
}

void DataManager::readReservoirs() {
    ifstream file("Project1DataSetSmall/Reservoirs_Madeira.csv");
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
        if (reservoirs.find(id) == reservoirs.end())
            reservoirs.insert({id, reservoir});
    }
}

void DataManager::readStations() {
    ifstream file("Project1DataSetSmall/Stations_Madeira.csv");
    string line;
    getline(file, line); // Skip first line (header)
    while (getline(file, line)) {
        string id, code;
        istringstream iss(line);
        getline(iss, id, ',');
        getline(iss, code, ',');
        Station station(id, code);
        if (stations.find(id) == stations.end())
            stations.insert({id, station});
    }
}

void DataManager::readPipes() {
    for (auto &city: cities) {
        graph.addVertex(city.first);
    }
    for (auto &reservoir: reservoirs) {
        graph.addVertex(reservoir.first);
    }
    for (auto &station: stations) {
        graph.addVertex(station.first);
    }
    ifstream file("Project1DataSetSmall/Pipes_Madeira.csv");
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

DataManager::DataManager() {
    readCities();
    readReservoirs();
    readStations();
    readPipes();
}
