/**
 ******************************************************************************
 * @file           : webEngineView.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/15
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_WebEngineView.h" resolved

#include "webEngineView.h"
#include "ui_WebEngineView.h"
#include <QKeyEvent>

WebEngineView::WebEngineView(QWidget *parent) :
    QWebEngineView(parent), ui(new Ui::WebEngineView) {
    ui->setupUi(this);

    init();
}

WebEngineView::~WebEngineView() {
    delete ui;
}

void WebEngineView::init() {
//    this->load(QUrl("C:/Program Files (x86)/Steam/steamapps/workshop/content/431960/1321411879/index.html"));
}