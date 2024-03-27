/**
 ******************************************************************************
 * @file           : homeWidget.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/11
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_UI_PAGE_HOMEWIDGET_H_
#define SUDOKU_SOLO_SRC_UI_PAGE_HOMEWIDGET_H_

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class HomeWidget;
}
QT_END_NAMESPACE

class HomeWidget : public QWidget {
    Q_OBJECT

public:
    explicit HomeWidget(QWidget *parent = nullptr);
    ~HomeWidget() override;

signals:
    void sigNewGame();
    void sigGameSetting();
    void sigGameSaves();
    void sigLoadLastGame();
    
private:
    Ui::HomeWidget *ui;

    void init();
    void signalProcess();
};

#endif // SUDOKU_SOLO_SRC_UI_PAGE_HOMEWIDGET_H_
