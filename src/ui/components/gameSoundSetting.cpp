/**
 ******************************************************************************
 * @file           : gameSoundSetting.cpp
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/4/3
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_GameSoundSetting.h" resolved

#include "gameSoundSetting.h"
#include "ui_GameSoundSetting.h"
#include "gameSettings.h"
#include "soundEffects.h"
#include "bgmPlayer.h"

GameSoundSetting::GameSoundSetting(QWidget *parent) :
    QWidget(parent), ui(new Ui::GameSoundSetting) {
    ui->setupUi(this);

    init();
    signalsProcess();
}

GameSoundSetting::~GameSoundSetting() {
    delete ui;
}

void GameSoundSetting::init() {
    ui->buttonSoundSlider->setTickPosition(QSlider::TicksAbove);
    ui->buttonSoundSlider->setRange(0, 100);
    ui->buttonSoundSlider->setSingleStep(10);
    ui->buttonSoundSpinBox->setRange(0, 100);

    ui->bgmSlider->setTickPosition(QSlider::TicksAbove);
    ui->bgmSlider->setRange(0, 100);
    ui->bgmSlider->setTickInterval(10);
    ui->bgmSpinBox->setRange(0, 100);

    QJsonValue soundEffectValue = GameSettings::getInstance()->getSetting(GameSettings::getInstance()->m_soundEffectVolume);
    QJsonValue bgmValue = GameSettings::getInstance()->getSetting(GameSettings::getInstance()->m_backgroundMusicVolume);
    if (!soundEffectValue.isNull()) {
        ui->buttonSoundSpinBox->setValue(soundEffectValue.toInt());
        ui->buttonSoundSlider->setValue(soundEffectValue.toInt());
    } else {
        ui->buttonSoundSpinBox->setValue(ui->buttonSoundSpinBox->maximum());
        ui->buttonSoundSlider->setValue(ui->buttonSoundSlider->maximum());
    }
    if (!bgmValue.isNull()) {
        ui->bgmSpinBox->setValue(bgmValue.toInt());
        ui->bgmSlider->setValue(bgmValue.toInt());
    } else {
        ui->bgmSpinBox->setValue(ui->bgmSpinBox->maximum());
        ui->bgmSlider->setValue(ui->bgmSlider->maximum());
    }
}

void GameSoundSetting::signalsProcess() {
    connect(ui->buttonSoundSlider, &QSlider::valueChanged, ui->buttonSoundSpinBox, &QSpinBox::setValue);
    connect(ui->buttonSoundSpinBox, &QSpinBox::valueChanged, ui->buttonSoundSlider, &QSlider::setValue);

    connect(ui->bgmSlider, &QSlider::valueChanged, ui->bgmSpinBox, &QSpinBox::setValue);
    connect(ui->bgmSpinBox, &QSpinBox::valueChanged, ui->bgmSlider, &QSlider::setValue);

    connect(ui->buttonSoundSpinBox, &QSpinBox::valueChanged, this, &GameSoundSetting::sigSettingsChanged);
    connect(ui->bgmSpinBox, &QSpinBox::valueChanged, this, &GameSoundSetting::sigSettingsChanged);
}

void GameSoundSetting::saveSettings() {
    float soundEffectVolume = (float)ui->buttonSoundSpinBox->value() / 100;
    float bgmVolume = (float)ui->bgmSpinBox->value() / 100;
    SoundEffects::getInstance()->setVolume(soundEffectVolume);
    BGMPlayer::getInstance()->setVolume(bgmVolume);

    GameSettings::getInstance()->setSetting(GameSettings::getInstance()->m_soundEffectVolume,
                                            QJsonValue(ui->buttonSoundSpinBox->value()));
    GameSettings::getInstance()->setSetting(GameSettings::getInstance()->m_backgroundMusicVolume,
                                            QJsonValue(ui->bgmSpinBox->value()));
}

bool GameSoundSetting::event(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    return QWidget::event(event);
}
