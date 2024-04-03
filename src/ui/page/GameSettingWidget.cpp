#include "GameSettingWidget.h"
#include "ui_GameSettingWidget.h"

GameSettingWidget::GameSettingWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::GameSettingWidget) {
    ui->setupUi(this);
    m_gameSoundSetting = new GameSoundSetting;

    init();
    signalsProcess();
}

GameSettingWidget::~GameSettingWidget() {
    delete ui;
    delete m_gameSoundSetting;
}

void GameSettingWidget::init() {
    ui->verticalLayout_2->addWidget(m_gameSoundSetting);
}

void GameSettingWidget::signalsProcess() {
    connect(ui->btnOK, &QPushButton::clicked, this, [&] {
        emit sigFnished();
    });

    connect(ui->btnCancel, &QPushButton::clicked, this, [&] {
        emit sigFnished();
    });

    connect(ui->btnApply, &QPushButton::clicked, this, [&] {
        m_gameSoundSetting->saveSettings();
    });
}
