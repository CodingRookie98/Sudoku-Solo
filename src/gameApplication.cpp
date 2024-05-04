/**
 ******************************************************************************
 * @file           : gameapplication.cpp
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 24-4-28
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_GameApplication.h" resolved

#include "gameApplication.h"
#include "gameSettings.h"
#include "bgmPlayer.h"

GameApplication::GameApplication(int &argc, char **argv) :
    QApplication(argc, argv) {
    m_gameResourceCheckWidget = new GameResourceCheckWidget;
    m_mainWindow = nullptr;

    init();
    signalsProcess();
}

GameApplication::~GameApplication() {
    delete m_gameResourceCheckWidget;
    delete m_mainWindow;
}

void GameApplication::signalsProcess() {
    connect(m_gameResourceCheckWidget, &GameResourceCheckWidget::sigCheckResourceFinished, [&] {
        if (m_mainWindow == nullptr) {
            m_mainWindow = new MainWindow;
        }

        m_gameResourceCheckWidget->close();
        m_mainWindow->showFullScreen();

        GameSettings::getInstance()->preCheck();
        BGMPlayer::getInstance()->play();

        delete m_gameResourceCheckWidget;
        m_gameResourceCheckWidget = nullptr;
    });
}

void GameApplication::start() {
    m_gameResourceCheckWidget->show();
    m_gameResourceCheckWidget->start();
}

void GameApplication::init() {
}
