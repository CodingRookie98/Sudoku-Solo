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

#include <QWidget>
#include "sudokuGridWidget.h"
#include "gameOptionWidget.h"

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

private:
    Ui::GamingWidget *ui;

    SudokuGridWidget *sudokuGridWidget_;
    GameOptionWidget *gameOptionWidget_;
    
    void init();
    void signalsProcess();
};

#endif // SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SINGLEPLAYER_GAMINGWIDGET_H_
