/**
 ******************************************************************************
 * @file           : savesBrowserWidget.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/21
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_SavesBrowserWidget.h" resolved

#include <QThreadPool>
#include "savesBrowserWidget.h"
#include "ui_SavesBrowserWidget.h"
#include "sudokuMatrix.h"
#include "mapForQObject.h"
#include "sudokuGridWidget.h"
#include "logger.h"
#include "gameMessageBox.h"
#include "gameSettings.h"

SavesBrowserWidget::SavesBrowserWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::SavesBrowserWidget) {
    ui->setupUi(this);
    m_fileSystemModel = new QFileSystemModel;
    m_sudokuGameData = nullptr;
    m_indexForGameData = 0;
    m_currentModelIndex = nullptr;
    m_fileSystemWatcher = new QFileSystemWatcher(this);

    init();
    signalsProcess();
}

SavesBrowserWidget::~SavesBrowserWidget() {
    delete ui;
    delete m_fileSystemModel;
    delete m_sudokuGameData;
    delete m_currentModelIndex;
    delete m_fileSystemWatcher;
}

void SavesBrowserWidget::init() {
    ui->filesView->setModel(m_fileSystemModel);
    m_fileSystemModel->setRootPath(QDir::currentPath() + "/save");
    ui->filesView->setRootIndex(m_fileSystemModel->index(QDir::currentPath() + "/save"));
}

void SavesBrowserWidget::signalsProcess() {
    connect(ui->btnNewSave, &QPushButton::clicked, this, &SavesBrowserWidget::creatNewSave);

    connect(ui->btnDeleteSave, &QPushButton::clicked, this, [&] {
        if (!ui->filesView->selectionModel()->hasSelection()) {
            GameMessageBox gameMessageBox(this);
            gameMessageBox.setMessage(QApplication::translate(metaObject()->className(), tr("请选择一个需要删除的存档").toStdString().c_str()));
            gameMessageBox.exec();
            return;
        }
        deleteSave(m_fileSystemModel->fileInfo(ui->filesView->currentIndex()).filePath());
    });

    connect(ui->btnCancel, &QPushButton::clicked, this, &SavesBrowserWidget::sigBackToPrevious);

    connect(ui->btnNext, &QPushButton::clicked, this, [&] {
        if (!ui->filesView->selectionModel()->hasSelection()) {
            GameMessageBox gameMessageBox(this);
            gameMessageBox.setMessage(QApplication::translate(metaObject()->className(), tr("请选择一个存档").toStdString().c_str()));
            gameMessageBox.exec();
            return;
        }
        m_indexForGameData++;
        updateTabWidget();
    });

    connect(ui->btnPre, &QPushButton::clicked, this, [&] {
        if (!ui->filesView->selectionModel()->hasSelection()) {
            GameMessageBox gameMessageBox(this);
            gameMessageBox.setMessage(QApplication::translate(metaObject()->className(), tr("请选择一个存档").toStdString().c_str()));
            gameMessageBox.exec();
            return;
        }
        m_indexForGameData--;
        updateTabWidget();
    });

    connect(ui->btnOK, &QPushButton::clicked, this, [&] {
        GameManager::getInstance()->setSaveFilePath(m_fileSystemModel->filePath(ui->filesView->currentIndex()));
        emit sigBackToPrevious();
    });

    connect(ui->btnContinueGame, &QPushButton::clicked, this, [&] {
        auto sudokuGridWidget = qobject_cast<SudokuGridWidget *>(MapForQObject::getInstance()->getObject(TypeName<SudokuGridWidget>::get()));
        if (sudokuGridWidget == nullptr) {
            // 从mapForQObject中取得的sudokuGridWidget是空指针
            Logger::getInstance()->log(Logger::Error, QString(__FUNCTION__) + " "
                                                          + QString::number(__LINE__) + " "
                                                          + "The sudokuGridWidget obtained from mapForQObject is a null pointer");
            return;
        } else {
            if (m_sudokuGameData == nullptr || m_sudokuGameData->empty()) {
                GameMessageBox gameMessageBox;
                gameMessageBox.setMessage(QApplication::translate(metaObject()->className(), tr("未选择存档或游戏").toStdString().c_str()));
                gameMessageBox.exec();
                return;
            }
            sudokuGridWidget->setGameMatrix(m_sudokuGameData->at(m_indexForGameData).m_answerMatrix,
                                            m_sudokuGameData->at(m_indexForGameData).m_originalMatrix,
                                            m_sudokuGameData->at(m_indexForGameData).m_workMatrix);
            emit sigStartGame();
        }
    });

    connect(m_fileSystemWatcher, &QFileSystemWatcher::fileChanged, this, [&](const QString &path) {
        auto func = [&, path] {
            if (QFile(path).exists() && path == m_fileSystemModel->filePath(*m_currentModelIndex)) {
                auto sudokuGameData = GameManager::getInstance()->loadWithPath(path);
                delete m_sudokuGameData;
                m_sudokuGameData = new std::vector<SudokuGameData>(sudokuGameData);
                updateLabelTextAndBtnStatus();
            }
        };
        QThreadPool::globalInstance()->start(func);
    });

    connect(ui->filesView, &QListView::clicked, this, [&](const QModelIndex &index) {
        if (m_currentModelIndex != nullptr && *m_currentModelIndex != index) {
            delete m_currentModelIndex;
            m_currentModelIndex = new QModelIndex(index);
            loadGame();
            updateBtnNextAndBtnPreStatus();
        } else if (m_currentModelIndex == nullptr) {
            m_currentModelIndex = new QModelIndex(index);
            loadGame();
            updateBtnNextAndBtnPreStatus();
        }
    });
}

void SavesBrowserWidget::creatNewSave() {
    QString saveFileName = ui->lineEdit->text();
    if (ui->lineEdit->text().isEmpty()) {
        saveFileName = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz");
    }

    QRegularExpression regex;

#ifdef Q_OS_WIN
    regex.setPattern(R"([\\/:*?"<>|])");
#endif

#ifdef Q_OS_LINUX
    regex.setPattern("[/\\\\\\0\"*?|<>:\\(\\) ]");
#endif

    if (regex.match(saveFileName).hasMatch()) {
        GameMessageBox messageBox(this);
        messageBox.setMessage(QApplication::translate(metaObject()->className(), tr("文件名含有非法字符").toStdString().c_str()));
        messageBox.exec();
        return;
    }

    if (!saveFileName.endsWith(".json")) {
        saveFileName += ".json";
    }

    saveFileName.push_front(QDir::currentPath() + "/save/");
    auto *file = new QFile(saveFileName);
    if (file->exists()) {
        ui->filesView->setCurrentIndex(m_fileSystemModel->index(saveFileName));
    } else {
        if (!file->open(QIODevice::WriteOnly)) {
            // 日志输出文件打开失败
            Logger::getInstance()->log(Logger::Error, QString(__FUNCTION__) + " "
                                                          + QString::number(__LINE__) + " "
                                                          + file->errorString());
            return;
        }
        file->close();
    }
    delete file;
}

void SavesBrowserWidget::deleteSave(const QString &filePath) {
    if (!QFile::remove(filePath)) {
        // 删除文件失败
        Logger::getInstance()->log(Logger::Error, QString(__FUNCTION__) + " "
                                                      + QString::number(__LINE__) + " "
                                                      + "delete " + filePath + " failed");
    }

    m_sudokuGameData->clear();
    updateTabWidget();
    // 如果删除的存档是上一次保存游戏的存档则发出信号，信号会被homeWidget接收，继续游戏按钮会被隐藏
    if (GameSettings::getInstance()->getSetting(GameSettings::getInstance()->m_lastGameSaveName).toString() == filePath) {
        emit GameSettings::getInstance() -> sigLastGameIsEmpty();
        GameSettings::getInstance()->setSetting(GameSettings::getInstance()->m_lastGameSaveName, QJsonValue());
    }
}

void SavesBrowserWidget::updateTabWidget() {
    updateLabelTextAndBtnStatus();

    if (m_sudokuGameData->empty()) {
        return;
    }
    ui->tabAnswer->buildMatrix(*(m_sudokuGameData->at(m_indexForGameData).m_answerMatrix));
    ui->tabOriginal->buildMatrix(*(m_sudokuGameData->at(m_indexForGameData).m_originalMatrix));
    ui->tabWorker->buildMatrix(*(m_sudokuGameData->at(m_indexForGameData).m_workMatrix));
}

void SavesBrowserWidget::updateBtnNextAndBtnPreStatus() {
    if (!ui->filesView->selectionModel()->hasSelection() || !m_currentModelIndex) {
        return;
    }

    if (m_sudokuGameData->empty()) {
        GameMessageBox messageBox(this);
        messageBox.setMessage(QApplication::translate(metaObject()->className(), tr("此存档尚未保存任何游戏").toStdString().c_str()));
        updateTabWidget();
        messageBox.exec();

        ui->btnNext->setEnabled(false);
        ui->btnPre->setEnabled(false);
    } else {
        ui->btnNext->setEnabled(true);
        ui->btnPre->setEnabled(true);
        updateTabWidget();
    }
}

void SavesBrowserWidget::loadGame() {
    QString saveFilePath = m_fileSystemModel->filePath(*m_currentModelIndex);
    m_fileSystemWatcher->addPath(saveFilePath);
    auto gameData = GameManager::getInstance()->loadWithPath(saveFilePath);

    delete m_sudokuGameData;
    m_sudokuGameData = new std::vector<SudokuGameData>(gameData);
}

void SavesBrowserWidget::updateLabelTextAndBtnStatus() {
    if (m_sudokuGameData->empty()) {
        m_indexForGameData = 0;
        ui->tabWorker->clear();
        ui->tabOriginal->clear();
        ui->tabAnswer->clear();

        ui->labelPlayDateTime->setText("");
        ui->labelSpentTime->setText("");
        ui->labelndex->setText("");
        ui->btnNext->setEnabled(false);
        ui->btnPre->setEnabled(false);
        return;
    }

    if (m_indexForGameData < 0) {
        m_indexForGameData = (unsigned int)m_sudokuGameData->size() - 1;
    } else if (m_indexForGameData >= m_sudokuGameData->size()) {
        m_indexForGameData = 0;
    }

    ui->btnNext->setEnabled(true);
    ui->btnPre->setEnabled(true);

    QString textPlayDateTime = QApplication::translate(metaObject()->className(), tr("上一次游玩: ").toStdString().c_str())
                               + m_sudokuGameData->at(m_indexForGameData).m_gameDatetime.toString("yyyy.MM.dd hh:mm:ss");
    QString textSpentTime = QApplication::translate(metaObject()->className(), tr("纪录: ").toStdString().c_str())
                            + QString::number(m_sudokuGameData->at(m_indexForGameData).m_gameSpentTime)
                            + QString(" s");
    QString textIndex = QString::number(m_indexForGameData + 1) + "/" + QString::number(m_sudokuGameData->size());

    ui->labelPlayDateTime->setText(textPlayDateTime);
    ui->labelSpentTime->setText(textSpentTime);
    ui->labelndex->setText(textIndex);
}
