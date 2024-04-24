/**
 ******************************************************************************
 * @file           : main.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/3
 ******************************************************************************
 */
#include "ui/mainWindow.h"
#include <QApplication>
#include "gameSettings.h"
#include "bgmPlayer.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;

    GameSettings::getInstance()->preCheck();
    BGMPlayer::getInstance()->play();

    w.showFullScreen();
    return a.exec();
}
