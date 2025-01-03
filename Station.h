#ifndef DA2324_PRJ1_G29_STATION_H
#define DA2324_PRJ1_G29_STATION_H

#include "Graph.h"
#include <string>
#include <utility>

class Station {
private:
    std::string id;
    std::string code;
public:
    Station(std::string id, std::string code) :
            id(std::move(id)), code(std::move(code)) {}
    std::string getId() const { return id; }
    std::string getCode() const { return code; }
    void setId(int id) { this->id = id; }
    void setCode(std::string code) { this->code = code; }
};

#endif //DA2324_PRJ1_G29_STATION_H
