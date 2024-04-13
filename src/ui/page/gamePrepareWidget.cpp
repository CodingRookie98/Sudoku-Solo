/**
 ******************************************************************************
 * @file           : gamePrepareWidget.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/11
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_GamePrepareWidget.h" resolved

#include "gamePrepareWidget.h"
#include "ui_GamePrepareWidget.h"
#include <QKeyEvent>

GamePrepareWidget::GamePrepareWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::GamePrepareWidget) {
    ui->setupUi(this);

    m_buttonGroup = new QButtonGroup;
    m_btnWithSudokuType = new std::map<QPushButton *, Sudoku::SudokuMatrix::SudokuMatrixType>;

    init();
    signalProcess();
}

GamePrepareWidget::~GamePrepareWidget() {
    delete ui;
    delete m_buttonGroup;
    delete m_btnWithSudokuType;
}

void GamePrepareWidget::init() {
    m_btnWithSudokuType->insert(std::make_pair(ui->btnFour, Sudoku::SudokuMatrix::SudokuMatrixType::FourFour));
    m_btnWithSudokuType->insert(std::make_pair(ui->btnSix, Sudoku::SudokuMatrix::SudokuMatrixType::SixSix));
    m_btnWithSudokuType->insert(std::make_pair(ui->btnNine, Sudoku::SudokuMatrix::SudokuMatrixType::NineNine));

    for (const auto &val : *m_btnWithSudokuType) {
        m_buttonGroup->addButton(val.first);
        val.first->setCheckable(true);
    }
    m_buttonGroup->setExclusive(true);
    ui->btnFour->setChecked(true);
}

void GamePrepareWidget::signalProcess() {
    connect(ui->btnStart, &QPushButton::clicked, this, [&] {
        emit sigStart(m_btnWithSudokuType->at(qobject_cast<QPushButton *>(m_buttonGroup->checkedButton())));
    });
    connect(ui->btnCancel, &QPushButton::clicked, this, &GamePrepareWidget::sigCancel);
}

void GamePrepareWidget::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key::Key_Escape) {
        emit sigKeyEscRelease();
        event->accept();
    }
}

bool GamePrepareWidget::event(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    return QWidget::event(event);
}
