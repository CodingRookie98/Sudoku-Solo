/**
 ******************************************************************************
 * @file           : gamingWidget.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/4
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_GamingWidget.h" resolved

#include <thread>
#include "ui_GamingWidget.h"
#include "gamingWidget.h"
#include "gameManager.h"
#include "gameSettings.h"

GamingWidget::GamingWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::GamingWidget) {
    ui->setupUi(this);
    m_sudokuGridWidget = new SudokuGridWidget(this);
    m_timerForGameSpent = new QTimer;
    m_timerForMessageLabel = nullptr;

    init();
    signalsProcess();
}

GamingWidget::~GamingWidget() {
    delete ui;
    delete m_sudokuGridWidget;
    delete m_timerForGameSpent;
    delete m_timerForMessageLabel;
}

void GamingWidget::init() {
    ui->gridLayoutSudokuBoard->addWidget(m_sudokuGridWidget);
}

void GamingWidget::signalsProcess() {
    sudokuGridWidgetSignalsProcess();

    connect(ui->btnHint, &QPushButton::clicked, m_sudokuGridWidget, &SudokuGridWidget::hintAnswer);
    connect(m_timerForGameSpent, &QTimer::timeout, [&] {
        m_timeElapsed++;
        ui->labelTime->setText(QApplication::translate(metaObject()->className(), tr("时间 ").toStdString().c_str()) + QString::number(m_timeElapsed) + QString(" S"));
    });
}

void GamingWidget::generateSudokuMatrix(const Sudoku::SudokuMatrix::SudokuMatrixType &sudokuMatrixType) {
    m_sudokuGridWidget->generateRandomSudoku(sudokuMatrixType);
    initTimeElapsedAndMessageLabel();

    // 这句代码是为了让当前Widget获得焦点使快捷键生效
    this->setFocus(Qt::FocusReason::NoFocusReason);
}

void GamingWidget::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key::Key_Escape) {
        m_timerForGameSpent->stop();
        emit sigKeyEscRelease();
        event->accept();
    }
}

void GamingWidget::sudokuGridWidgetSignalsProcess() {
    connect(m_sudokuGridWidget, &SudokuGridWidget::sigMessageReadyToRead, this, [&](const SudokuGridWidget::Message &message) {
        switch (message) {
        case SudokuGridWidget::IsFilledButNotFinished:
            showMessage(QApplication::translate(metaObject()->className(),
                                                tr("数独已经填完但答案不正确，请仔细检查").toStdString().c_str()));
            break;
        case SudokuGridWidget::IsFilledAndFinished:
            onGameFinished();
            break;
        case SudokuGridWidget::PleaseSelectButtonToHint:
            showMessage(QApplication::translate(metaObject()->className(),
                                                tr("请选择需要提示答案的位置").toStdString().c_str()));
            break;
        }
    });
}

void GamingWidget::startTimeCount() {
    m_timerForGameSpent->start(1000);
}

void GamingWidget::saveGame() const {
    std::thread thread1(&SudokuGridWidget::saveCurrentSudoku, m_sudokuGridWidget, m_timeElapsed);
    thread1.detach();
}

void GamingWidget::loadLastGame() {
    SudokuGameData sudoData = GameManager::getInstance()->loadWithId(GameSettings::getInstance()->getSetting(GameSettings::getInstance()->m_lastGameId).toString());
    m_sudokuGridWidget->setGameMatrix(sudoData.m_answerMatrix, sudoData.m_originalMatrix, sudoData.m_workMatrix);

    initTimeElapsedAndMessageLabel();
}

void GamingWidget::initTimeElapsedAndMessageLabel() {
    m_timeElapsed = 0;
    ui->labelTime->setText(QApplication::translate(metaObject()->className(), tr("时间 0 S").toStdString().c_str()));
    startTimeCount();
}

void GamingWidget::onGameFinished() {
    m_sudokuGridWidget->setAllUnitDisable();
    m_timerForGameSpent->stop();
    // 存档
    saveGame();

    static QTimer countDown;
    // 显示游戏结束消息
    showMessage(QApplication::translate(metaObject()->className(), tr("游戏结束，10秒后开始下一局游戏。").toStdString().c_str()));
    // 这句代码是为了让当前Widget获得焦点使快捷键生效
    this->setFocus(Qt::FocusReason::NoFocusReason);
    countDown.start(10000);
    connect(&countDown, &QTimer::timeout, this, [&] {
        countDown.stop();
        generateSudokuMatrix(m_sudokuGridWidget->getSudokuMatrix().getSudokuType());
    });
}

void GamingWidget::showMessage(const QString &message) {
    ui->labelMessage->setText(message);

    if (m_timerForMessageLabel == nullptr) {
        m_timerForMessageLabel = new QTimer;
        connect(m_timerForMessageLabel, &QTimer::timeout, this, [&] {
            ui->labelMessage->setText("");
            m_timerForMessageLabel->stop();
        });
    }

    m_timerForMessageLabel->start(10000);
}
