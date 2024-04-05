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
        void basicServiceMetrics();

    [[noreturn]] void  maxWaterReach();
        DataManager dataManager;
        static bool isDigit(const string &s);
        void configurations();
        void reliabilityAndSensitivity();
};

#endif //DA2324_PRJ1_G29_MENU_H
