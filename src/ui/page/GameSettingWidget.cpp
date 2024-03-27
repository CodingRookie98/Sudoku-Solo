#include "GameSettingWidget.h"
#include "ui_GameSettingWidget.h"

GameSettingWidget::GameSettingWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::GameSettingWidget) {
    ui->setupUi(this);

    init();
    signalsProcess();
}

GameSettingWidget::~GameSettingWidget() {
    delete ui;
}
void GameSettingWidget::init() {
}

void GameSettingWidget::signalsProcess() {
    connect(ui->btnOK, &QPushButton::clicked, this, [&] {
        emit sigFnished();
    });

    connect(ui->btnCancel, &QPushButton::clicked, this, [&] {
        emit sigFnished();
    });
}
