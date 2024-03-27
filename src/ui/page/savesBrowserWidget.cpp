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

#include <QMessageBox>
#include "savesBrowserWidget.h"
#include "ui_SavesBrowserWidget.h"
#include "sudokuMatrix.h"

SavesBrowserWidget::SavesBrowserWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::SavesBrowserWidget) {
    ui->setupUi(this);
    m_fileSystemModel = new QFileSystemModel;
    m_sudokuGameData = nullptr;

    init();
    signalsProcess();
}

SavesBrowserWidget::~SavesBrowserWidget() {
    delete ui;
    delete m_fileSystemModel;
    delete m_sudokuGameData;
}

void SavesBrowserWidget::init() {
    ui->filesView->setModel(m_fileSystemModel);
    m_fileSystemModel->setRootPath(QDir::currentPath() + "/save");
    ui->filesView->setRootIndex(m_fileSystemModel->index(QDir::currentPath() + "/save"));

    if (ui->lineEdit->text().isEmpty()) {
        ui->btnNewSave->setEnabled(false);
    }

    ui->btnDeleteSave->setEnabled(false);
    ui->btnNext->setEnabled(false);
    ui->btnPre->setEnabled(false);
}

void SavesBrowserWidget::signalsProcess() {
    connect(ui->btnNewSave, &QPushButton::clicked, this, &SavesBrowserWidget::creatNewSave);

    connect(ui->lineEdit, &QLineEdit::textChanged, this, [&](const QString &text) {
        if (text.isEmpty()) {
            ui->btnNewSave->setEnabled(false);
        } else {
            ui->btnNewSave->setEnabled(true);
        }
    });

    connect(ui->filesView->selectionModel(), &QItemSelectionModel::currentChanged, this, [&](const QModelIndex &current) {
        ui->btnDeleteSave->setEnabled(true);

        QString saveFilePath = m_fileSystemModel->filePath(current);
        auto gameData = GameManager::getInstance()->loadWithPath(saveFilePath);

        delete m_sudokuGameData;
        m_sudokuGameData = new std::vector<SudokuGameData>(gameData);
        m_indexForGameData = 0;

        if (!m_sudokuGameData->empty()) {
            ui->btnNext->setEnabled(true);
            ui->btnPre->setEnabled(true);
            updateTabWidget();
        } else {
            ui->btnNext->setEnabled(false);
            ui->btnPre->setEnabled(false);
        }
    });

    connect(ui->btnDeleteSave, &QPushButton::clicked, this, [&] {
        deleteSave(m_fileSystemModel->fileInfo(ui->filesView->currentIndex()).filePath());
    });

    connect(ui->btnCancel, &QPushButton::clicked, this, &SavesBrowserWidget::sigBackToHome);

    connect(ui->btnNext, &QPushButton::clicked, this, [&] {
        m_indexForGameData++;
        updateTabWidget();
    });

    connect(ui->btnPre, &QPushButton::clicked, this, [&] {
        m_indexForGameData--;
        updateTabWidget();
    });

    connect(ui->btnOK, &QPushButton::clicked, this, [&] {
       GameManager::getInstance()->setSaveFilePath(m_fileSystemModel->filePath(ui->filesView->currentIndex()));
       emit sigBackToHome();
    });
    
    connect(ui->btnContinueGame, &QPushButton::clicked, this, [&] {
    
    });
}

void SavesBrowserWidget::creatNewSave() {
    if (ui->lineEdit->text().isEmpty()) {
        ui->btnNewSave->setEnabled(false);
        return;
    }

    QString saveFileName = ui->lineEdit->text();
    QRegularExpression regex;

#ifdef Q_OS_WIN
    regex.setPattern(R"([\\/:*?"<>|])");
#endif

#ifdef Q_OS_LINUX
    regex.setPattern("[/\\\\\\0\"*?|<>:\\(\\) ]");
#endif

    if (regex.match(saveFileName).hasMatch()) {
        QMessageBox messageBox(this);
        messageBox.setText(QApplication::translate(metaObject()->className(), tr("文件名含有非法字符").toStdString().c_str()));
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
            // Todo 日志输出文件打开失败
            return;
        }
        file->close();
    }
    delete file;
}

void SavesBrowserWidget::deleteSave(const QString &filePath) {
    if (!QFile::remove(filePath)) {
        // Todo 日志: 删除文件失败
    }
}

void SavesBrowserWidget::updateTabWidget() {
    if (m_indexForGameData < 0) {
        m_indexForGameData = (unsigned int)m_sudokuGameData->size() - 1;
    } else if (m_indexForGameData >= m_sudokuGameData->size()) {
        m_indexForGameData = 0;
    }

    ui->tabAnswer->buildMatrix(*(m_sudokuGameData->at(m_indexForGameData).m_answerMatrix));
    ui->tabOriginal->buildMatrix(*(m_sudokuGameData->at(m_indexForGameData).m_originalMatrix));
    ui->tabWorker->buildMatrix(*(m_sudokuGameData->at(m_indexForGameData).m_workMatrix));

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
