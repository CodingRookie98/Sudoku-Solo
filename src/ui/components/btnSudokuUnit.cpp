/**
 ******************************************************************************
 * @file           : btnsudokuunit.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/3
 ******************************************************************************
 */

#include "btnSudokuUnit.h"
#include "ui_BtnSudokuUnit.h"

BtnSudokuUnit::BtnSudokuUnit(QWidget *parent, const int &row, const int &col, const QString &qstr) :
    QPushButton(qstr, parent), ui(new Ui::BtnSudokuUnit), pos(row, col){
    ui->setupUi(this);
    init();
    signalsProcess();
}

BtnSudokuUnit::~BtnSudokuUnit() {
    delete ui;
}

void BtnSudokuUnit::init() {
    this->setMinimumSize(60, 60);
}

void BtnSudokuUnit::signalsProcess() {

}

void BtnSudokuUnit::resizeEvent(QResizeEvent *event) {
        this->setMaximumWidth(this->height());
        event->accept();
}

std::pair<int, int> BtnSudokuUnit::getPosForMatrix() const {
    return pos;
}
