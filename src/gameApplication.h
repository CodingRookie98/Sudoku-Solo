/**
 ******************************************************************************
 * @file           : gameapplication.h
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 24-4-28
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_GAMEAPPLICATION_H_
#define SUDOKU_SOLO_SRC_GAMEAPPLICATION_H_

#include <QApplication>
#include "gameResourceCheckWidget.h"
#include "mainWindow.h"

class GameApplication : public QApplication {
    Q_OBJECT

public:
    explicit GameApplication(int &argc, char **argv);
    ~GameApplication() override;

    void init();
    void signalsProcess();
    void start();
    
private:
    GameResourceCheckWidget *m_gameResourceCheckWidget;
    MainWindow *m_mainWindow;
};

#endif // SUDOKU_SOLO_SRC_GAMEAPPLICATION_H_
