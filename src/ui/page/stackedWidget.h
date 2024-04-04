/**
 ******************************************************************************
 * @file           : stackedWidget.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/12
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_UI_STACKEDWIDGET_H_
#define SUDOKU_SOLO_SRC_UI_STACKEDWIDGET_H_

#include <QStackedWidget>
#include <stack>
#include "GameSettingWidget.h"
#include "homeWidget.h"
#include "gamePrepareWidget.h"
#include "gamePauseWidget.h"
#include "gamingWidget.h"
#include "savesBrowserWidget.h"
#include "aboutWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class StackedWidget;
}
QT_END_NAMESPACE

class StackedWidget : public QStackedWidget {
    Q_OBJECT

public:
    explicit StackedWidget(QWidget *parent = nullptr);
    ~StackedWidget() override;

    void switchWidget(QWidget *widget);

private:
    Ui::StackedWidget *ui;

    std::map<QWidget *, int> *m_mapWidgets;
    std::stack<QWidget *> *m_stackWidgets;

    // Page
    HomeWidget *m_homeWidget;
    GamePrepareWidget *m_gamePrepareWidget;
    GamePauseWidget *m_gamePauseWidget;
    GamingWidget *m_gamingWidget;
    GameSettingWidget *m_gameSettingWidget;
    SavesBrowserWidget *m_savesBrowserWidget;
    AboutWidget *m_aboutWidget;

    void init();
    void signalsProcess();
    void homeWidgetSignalsProcess();
    void gamePrepareWidgetSignalsProcess();
    void gamePauseWidgetSignalsProcess();
    void gamingWidgetSignalsProcess();
    void gameSettingWidgetSignalsProcess();
    void saveBrowserWidgetSignalsProcess();
    void aboutWidgetSignalsProcess();
};

#endif // SUDOKU_SOLO_SRC_UI_STACKEDWIDGET_H_
