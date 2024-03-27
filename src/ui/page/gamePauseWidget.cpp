/**
 ******************************************************************************
 * @file           : gamePauseWidget.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/12
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_GamePauseWidget.h" resolved

#include "gamePauseWidget.h"
#include "ui_GamePauseWidget.h"
#include <QKeyEvent>

GamePauseWidget::GamePauseWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::GamePauseWidget) {
    ui->setupUi(this);

    init();
    signalsProcess();
}

GamePauseWidget::~GamePauseWidget() {
    delete ui;
}

void GamePauseWidget::init() {
}

void GamePauseWidget::signalsProcess() {
    connect(ui->btnBackToGame, &QPushButton::clicked, this, [&] {
        emit sigBackToGame();
    });

    connect(ui->btnSaveGame, &QPushButton::clicked, this, [&] {
        emit sigSaveGame();
    });

    connect(ui->btnLoadGameSave, &QPushButton::clicked, this, [&] {
        emit sigLoadGameSave();
    });

    connect(ui->btnGameSetting, &QPushButton::clicked, this, [&] {
        emit sigGameSetting();
    });

    connect(ui->btnHome, &QPushButton::clicked, this, [&] {
        emit sigHome();
    });

    connect(ui->btnExitGame, &QPushButton::clicked, this, [&] {
        emit sigExitGame();
    });
}

void GamePauseWidget::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key::Key_Escape) {
        emit sigKeyEscRelease();
        event->accept();
    }
}
