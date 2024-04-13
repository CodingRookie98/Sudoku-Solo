#include "GameSettingWidget.h"
#include "ui_GameSettingWidget.h"

GameSettingWidget::GameSettingWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::GameSettingWidget) {
    ui->setupUi(this);
    m_gameSoundSetting = new GameSoundSetting;
    m_gameLanguage = new GameLanguage;
    m_settingChangedClass = new std::unordered_set<SettingBase *>;

    init();
    signalsProcess();
    childSignalsProcess();
}

GameSettingWidget::~GameSettingWidget() {
    delete ui;
    delete m_gameSoundSetting;
    delete m_gameLanguage;
    delete m_settingChangedClass;
}

void GameSettingWidget::init() {
    ui->verticalLayout_2->insertWidget(0, m_gameSoundSetting); // 0
    ui->verticalLayout_2->insertWidget(1, m_gameLanguage);     // 1
}

void GameSettingWidget::signalsProcess() {
    connect(ui->btnOK, &QPushButton::clicked, this, [&] {
        emit sigFnished();
        applySettings();
    });

    connect(ui->btnCancel, &QPushButton::clicked, this, [&] {
        emit sigFnished();
    });

    connect(ui->btnApply, &QPushButton::clicked, this, [&] {
        applySettings();
    });
}

void GameSettingWidget::childSignalsProcess() {
    connect(m_gameSoundSetting, &GameSoundSetting::sigSettingsChanged, this, [&] {
        insertToChangedClass(m_gameSoundSetting);
    });

    connect(m_gameLanguage, &GameLanguage::sigSettingsChanged, this, [&] {
        insertToChangedClass(m_gameLanguage);
    });
}

void GameSettingWidget::applySettings() {
    for (auto &item : *m_settingChangedClass) {
        item->saveSettings();
    }
    m_settingChangedClass->clear();
}

void GameSettingWidget::insertToChangedClass(SettingBase *settingBase) {
    m_settingChangedClass->insert(settingBase);
}

bool GameSettingWidget::event(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    return QWidget::event(event);
}
