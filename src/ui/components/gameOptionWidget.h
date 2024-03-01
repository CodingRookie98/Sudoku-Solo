/**
 ******************************************************************************
 * @file           : gameOptionWidget.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/4
 ******************************************************************************
 */

#ifndef SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SUDOKU_GAMEOPTIONWIDGET_H_
#define SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SUDOKU_GAMEOPTIONWIDGET_H_

#include <QWidget>
#include "sudokuGenerator.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class GameOptionWidget;
}
QT_END_NAMESPACE

class GameOptionWidget : public QWidget {
    Q_OBJECT

public:
    explicit GameOptionWidget(QWidget *parent = nullptr);
    ~GameOptionWidget() override;

signals:
    void sigGenerateRandomSudoku(const Sudoku::SudokuMatrix::SudokuMatrixTypes &sudokuType, const Sudoku::SudokuGenerator::GenerateMethod &generateMethod, const int &num);
    void sigCheckCurrentSudoku();
    void sigSolveCurrentSudoku();
    void sigSaveCurrentSudoku();
    void sigReadSudokuFromFile();
    void sigPromtAnswer();
    void sigInputSudoku();
    
public slots:
    void enabledSudokuOperateBtns(const bool &status);
    void onSudokuIsSolved();

protected:
    bool event(QEvent *event) override;

private:
    Ui::GameOptionWidget *ui;

    void init();
    void signalsProcess();
};

#endif // SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SUDOKU_GAMEOPTIONWIDGET_H_
