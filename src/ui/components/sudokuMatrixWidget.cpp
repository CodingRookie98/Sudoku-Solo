/**
  ******************************************************************************
  * @file           : sudokuMatrixWidget.cpp
  * @author         : hui
  * @brief          : None
  * @attention      : None
  * @date           : 2024/3/23
  ******************************************************************************
  */

// You may need to build the project (run Qt uic code generator) to get "ui_SudokuMatrixWidget.h" resolved

#include "sudokuMatrixWidget.h"
#include "ui_SudokuMatrixWidget.h"


SudokuMatrixWidget::SudokuMatrixWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::SudokuMatrixWidget) {
    ui->setupUi(this);
}

SudokuMatrixWidget::~SudokuMatrixWidget() {
    delete ui;
}
