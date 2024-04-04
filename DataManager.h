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
        void readCities(int i);
        void readPipes(int i);
        void readReservoirs(int i);
        void readStations(int i);

        static void normalizePopulation(string &population);

        unordered_map<string, City> getCities() const { return cities; }
        unordered_map<string, Reservoir> getReservoirs() const { return reservoirs; }
        unordered_map<string, Station> getStations() const { return stations; }
        Graph<string> getGraph() { return graph; }

        unordered_map<string, int> citiesCapacity();

        static void edmondsKarp(Graph<string> *g, const string& source, const string& target);

        void reservoirOutCommission(Reservoir &reservoir, unordered_map<string, int> &map);
        void pumpingStationOutCommission(Station &station, unordered_map<string, int> &oldSites);
        void connectSuperSourceToReservoirs(const string& superSource, Graph<string>& graphCopy) const;
        void connectSuperSinktoCity(const string &supersink, Graph<string> &graphcopy) const;
        void pipelineFailures(unordered_map<string, int> &oldSites);
    private:
        unordered_map<string, City> cities;
        unordered_map<string, Reservoir> reservoirs;
        unordered_map<string, Station> stations;
        Graph<string> graph;

    void evaluateReservoirImpact();


};


#endif //DA2324_PRJ1_G29_DATAMANAGER_H