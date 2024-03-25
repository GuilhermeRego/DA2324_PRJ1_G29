//
// Created by guire on 24/03/2024.
//

#ifndef DA2324_PRJ1_G29_MENU_H
#define DA2324_PRJ1_G29_MENU_H

#include "DataManager.h"

class Menu {
public:
    Menu();
    void init();
private:
    void mainMenu();
    void statistics();
    void  maxWaterReach();
    DataManager dataManager;
    bool isDigit(const string &s);
};

#endif //DA2324_PRJ1_G29_MENU_H