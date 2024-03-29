/**
 ******************************************************************************
 * @file           : gameButton.cpp
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/28
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_GameButton.h" resolved

#include <QFile>
#include "gameButton.h"
#include "ui_GameButton.h"
#include "soundEffects.h"

GameButton::GameButton(QWidget *parent) :
    QPushButton(parent), ui(new Ui::GameButton) {
    ui->setupUi(this);

    init();
    signalsProcess();
}

GameButton::~GameButton() {
    delete ui;
}

void GameButton::init() {
    QFile styleFile(":/qss/gameButton.qss");
    styleFile.open(QFile::ReadOnly);
    QString style = QLatin1String(styleFile.readAll());
    this->setStyleSheet(style);
}

void GameButton::signalsProcess() {
    connect(this, &QPushButton::clicked, this, [] {
        SoundEffects::getInstance()->play(SoundEffects::getInstance()->m_click_001);
    });
}

void GameButton::enterEvent(QEnterEvent *event) {
    SoundEffects::getInstance()->play(SoundEffects::getInstance()->m_hover_34);
    QWidget::enterEvent(event);
}
