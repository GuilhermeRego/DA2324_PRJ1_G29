//
// Created by diogo on 24/03/2024.
//

#ifndef DA2324_PRJ1_G29_MENU_H
#define DA2324_PRJ1_G29_MENU_H

#include "DataManager.h"
#include <iostream>

class Menu{
public:
    Menu(DataManager& dataManager);
    void displayMainMenu();

private:
    DataManager& dataManager;
    // Functions for Edmonds-Karp algorithm
    template <class T> void testAndVisit(std::queue<Vertex<T>*> &q, Edge<T> *e, Vertex<T> *w, double residual);
    template <class T> bool findAugmentingPath(Graph<T> *g, Vertex<T> *s, Vertex<T> *t);
    template <class T> double findMinResidualAlongPath(Vertex<T> *s, Vertex<T> *t);
    template <class T> void augmentFlowAlongPath(Vertex<T> *s, Vertex<T> *t, double f);
    template <class T> void edmondsKarp(Graph<T> *g, std::string source, std::string target);
};


#endif //DA2324_PRJ1_G29_MENU_H
