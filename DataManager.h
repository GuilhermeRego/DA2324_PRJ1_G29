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
        void readCities(int option);
        void readPipes(int option);
        void readReservoirs(int option);
        void readStations(int option);

        static void normalizePopulation(string &population);

        unordered_map<string, City> getCities() const { return cities; }
        unordered_map<string, Reservoir> getReservoirs() const { return reservoirs; }
        unordered_map<string, Station> getStations() const { return stations; }
        Graph<string> getGraph() { return graph; }

        unordered_map<string, int> citiesCapacity(bool b);

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
};


#endif //DA2324_PRJ1_G29_DATAMANAGER_H