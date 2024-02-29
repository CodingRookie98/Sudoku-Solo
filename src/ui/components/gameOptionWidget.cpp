/**
 ******************************************************************************
 * @file           : gameOptionWidget.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/4
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_GameOptionWidget.h" resolved

#include "gameOptionWidget.h"
#include "ui_GameOptionWidget.h"

GameOptionWidget::GameOptionWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::GameOptionWidget) {
    ui->setupUi(this);
    init();
    signalsProcess();
}

GameOptionWidget::~GameOptionWidget() {
    delete ui;
}

void GameOptionWidget::signalsProcess() {
    connect(ui->pBtnGenSudoku, &QPushButton::clicked, this, [&] {
        Sudoku::SudokuMatrix::SudokuMatrix::SudokuMatrixTypes type                                = (Sudoku::SudokuMatrix::SudokuMatrix::SudokuMatrixTypes)ui->sudokuType->currentData().toInt();
        Sudoku::SudokuGenerator::GenerateMethod generateMethod = (Sudoku::SudokuGenerator::GenerateMethod)ui->cbGenerateMethod->currentData().toInt();
        emit sigGenerateRandomSudoku(type, generateMethod, ui->qsbNum->value());
        // 游戏开始时以下按钮处于不可用状态
        enabledSudokuOperateBtns(true);
    });

    connect(ui->sudokuType, &QComboBox::currentIndexChanged, ui->qsbNum, [&]() {
        Sudoku::SudokuMatrix::SudokuMatrixTypes type = (Sudoku::SudokuMatrix::SudokuMatrixTypes)ui->sudokuType->currentData().toInt();
        ui->qsbNum->setRange(1, Sudoku::SudokuMatrix::SudokuMatrix::getMatrixSize(type));
    });

    connect(ui->btnCheck, &QPushButton::clicked, this, &GameOptionWidget::sigCheckCurrentSudoku);
    connect(ui->btnSolve, &QPushButton::clicked, this, &GameOptionWidget::sigSolveCurrentSudoku);
    connect(ui->btnSaveSudoku, &QPushButton::clicked, this, &GameOptionWidget::sigSaveCurrentSudoku);
    connect(ui->btnReadSudoku, &QPushButton::clicked, this, &GameOptionWidget::sigReadSudokuFromFile);
    connect(ui->btnPromptAnswer, &QPushButton::clicked, this, &GameOptionWidget::sigPromtAnswer);
    connect(ui->btnInputSudoku, &QPushButton::clicked, this, &GameOptionWidget::sigInputSudoku);
}

void GameOptionWidget::init() {
    ui->sudokuType->addItem("9*9", QVariant(Sudoku::SudokuMatrix::SudokuMatrixTypes::Nine_Nine));
    ui->sudokuType->addItem("6*6", QVariant(Sudoku::SudokuMatrix::SudokuMatrixTypes::Six_Six));
    ui->sudokuType->addItem("4*4", QVariant(Sudoku::SudokuMatrix::SudokuMatrixTypes::Four_Four));

    Sudoku::SudokuMatrix::SudokuMatrixTypes type = (Sudoku::SudokuMatrix::SudokuMatrixTypes)ui->sudokuType->currentData().toInt();
    ui->qsbNum->setRange(1, Sudoku::SudokuMatrix::SudokuMatrix::getMatrixSize(type));

    ui->cbGenerateMethod->addItem("递归", QVariant(Sudoku::SudokuGenerator::GenerateMethod::Recursion));

    // 游戏初始未生成数独时以下按钮处于不可用状态
    enabledSudokuOperateBtns(false);
}

void GameOptionWidget::enabledSudokuOperateBtns(const bool &status) {
    ui->btnCheck->setEnabled(status);
    ui->btnSolve->setEnabled(status);
    ui->btnPromptAnswer->setEnabled(status);
    ui->btnSaveSudoku->setEnabled(status);
}

void GameOptionWidget::onSudokuIsSolved() {
    ui->btnCheck->setEnabled(false);
    ui->btnSolve->setEnabled(false);
    ui->btnPromptAnswer->setEnabled(false);
}
