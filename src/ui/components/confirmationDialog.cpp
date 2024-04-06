/**
 ******************************************************************************
 * @file           : confirmationDialog.cpp
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/4/6
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_ConfirmationDialog.h" resolved

#include "confirmationDialog.h"
#include "ui_ConfirmationDialog.h"

ConfirmationDialog::ConfirmationDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::ConfirmationDialog) {
    ui->setupUi(this);

    init();
    signalsProcess();
}

ConfirmationDialog::~ConfirmationDialog() {
    delete ui;
}

void ConfirmationDialog::init() {
    this->setModal(true);
    this->setWindowOpacity(0.8);
    this->setWindowFlag(Qt::WindowType::FramelessWindowHint);
}

void ConfirmationDialog::signalsProcess() {
    connect(ui->btnYes, &QPushButton::clicked, this, [&] {
        emit sigYes();
        this->close();
    });

    connect(ui->btnNo, &QPushButton::clicked, this, [&] {
        emit sigNo();
        this->close();
    });
}

void ConfirmationDialog::setText(const QString &text) {
    ui->label->setText(text);
}
