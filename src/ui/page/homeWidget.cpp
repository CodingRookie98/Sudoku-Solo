/**
 ******************************************************************************
 * @file           : homeWidget.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/11
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_HomeWidget.h" resolved

#include "homeWidget.h"
#include "ui_HomeWidget.h"
#include "mapForQObject.h"
#include "gameSettings.h"

HomeWidget::HomeWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::HomeWidget) {
    ui->setupUi(this);

    init();
    signalProcess();
}

HomeWidget::~HomeWidget() {
    delete ui;
}

void HomeWidget::init() {
}

void HomeWidget::signalProcess() {
    connect(ui->btnNewGame, &QPushButton::clicked, this, &HomeWidget::sigNewGame);

    connect(ui->btnExit, &QPushButton::clicked, this, [&] {
        QApplication::closeAllWindows();
    });

    connect(ui->btnGameSaves, &QPushButton::clicked, this, &HomeWidget::sigGameSaves);

    connect(ui->btnSetting, &QPushButton::clicked, this, &HomeWidget::sigGameSetting);

    connect(GameSettings::getInstance(), &GameSettings::sigLastGameIsEmpty, this, [&] {
        ui->btnGameContinue->setVisible(false);
    });

    connect(ui->btnGameContinue, &QPushButton::clicked, this, &HomeWidget::sigLoadLastGame);
}
