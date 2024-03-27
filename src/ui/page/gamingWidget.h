/**
 ******************************************************************************
 * @file           : gamingWidget.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/4
 ******************************************************************************
 */

#ifndef SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SINGLEPLAYER_GAMINGWIDGET_H_
#define SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SINGLEPLAYER_GAMINGWIDGET_H_

#include <QTimer>
#include "sudokuGridWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class GamingWidget;
}
QT_END_NAMESPACE

class GamingWidget : public QWidget {
    Q_OBJECT

public:
    explicit GamingWidget(QWidget *parent = nullptr);
    ~GamingWidget() override;

    void generateSudokuMatrix(const Sudoku::SudokuMatrix::SudokuMatrixType &sudokuMatrixType);
    void startTimeCount();
    void saveGame() const;

signals:
    void sigKeyEscRelease();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    Ui::GamingWidget *ui;
    SudokuGridWidget *m_sudokuGridWidget;
    QTimer *m_timerForGameSpent;
    int m_timeElapsed = 0;

    void init();
    void signalsProcess();
    void sudokuGridWidgetSignalsProcess();
};

#endif // SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SINGLEPLAYER_GAMINGWIDGET_H_
