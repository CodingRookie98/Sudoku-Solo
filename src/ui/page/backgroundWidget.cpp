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
#include <QDir>
#include "backgroundWidget.h"
#include "ui_BackgroundWidget.h"
#include "mapForQObject.h"
#include "logger.h"
#include "gameSettings.h"

BackgroundWidget::BackgroundWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::BackgroundWidget) {
    ui->setupUi(this);

    m_cefSetting = new QCefSetting;
    m_cefView = nullptr;

    init();
    signalProcess();
}

BackgroundWidget::~BackgroundWidget() {
    delete ui;
    delete m_cefSetting;
    delete m_cefView;
}

void BackgroundWidget::init() {
    MapForQObject::getInstance()->registerObject(TypeName<BackgroundWidget>::get(), this);
    //    ui->gridLayout_2->addWidget(m_cefView);

    QString webFilePath = GameSettings::getInstance()->getSetting(GameSettings::getInstance()->m_backgroundWebPath).toString();
    QString url = QString("file://");
    if (!webFilePath.isEmpty()) {
        delete m_cefView;
        url += QDir(webFilePath).absolutePath();

    } else {
        QFile file(QString("./background/geometry/") + "index.html");
        QString absolutePath = std::string(std::filesystem::absolute(file.filesystemFileName()).string()).c_str();
        url += QDir(webFilePath).absolutePath();
        
        GameSettings::getInstance()->setSetting(GameSettings::getInstance()->m_backgroundWebPath,
                                                absolutePath);

        Logger::getInstance()->log(Logger::LogLevel::Error, QString(__FUNCTION__) + " "
                                                                + QString::number(__LINE__) + " "
                                                                + GameSettings::getInstance()->m_backgroundWebPath
                                                                + " is not exits");
    }
    m_cefView = new QCefView(url, m_cefSetting);
    ui->gridLayout_2->addWidget(m_cefView);
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
    
    
    QString url = QString("file://") + QDir(path).absolutePath();
    if (m_cefView != nullptr) {
        ui->gridLayout_2->removeWidget(m_cefView);
        delete m_cefView;
    }
    m_cefView = new QCefView(url, m_cefSetting);
    ui->gridLayout_2->addWidget(m_cefView);
}
