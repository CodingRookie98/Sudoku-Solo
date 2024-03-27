/**
 ******************************************************************************
 * @file           : backgroundWidget.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/12
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get
// "ui_BackgroundWidget.h" resolved

#include "backgroundWidget.h"
#include "ui_BackgroundWidget.h"

BackgroundWidget::BackgroundWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::BackgroundWidget) {
    ui->setupUi(this);

    m_webEngineView = new WebEngineView(this);
//    m_webEngineView = nullptr;

    init();
    signalProcess();
}

BackgroundWidget::~BackgroundWidget() {
    delete ui;
    delete m_webEngineView;
}

void BackgroundWidget::init() {
    ui->gridLayout_2->addWidget(m_webEngineView);
}

void BackgroundWidget::signalProcess() {
    connect(m_webEngineView, &WebEngineView::sigKeyEscRelease, this, [&]{
    });
}