/**
 ******************************************************************************
 * @file           : gamePrepareWidget.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/11
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_UI_COMPONENTS_GAMEPREPAREWIDGET_H_
#define SUDOKU_SOLO_SRC_UI_COMPONENTS_GAMEPREPAREWIDGET_H_

#include <QWidget>
#include <QButtonGroup>
#include <QPushButton>
#include "sudokuMatrix.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class GamePrepareWidget;
}
QT_END_NAMESPACE

class GamePrepareWidget : public QWidget {
    Q_OBJECT

public:
    explicit GamePrepareWidget(QWidget *parent = nullptr);
    ~GamePrepareWidget() override;

signals:
    void sigStart(const Sudoku::SudokuMatrix::SudokuMatrixType &sudokuMatrixTypes);
    void sigCancel();
    void sigKeyEscRelease();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;
    bool event(QEvent *event) override;

private:
    Ui::GamePrepareWidget *ui;
    QButtonGroup *m_buttonGroup;
    std::map<QPushButton *, Sudoku::SudokuMatrix::SudokuMatrixType> *m_btnWithSudokuType;

    void init();
    void signalProcess();
};

#endif // SUDOKU_SOLO_SRC_UI_COMPONENTS_GAMEPREPAREWIDGET_H_
