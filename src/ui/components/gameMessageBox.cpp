/**
 ******************************************************************************
 * @file           : gameMessageBox.cpp
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/4/5
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_GameMessageBox.h" resolved

#include <QFile>
#include "gameMessageBox.h"
#include "ui_GameMessageBox.h"

GameMessageBox::GameMessageBox(QWidget *parent) :
    QDialog(parent), ui(new Ui::GameMessageBox) {
    ui->setupUi(this);

    init();
    signalsProcess();
}

GameMessageBox::~GameMessageBox() {
    delete ui;
}

void GameMessageBox::setMessage(const QString &message) {
    ui->messageLabel->setText(message);
}

void GameMessageBox::init() {
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setModal(true);

    QFile styleFile(":/qss/gameMessageBox.qss");
    QString style(styleFile.readAll());
    this->setStyleSheet(style);
    this->setWindowOpacity(0.8);
}

void GameMessageBox::signalsProcess() {
    connect(ui->btnClose, &QPushButton::clicked, this, &GameMessageBox::close);
}
