#include <string>
#include <utility>
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
                name(std::move(name)), municipality(std::move(municipality)), id(std::move(id)), code(std::move(code)), maxDelivery(maxDelivery) {}
        std::string getName() { return name; }
        std::string getMunicipality() { return municipality; }
        std::string getId() { return id; }
        std::string getCode() { return code; }
        double getMaxDelivery() const { return maxDelivery; }
        void setName(std::string name) { this->name = name; }
        void setMunicipality(std::string municipality) { this->municipality = municipality; }
        void setId(int id) { this->id = id; }
        void setCode(std::string code) { this->code = code; }
        void setMaxDelivery(double maxDelivery) { this->maxDelivery = maxDelivery; }
};

#endif //DA2324_PRJ1_G29_RESERVOIR_H