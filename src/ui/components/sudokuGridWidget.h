/**
 ******************************************************************************
 * @file           : wdgetSudokuGrid.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/3
 ******************************************************************************
 */

#ifndef SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SUDOKU_SUDOKUGRIDWIDGET_H_
#define SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SUDOKU_SUDOKUGRIDWIDGET_H_

#include <QWidget>
#include <QGridLayout>
#include <QButtonGroup>
#include <QFrame>
#include <vector>
#include "btnSudokuUnit.h"
#include "numberChoiceDialog.h"
#include "sudokuMatrix.h"
#include "sudokuMatrix.h"
#include "sudokuGenerator.h"
#include "solverBase.h"
#include "solverDl.h"
#include <iostream>
#include <fstream>
#include "inputSudokuDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SudokuGridWidget;
}
QT_END_NAMESPACE

class SudokuGridWidget : public QWidget {
    Q_OBJECT

public:
    explicit SudokuGridWidget(QWidget *parent = nullptr);
    ~SudokuGridWidget() override;

signals:
    void sigShowWidget();
    void sigCurrentSudokuIsSolved();

public slots:
    void generateRandomSudoku(const Sudoku::SudokuMatrix::SudokuMatrixTypes &sodokuType, const Sudoku::SudokuGenerator::GenerateMethod &generateMethod, const int &num);
    void checkCurrentSudoku();
    void solveCurrentSudoku();
    void saveCurrentSudoku();
    void readSudokuFromFile();
    void promptAnswer();
    void inputSudoku();
    
protected:
    bool event(QEvent *event) override;

private:
    Ui::SudokuGridWidget *ui;

    QGridLayout *board;
    std::vector<QGridLayout *> *boxesGridLayout;
    std::vector<std::vector<BtnSudokuUnit *>> *btns;
    QButtonGroup *btnGroup;
    NumberChoiceDialog *choiceDialog;
    Sudoku::SudokuGenerator *generator_;                      // 用作生成完全数独
    std::shared_ptr<Sudoku::SudokuMatrix> sudokuMatrixAnswer; // 数独答案
    std::shared_ptr<Sudoku::SudokuMatrix> sudokuMatrixWorker; // 用作数独中间态
    Sudoku::SolverBase *solverBase_;
    InputSudokuDialog *inputSudokuDialog_;

    void init();
    void initBoard();
    void signalsProcess();
    void initBtns();
    void updateBtns();
    void deleteBtns();
    void initBoxes();
    void deleteBoxes();
    void initChoiceDialog();

private slots:
    void handleButtonClicked(QAbstractButton *btn);
};

#endif // SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SUDOKU_SUDOKUGRIDWIDGET_H_