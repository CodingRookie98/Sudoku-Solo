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

#include "gamingWidget.h"
#include "ui_GamingWidget.h"

GamingWidget::GamingWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::GamingWidget) {
    ui->setupUi(this);
    m_sudokuGridWidget = new SudokuGridWidget(this);
    m_timerForGameSpent = nullptr;

    init();
    signalsProcess();
}

GamingWidget::~GamingWidget() {
    delete ui;
    delete m_sudokuGridWidget;
}

void GamingWidget::init() {
    ui->gridLayoutSudokuBoard->addWidget(m_sudokuGridWidget);
}

void GamingWidget::signalsProcess() {
    sudokuGridWidgetSignalsProcess();

    connect(ui->btnHint, &QPushButton::clicked, m_sudokuGridWidget, &SudokuGridWidget::hintAnswer);
}

void GamingWidget::generateSudokuMatrix(const Sudoku::SudokuMatrix::SudokuMatrixType &sudokuMatrixType) {
    m_sudokuGridWidget->generateRandomSudoku(sudokuMatrixType);
    if (m_timerForGameSpent == nullptr) {
        m_timerForGameSpent = new QTimer;
        connect(m_timerForGameSpent, &QTimer::timeout, [&]{
           m_timeElapsed++;
           ui->labelTime->setText(QString::number(m_timeElapsed));
        });
    }
    m_timeElapsed = 0;
    ui->labelTime->setText("0");
    m_timerForGameSpent->start(1000);
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
            ui->labelMessage->setText(QApplication::translate(metaObject()->className(),
                                                              tr("数独已经填完但答案不正确，请仔细检查").toStdString().c_str()));
            break;
        case SudokuGridWidget::IsFilledAndFinished:
            // Todo 游戏结束
            ui->labelMessage->setText(QApplication::translate(metaObject()->className(),
                                                              tr("游戏结束").toStdString().c_str()));
            break;
        case SudokuGridWidget::PleaseSelectButtonToHint:
            ui->labelMessage->setText(QApplication::translate(metaObject()->className(),
                                                              tr("请选择需要提示答案的位置").toStdString().c_str()));
            break;
        }
    });
}

void GamingWidget::startTimeCount() {
    m_timerForGameSpent->start(1000);
}

void GamingWidget::saveGame() const{
    m_sudokuGridWidget->saveCurrentSudoku();
}
