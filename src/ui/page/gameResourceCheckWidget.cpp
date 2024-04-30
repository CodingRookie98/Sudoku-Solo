/**
 ******************************************************************************
 * @file           : gameResourceCheckWidget.cpp
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 24-4-28
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_GameResourceCheckWidget.h" resolved

#include <QFile>
#include <QThreadPool>
#include <QApplication>
#include "gameResourceCheckWidget.h"
#include "ui_GameResourceCheckWidget.h"

GameResourceCheckWidget::GameResourceCheckWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::GameResourceCheckWidget) {
    ui->setupUi(this);
    m_resourceDownloader = new ResourceDownloader();
    m_filesSize = 0;
    m_neededDownloadFiles = nullptr;

    init();
    signalsProcess();
}

GameResourceCheckWidget::~GameResourceCheckWidget() {
    delete ui;
    delete m_neededDownloadFiles;
    delete m_resourceDownloader;
}

void GameResourceCheckWidget::init() {
    this->setWindowFlag(Qt::WindowType::FramelessWindowHint);

    ui->progressBar_check->setValue(0);
    ui->progressBar_download->setValue(0);
    m_filesSize = m_resourceDownloader->getFilesSize();
    ui->label_check_1->setText(QApplication::translate(metaObject()->className(), tr("正在验证程序要求：").toStdString().c_str()));
    ui->widget_download->setVisible(false);
}

void GameResourceCheckWidget::signalsProcess() {
    connect(m_resourceDownloader, &ResourceDownloader::sigUpdateProgressBar,
            this, &GameResourceCheckWidget::updateDownloadProgressBar);

    connect(ui->btnClose, &QPushButton::clicked, this, [] {
        QThreadPool::globalInstance()->clear();
        QThreadPool::globalInstance()->waitForDone(1000);
        QApplication::closeAllWindows();
    });
}

void GameResourceCheckWidget::start() {
    if (m_neededDownloadFiles == nullptr) {
        m_neededDownloadFiles = new std::map<int, std::string>;
    }
    // 获取需要下载的文件
    for (int i = 0; i < m_filesSize; ++i) {
        QString fileName(m_resourceDownloader->getFileName(i).c_str());
        if (!QFile::exists(fileName)) {
            (*m_neededDownloadFiles)[i] = fileName.toStdString();
        }
    }

    ui->progressBar_check->setValue((int)((m_filesSize - m_neededDownloadFiles->size()) * 100 / m_filesSize));

    if (!m_neededDownloadFiles->empty()) {
        static auto iter = m_neededDownloadFiles->cbegin();
        std::function<void(size_t, int64_t, int64_t, void *)> progressCallback = std::bind(&GameResourceCheckWidget::progressCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        std::function<void(const std::string &, std::function<void(size_t, int64_t, int64_t, void *)>)> downloadFunction =
            std::bind(&ResourceDownloader::downloadFile, m_resourceDownloader, iter->second, progressCallback);
        // 使用 lambda 表达式将 downloadFunction 包装成没有参数和返回值的函数
        auto task = [downloadFunction, fileName = iter->second, progressCallback]() {
            downloadFunction(fileName, progressCallback);
        };

        connect(m_resourceDownloader, &ResourceDownloader::sigFileDownloadFinished, this, [&] {
            iter++;
            ui->progressBar_check->setValue(ui->progressBar_check->value() + ((1 * 100) / m_filesSize));
            if (iter != m_neededDownloadFiles->cend()) {
                std::function<void(size_t, int64_t, int64_t, void *)> progressCallback = std::bind(&GameResourceCheckWidget::progressCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
                std::function<void(const std::string &, std::function<void(size_t, int64_t, int64_t, void *)>)> downloadFunction =
                    std::bind(&ResourceDownloader::downloadFile, m_resourceDownloader, iter->second, progressCallback);
                // 使用 lambda 表达式将 downloadFunction 包装成没有参数和返回值的函数
                auto task = [downloadFunction, fileName = iter->second, progressCallback]() {
                    downloadFunction(fileName, progressCallback);
                };
                QThreadPool::globalInstance()->start(task);
            } else { // 需要下载的文件全部下载完成
                delete m_neededDownloadFiles;
                m_neededDownloadFiles = nullptr;
                ui->progressBar_check->setValue(100);
                ui->widget_download->setVisible(false);
                emit sigCheckResourceFinished();
            }
        });

        connect(m_resourceDownloader, &ResourceDownloader::sigDownloadingFile, this, [&](const std::string &fileName) {
            ui->widget_download->setVisible(true);
            ui->label_check_2->setText(fileName.c_str());
            ui->label_download->setText(QApplication::translate(metaObject()->className(), tr("正在下载文件：").toStdString().c_str()) + fileName.c_str());
        });

        QThreadPool::globalInstance()->start(task);
    } else {
        emit sigCheckResourceFinished();
    }
}

void GameResourceCheckWidget::updateDownloadProgressBar(const int &value) {
    ui->progressBar_download->setValue(value);
}

void GameResourceCheckWidget::progressCallback(size_t increment, int64_t transfered, int64_t total, void *userData) {
    // increment表示本次回调接收的数据大小。
    // transferred表示已接收的数据大小。
    // total表示下载文件的总大小。
    int value = (int)((transfered * 100) / total);
    emit m_resourceDownloader->sigUpdateProgressBar(value);
}
