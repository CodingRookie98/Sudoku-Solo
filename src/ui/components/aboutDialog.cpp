/**
 ******************************************************************************
 * @file           : aboutDialog.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/29
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_AboutDialog.h" resolved

#include "aboutDialog.h"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::AboutDialog) {
    ui->setupUi(this);
    
    init();
    signalsProcess();
}

AboutDialog::~AboutDialog() {
    delete ui;
}

void AboutDialog::init() {
    this->setModal(true);
    ui->textBrowser->setSource(QUrl("qrc:/markdown/about.md"), QTextDocument::HtmlResource);
}

void AboutDialog::signalsProcess() {
    connect(ui->btnClose, &QPushButton::clicked, this, &QDialog::close);
}
