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
#include <QGridLayout>
#include <QButtonGroup>
#include <QPushButton>
#include "sudokuMatrix.h"
#include "btnSudokuUnit.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SudokuMatrixWidget;
}
QT_END_NAMESPACE

class SudokuMatrixWidget : public QWidget {
    Q_OBJECT

public:
    explicit SudokuMatrixWidget(QWidget *parent = nullptr);
    ~SudokuMatrixWidget() override;

    void buildMatrix(const Sudoku::SudokuMatrix &matrix);                               // 将数独矩阵显示在widget
    bool setMatrixValue(const int &rowIndex, const int &columnIndex, const int &value); // 设置矩阵值
    bool setButtonEnable(const int &rowIndex, const int &columnIndex, const bool &enable);
    Sudoku::SudokuMatrix getSudokuMatrix() const;
    bool isSolved() const; // 返回数独是否被解决
    BtnSudokuUnit *getCheckedButton() const;

signals:
    void sigUnitClicked(BtnSudokuUnit *btnSudokuUnit);
    void sigIsFilled(); // 数独被填满

private:
    Ui::SudokuMatrixWidget *ui;
    std::unique_ptr<Sudoku::SudokuMatrix> m_sudokuMatrix;
    QGridLayout *m_gridLayoutBoard;
    std::vector<std::vector<QGridLayout *>> *m_gridLayoutBoxes;
    QButtonGroup *m_buttonGroup;
    std::vector<std::vector<BtnSudokuUnit *>> *m_buttons;

    void init();
    void signalsProcess();
    void updateBoard();
    void updateBoxes();
    void deleteBoxes();
    void updateButtons();
    void deleteButtons();
    void updateButtonGroup();
};

#endif // SUDOKU_SOLO_SRC_UI_COMPONENTS_SUDOKUMATRIXWIDGET_H_
