/**
 ******************************************************************************
 * @file           : gamePauseWidget.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/12
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_UI_COMPONENTS_GAMEPAUSEWIDGET_H_
#define SUDOKU_SOLO_SRC_UI_COMPONENTS_GAMEPAUSEWIDGET_H_

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class GamePauseWidget;
}
QT_END_NAMESPACE

class GamePauseWidget : public QWidget {
    Q_OBJECT

public:
    explicit GamePauseWidget(QWidget *parent = nullptr);
    ~GamePauseWidget() override;

signals:
    void sigKeyEscRelease();
    void sigBackToGame();
    void sigSaveGame();
    void sigLoadGameSave();
    void sigGameSetting();
    void sigHome();
    void sigExitGame();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;
    bool event(QEvent *event) override;

private:
    Ui::GamePauseWidget *ui;

    void init();
    void signalsProcess();
};

#endif // SUDOKU_SOLO_SRC_UI_COMPONENTS_GAMEPAUSEWIDGET_H_
