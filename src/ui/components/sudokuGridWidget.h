/**
 ******************************************************************************
 * @file           : wdgetSudokuGrid.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/3
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_UI_PAGE_SUDOKUGRIDWIDGET_H_
#define SUDOKU_SOLO_SRC_UI_PAGE_SUDOKUGRIDWIDGET_H_

#include "btnSudokuUnit.h"
#include "numberChoiceDialog.h"
#include "sudokuMatrix.h"
#include "sudokuGenerator.h"
#include "sudokuMatrixWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SudokuGridWidget;
}
QT_END_NAMESPACE

class SudokuGridWidget : public SudokuMatrixWidget {
    Q_OBJECT

public:
    explicit SudokuGridWidget(QWidget *parent = nullptr);
    ~SudokuGridWidget() override;

    enum Message {
        IsFilledButNotFinished,
        IsFilledAndFinished,
        PleaseSelectButtonToHint,
    };

    void setAllUnitDisable();

signals:
    void sigMessageReadyToRead(const SudokuGridWidget::Message &gameState);

public slots:
    void generateRandomSudoku(const Sudoku::SudokuMatrix::SudokuMatrixType &sudokuMatrixType);
    void saveCurrentSudoku(const int &timeSpent);
    void hintAnswer();
    void setGameMatrix(std::shared_ptr<Sudoku::SudokuMatrix> answer,
                       std::shared_ptr<Sudoku::SudokuMatrix> original,
                       std::shared_ptr<Sudoku::SudokuMatrix> work);

protected:
    bool event(QEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    Ui::SudokuGridWidget *ui;

    NumberChoiceDialog *m_choiceDialog;
    Sudoku::SudokuGenerator *m_generator;                         // 用作生成完全数独
    std::shared_ptr<Sudoku::SudokuMatrix> m_sudokuMatrixAnswer;   // 数独答案
    std::shared_ptr<Sudoku::SudokuMatrix> m_sudokuMatrixOriginal; // 用作数独初始状态
    BtnSudokuUnit *m_lastClickedBtnSudokuUnit;

    void init();
    void signalsProcess();
    void initChoiceDialog();
    [[nodiscard]] int getRandomInitialNumber() const;
    void choiceDialogSignalsProcess();

private slots:
    void handleButtonClicked(BtnSudokuUnit *btn);
};

#endif // SUDOKU_SOLO_SRC_UI_PAGE_SUDOKUGRIDWIDGET_H_
