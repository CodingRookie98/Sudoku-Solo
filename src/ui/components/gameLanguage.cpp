/**
 ******************************************************************************
 * @file           : gameLanguage.cpp
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/4/11
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_GameLanguage.h" resolved

#include "gameLanguage.h"
#include "ui_GameLanguage.h"
#include "logger.h"
#include "gameSettings.h"

GameLanguage::GameLanguage(QWidget *parent) :
    QWidget(parent), ui(new Ui::GameLanguage) {
    ui->setupUi(this);
    m_translator = new QTranslator;

    init();
    signalsProcess();
}

GameLanguage::~GameLanguage() {
    delete ui;
    delete m_translator;
}

void GameLanguage::init() {
    ui->comboBox->addItem("简体中文", zh_CN);
    ui->comboBox->addItem("English", en_US);
}

void GameLanguage::signalsProcess() {
    static int preIndex = -1;
    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, [&](int index) {
        if (index != preIndex) {
            emit this->sigSettingsChanged();
            preIndex = index;
        }
    });
}

void GameLanguage::saveSettings() {
    QString languageFile = ui->comboBox->currentData().toString();
    if (!m_translator->load(languageFile)) {
        Logger::getInstance()->log(Logger::LogLevel::Error, QString(__FUNCTION__) + " "
                                                                + QString::number(__LINE__) + " "
                                                                + languageFile + " load failed.");
        return;
    }
    if (!QApplication::installTranslator(m_translator)) {
        Logger::getInstance()->log(Logger::LogLevel::Error, QString(__FUNCTION__) + " "
                                                                + QString::number(__LINE__) + " "
                                                                + "translator install failed.");
        return;
    }
    GameSettings::getInstance()->setSetting(GameSettings::getInstance()->m_language, languageFile);
}
