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
#include <QStackedLayout>
#include <atomic>
#include "btnSudokuUnit.h"
#include "numberChoiceDialog.h"
#include "sudokuMatrix.h"
#include "sudokuMatrix.h"
#include "sudokuGenerator.h"
#include "solverBase.h"
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
    
    enum Message {
        IsFilledButNotFinished,
        IsFilledAndFinished,
        PleaseSelectButtonToHint,
    };

signals:
    void sigShowWidget();
    void sigCurrentSudokuIsSolved();
    void sigMessageReadyToRead(const Message &gameState);

public slots:
    void generateRandomSudoku(const Sudoku::SudokuMatrix::SudokuMatrixType &sudokuMatrixType);
    void checkCurrentSudoku();
    void solveCurrentSudoku();
    void saveCurrentSudoku();
    void readSudokuFromFile();
    void hintAnswer();
    void inputSudoku();

protected:
    bool event(QEvent *event) override;

private:
    Ui::SudokuGridWidget *ui;

    QGridLayout *m_gridLayoutBoard;
    std::vector<QGridLayout *> *m_boxesGridLayout;
    std::vector<std::vector<BtnSudokuUnit *>> *m_buttons;
    QButtonGroup *m_btnGroup;
    NumberChoiceDialog *m_choiceDialog;
    Sudoku::SudokuGenerator *m_generator;                      // 用作生成完全数独
    std::shared_ptr<Sudoku::SudokuMatrix> m_sudokuMatrixAnswer; // 数独答案
    std::shared_ptr<Sudoku::SudokuMatrix> m_sudokuMatrixWorker; // 用作数独中间态
    Sudoku::SolverBase *m_solverBase;
    InputSudokuDialog *m_inputSudokuDialog;

    void init();
    void initBoard();
    void signalsProcess();
    void initButtons();
    void updateButtons();
    void deleteButtons();
    void initBoxes();
    void deleteBoxes();
    void initChoiceDialog();
    [[nodiscard]] int getRandomInitialNumber() const;

private slots:
    void handleButtonClicked(QAbstractButton *btn);
};

#endif // SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SUDOKU_SUDOKUGRIDWIDGET_H_
