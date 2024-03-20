//
// Created by guire on 07/03/2024.
//

#ifndef DA2324_PRJ1_G29_PIPE_H
#define DA2324_PRJ1_G29_PIPE_H

#include "Point.h"

class Pipe {
    private:
        Point* source;
        Point* destination;
        int capacity;
        bool direction;
    public:
        Pipe(Point* source, Point* destination, int capacity, bool direction) :
            source(source), destination(destination), capacity(capacity), direction(direction) {}
        Point getSource() const { return *source; }
        Point getDestination() const { return *destination; }
        int getCapacity() const { return capacity; }
        bool getDirection() const { return direction; }
        void setSource(Point source) { this->source = &source; }
        void setDestination(Point destination) { this->destination = &destination; }
        void setCapacity(int capacity) { this->capacity = capacity; }
        void setDirection(bool direction) { this->direction = direction; }
};

#endif //DA2324_PRJ1_G29_PIPE_H
