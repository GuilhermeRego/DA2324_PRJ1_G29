//
// Created by guire on 07/03/2024.
//

#ifndef DA2324_PRJ1_G29_DATAMANAGER_H
#define DA2324_PRJ1_G29_DATAMANAGER_H

#include <unordered_map>
#include <string>
#include "City.h"
#include "Reservoir.h"
#include "Station.h"

using namespace std;

class DataManager {
public:
    DataManager();
    void readCities();
    void readPipes();
    void readReservoirs();
    void readStations();

    static void normalizePopulation(string &population);

    unordered_map<string, City> getCities() const { return cities; }
    unordered_map<string, Reservoir> getReservoirs() const { return reservoirs; }
    unordered_map<string, Station> getStations() const { return stations; }
    Graph<string> getGraph() const { return graph; }

    void citiesCapacity();
private:
    unordered_map<string, City> cities;
    unordered_map<string, Reservoir> reservoirs;
    unordered_map<string, Station> stations;
    Graph<string> graph;
};


#endif //DA2324_PRJ1_G29_DATAMANAGER_H