/**
 ******************************************************************************
 * @file           : webEngineView.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/15
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_WebEngineView.h" resolved

#include <QKeyEvent>
#include <QFile>
#include <QWebEngineSettings>
#include "webEngineView.h"
#include "ui_WebEngineView.h"
#include "logger.h"

WebEngineView::WebEngineView(QWidget *parent) :
    QWebEngineView(parent), ui(new Ui::WebEngineView) {
    ui->setupUi(this);

    init();
}

WebEngineView::~WebEngineView() {
    delete ui;
}

void WebEngineView::init() {
//    this->settings()->setAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled, true);
//    this->settings()->setAttribute(QWebEngineSettings::WebGLEnabled, true);
}

void WebEngineView::setHtml(const QString &path) {
    QFile file(path);
    if (!file.exists()) {
        Logger::getInstance()->log(Logger::LogLevel::Error, QString(__FUNCTION__) + " "
                                                                + QString::number(__LINE__) + " "
                                                                + path + " is not exits");
        return;
    }
    QString localPath(std::string(absolute(file.filesystemFileName()).string()).c_str());
    this->load(QUrl::fromLocalFile(localPath));
}
