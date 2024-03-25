//
// Created by guire on 07/03/2024.
//

#include "DataManager.h"
#include "Menu.h"

int main() {
    DataManager dataManager;
    Menu Menu(dataManager);
    Menu.displayMainMenu();
    return 0;
}