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
    gameOptionWidget_ = new GameOptionWidget(this);
    sudokuGridWidget_ = new SudokuGridWidget(this);
    ui->gridLayoutSudokuBoard->addWidget(sudokuGridWidget_);
    ui->gridLayoutGameOption->addWidget(gameOptionWidget_);
    
    init();
    signalsProcess();
}

GamingWidget::~GamingWidget() {
    delete ui;
    delete sudokuGridWidget_;
    delete gameOptionWidget_;
}

void GamingWidget::init() {
    ui->frameSudokuBoard->hide();
}

void GamingWidget::signalsProcess() {
    connect(gameOptionWidget_, &GameOptionWidget::sigGenerateRandomSudoku, sudokuGridWidget_, &SudokuGridWidget::generateRandomSudoku);
    // 当生成数独时，显示数独界面
    connect(gameOptionWidget_, &GameOptionWidget::sigGenerateRandomSudoku, ui->frameSudokuBoard, &QFrame::show);
    connect(gameOptionWidget_, &GameOptionWidget::sigCheckCurrentSudoku, sudokuGridWidget_, &SudokuGridWidget::checkCurrentSudoku);
    connect(gameOptionWidget_, &GameOptionWidget::sigSolveCurrentSudoku, sudokuGridWidget_, &SudokuGridWidget::solveCurrentSudoku);
    connect(gameOptionWidget_, &GameOptionWidget::sigSaveCurrentSudoku, sudokuGridWidget_, &SudokuGridWidget::saveCurrentSudoku);
    connect(gameOptionWidget_, &GameOptionWidget::sigReadSudokuFromFile, sudokuGridWidget_, &SudokuGridWidget::readSudokuFromFile);
    connect(sudokuGridWidget_, &SudokuGridWidget::sigShowWidget, [&]{
       ui->frameSudokuBoard->show();
       gameOptionWidget_->enabledSudokuOperateBtns(true);
    });
    connect(sudokuGridWidget_, &SudokuGridWidget::sigCurrentSudokuIsSolved, gameOptionWidget_, &GameOptionWidget::onSudokuIsSolved);
    connect(gameOptionWidget_, &GameOptionWidget::sigPromtAnswer, sudokuGridWidget_, &SudokuGridWidget::promptAnswer);
    connect(gameOptionWidget_, &GameOptionWidget::sigInputSudoku, sudokuGridWidget_, &SudokuGridWidget::inputSudoku);
}
