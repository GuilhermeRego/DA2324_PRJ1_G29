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
#include "Pipe.h"

class DataManager {
    public:
        DataManager();
        void readCities();
        void readPipes();
        void readReservoirs();
        void readStations();
        void normalizePopulation(std::string &population);
    private:
        std::unordered_map<std::string, City> cities;
        std::unordered_map<std::string, Reservoir> reservoirs;
        std::unordered_map<std::string, Station> stations;
        Graph<std::string> graph;
};


#endif //DA2324_PRJ1_G29_DATAMANAGER_H
