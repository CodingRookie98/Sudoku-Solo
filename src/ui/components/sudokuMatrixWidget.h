/**
  ******************************************************************************
  * @file           : sudokuMatrixWidget.h
  * @author         : hui
  * @brief          : None
  * @attention      : None
  * @date           : 2024/3/23
  ******************************************************************************
  */

#ifndef SUDOKU_SOLO_SRC_UI_COMPONENTS_SUDOKUMATRIXWIDGET_H_
#define SUDOKU_SOLO_SRC_UI_COMPONENTS_SUDOKUMATRIXWIDGET_H_

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class SudokuMatrixWidget; }
QT_END_NAMESPACE

class SudokuMatrixWidget : public QWidget {
Q_OBJECT

public:
    explicit SudokuMatrixWidget(QWidget *parent = nullptr);
    ~SudokuMatrixWidget() override;

private:
    Ui::SudokuMatrixWidget *ui;
};


#endif //SUDOKU_SOLO_SRC_UI_COMPONENTS_SUDOKUMATRIXWIDGET_H_
