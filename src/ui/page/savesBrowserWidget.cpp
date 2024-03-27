/**
  ******************************************************************************
  * @file           : savesBrowserWidget.cpp
  * @author         : hui
  * @brief          : None
  * @attention      : None
  * @date           : 2024/3/21
  ******************************************************************************
  */

// You may need to build the project (run Qt uic code generator) to get "ui_SavesBrowserWidget.h" resolved

#include "savesBrowserWidget.h"
#include "ui_SavesBrowserWidget.h"


SavesBrowserWidget::SavesBrowserWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::SavesBrowserWidget) {
    ui->setupUi(this);
}

SavesBrowserWidget::~SavesBrowserWidget() {
    delete ui;
}
