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

#include <QFile>
#include "backgroundWidget.h"
#include "ui_BackgroundWidget.h"
#include "mapForQObject.h"
#include "logger.h"
#include "gameSettings.h"

BackgroundWidget::BackgroundWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::BackgroundWidget) {
    ui->setupUi(this);

    m_webEngineView = new WebEngineView(this);

    init();
    signalProcess();
}

BackgroundWidget::~BackgroundWidget() {
    delete ui;
    delete m_webEngineView;
}

void BackgroundWidget::init() {
    MapForQObject::getInstance()->registerObject(TypeName<BackgroundWidget>::get(), this);
    ui->gridLayout_2->addWidget(m_webEngineView);

    QString webFilePath = GameSettings::getInstance()->getSetting(GameSettings::getInstance()->m_backgroundWebPath).toString();
    if (!webFilePath.isEmpty()) {
        m_webEngineView->setHtml(webFilePath);
    } else {
        QFile file(QString("./background/geometry/") + "index.html");
        QString absolutePath = std::string(std::filesystem::absolute(file.filesystemFileName()).string()).c_str();
        m_webEngineView->setHtml(absolutePath);

        GameSettings::getInstance()->setSetting(GameSettings::getInstance()->m_backgroundWebPath,
                                                absolutePath);

        Logger::getInstance()->log(Logger::LogLevel::Error, QString(__FUNCTION__) + " "
                                                                + QString::number(__LINE__) + " "
                                                                + GameSettings::getInstance()->m_backgroundWebPath
                                                                + " is not exits");
    }
}

void BackgroundWidget::signalProcess() {
}

void BackgroundWidget::setBackground(const QString &path) {
    QFile file(path);
    if (!file.exists()) {
        Logger::getInstance()->log(Logger::LogLevel::Error, QString(__FUNCTION__) + " "
                                                                + QString::number(__LINE__) + " "
                                                                + path + " is not exits");
        return;
    }
    m_webEngineView->setHtml(path);
}
