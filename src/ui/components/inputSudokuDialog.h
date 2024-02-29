/**
 ******************************************************************************
 * @file           : inputSudokuDialog.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/28
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_UI_COMPONENTS_INPUTSUDOKUDIALOG_H_
#define SUDOKU_SOLO_SRC_UI_COMPONENTS_INPUTSUDOKUDIALOG_H_

#include <QDialog>
#include "sudokuMatrix.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class InputSudokuDialog;
}
QT_END_NAMESPACE

class InputSudokuDialog : public QDialog {
    Q_OBJECT

public:
    explicit InputSudokuDialog(QWidget *parent = nullptr);
    ~InputSudokuDialog() override;

signals:
    void sigInputSudokuMatrix(const Sudoku::SudokuMatrix &matrix);

private:
    Ui::InputSudokuDialog *ui;

    void init();
    void signalsProcess();
    void processInputText();
};

#endif // SUDOKU_SOLO_SRC_UI_COMPONENTS_INPUTSUDOKUDIALOG_H_
