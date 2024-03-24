//
// Created by guire on 07/03/2024.
//

#ifndef DA2324_PRJ1_G29_CITY_H
#define DA2324_PRJ1_G29_CITY_H

#include "Graph.h"
#include <string>

class City {

    private:
        std::string name;
        std::string id;
        std::string code;
        double demand;
        int population;

    public:
        City(std::string id, std::string code, std::string name, double demand, int population) :
            id(id), code(code), name(name), demand(demand), population(population) {}

        std::string getName() const { return name; }

        std::string getId() const { return id; }

        std::string getCode() const { return code; }

        double getDemand() const { return demand; }

        int getPopulation() const { return population; }

        void setDemand(double demand) { this->demand = demand; }

        void setPopulation(int population) { this->population = population; }

        void setName(std::string name) { this->name = name; }

        void setId(int id) { this->id = id; }

        void setCode(std::string code) { this->code = code; }
};

#endif //DA2324_PRJ1_G29_CITY_H
