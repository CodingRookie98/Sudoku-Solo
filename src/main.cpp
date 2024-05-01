/**
 ******************************************************************************
 * @file           : main.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/3
 ******************************************************************************
 */

#include "gameApplication.h"

int main(int argc, char *argv[]) {
    GameApplication a(argc, argv);
    
    a.start();

    return a.exec();
}
