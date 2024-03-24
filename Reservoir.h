//
// Created by guire on 07/03/2024.
//

#include <string>
#include "Graph.h"

#ifndef DA2324_PRJ1_G29_RESERVOIR_H
#define DA2324_PRJ1_G29_RESERVOIR_H

class Reservoir {
    private:
        std::string name;
        std::string municipality;
        std::string id;
        std::string code;
        double maxDelivery;

    public:
        Reservoir(std::string name, std::string municipality, std::string id, std::string code, double maxDelivery) :
                name(name), municipality(municipality), id(id), code(code), maxDelivery(maxDelivery) {}
        std::string getName() { return name; }
        std::string getMunicipality() { return municipality; }
        std::string getId() { return id; }
        std::string getCode() { return code; }
        double getMaxDelivery() { return maxDelivery; }
        void setName(std::string name) { this->name = name; }
        void setMunicipality(std::string municipality) { this->municipality = municipality; }
        void setId(int id) { this->id = id; }
        void setCode(std::string code) { this->code = code; }
        void setMaxDelivery(double maxDelivery) { this->maxDelivery = maxDelivery; }
};

#endif //DA2324_PRJ1_G29_RESERVOIR_H
