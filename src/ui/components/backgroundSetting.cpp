/**
 ******************************************************************************
 * @file           : backgroundSetting.cpp
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/4/16
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_BackgroundSetting.h" resolved

#include <QDir>
#include <filesystem>
#include "backgroundSetting.h"
#include "ui_BackgroundSetting.h"
#include "mapForQObject.h"
#include "backgroundWidget.h"
#include "gameSettings.h"

BackgroundSetting::BackgroundSetting(QWidget *parent) :
    QWidget(parent), ui(new Ui::BackgroundSetting) {
    ui->setupUi(this);

    init();
    signalsProcess();
}

BackgroundSetting::~BackgroundSetting() {
    delete ui;
}

void BackgroundSetting::saveSettings() {
    QString backgroundFilePath = ui->comboBox->currentData().toString();
    auto object = MapForQObject::getInstance()->getObject(TypeName<BackgroundWidget>::get());
    auto backgroundWidget = qobject_cast<BackgroundWidget *>(object);
    backgroundWidget->setBackground(backgroundFilePath);

    GameSettings::getInstance()->setSetting(GameSettings::getInstance()->m_backgroundWebPath,
                                            QJsonValue(ui->comboBox->currentData().toString()));
}

void BackgroundSetting::init() {
    QDir dir(m_path);
    QStringList dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    foreach (const QString &dirName, dirs) {
        QFile file(m_path + "/" + dirName + "/" + "index.html");
        QString absolutePath = std::string(std::filesystem::absolute(file.filesystemFileName()).string()).c_str();
        ui->comboBox->addItem(dirName, absolutePath);
    }

    QString webFilePath = GameSettings::getInstance()->getSetting(GameSettings::getInstance()->m_backgroundWebPath).toString();
    if (!webFilePath.isEmpty()) {
        foreach (const QString &dirName, dirs) {
            if (webFilePath.contains(dirName)) {
                ui->comboBox->setCurrentText(dirName);
            }
        }
    }
}

void BackgroundSetting::signalsProcess() {
    static int preIndex = -1;
    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, [&](int index) {
        if (index != preIndex) {
            emit this->sigSettingsChanged();
            preIndex = index;
        }
    });
}
