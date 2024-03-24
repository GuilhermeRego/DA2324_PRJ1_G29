//
// Created by guire on 07/03/2024.
//

#ifndef DA2324_PRJ1_G29_PIPE_H
#define DA2324_PRJ1_G29_PIPE_H

#include "Graph.h"

class Pipe {
    private:
        std::string codeA;
        std::string codeB;
        int capacity;
        bool direction;
    public:
        Pipe(std::string codeA, std::string codeB, int capacity, bool direction) {
            this->codeA = codeA;
            this->codeB = codeB;
            this->capacity = capacity;
            this->direction = direction;
        }

        std::string getCodeA() {
            return codeA;
        }

        std::string getCodeB() {
            return codeB;
        }

        int getCapacity() {
            return capacity;
        }

        bool getDirection() {
            return direction;
        }
};

#endif //DA2324_PRJ1_G29_PIPE_H
