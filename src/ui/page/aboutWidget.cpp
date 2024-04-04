/**
 ******************************************************************************
 * @file           : aboutWidget.cpp
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/4/4
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_AboutWidget.h" resolved

#include "aboutWidget.h"
#include "ui_AboutWidget.h"

AboutWidget::AboutWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::AboutWidget) {
    ui->setupUi(this);

    init();
    signalsProcess();
}

AboutWidget::~AboutWidget() {
    delete ui;
}

void AboutWidget::init() {
    ui->textBrowser->setSource(QUrl("qrc:/html/about.md"), QTextDocument::HtmlResource);
    // 设置 QTextBrowser 的样式表，将内容水平和垂直居中
    ui->textBrowser->setStyleSheet("QTextBrowser {"
                                   "    text-align: center;"     // 水平居中
                                   "    vertical-align: middle;" // 垂直居中
                                   "}");
    this->setWindowFlag(Qt::FramelessWindowHint);
}

void AboutWidget::signalsProcess() {
    connect(ui->btnClose, &QPushButton::clicked, this, [&] {
        emit sigClose();
    });
}

bool AboutWidget::event(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
        return true;
    }
    return QWidget::event(event);
}
