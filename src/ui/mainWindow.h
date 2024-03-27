/**
 ******************************************************************************
 * @file           : mainwindow.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/3
 ******************************************************************************
 */

#ifndef SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SINGLEPLAYER_MAINWINDOW_H_
#define SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SINGLEPLAYER_MAINWINDOW_H_

#include <QMainWindow>
#include <QTranslator>
#include <QStackedLayout>
#include <QStackedWidget>
#include "stackedWidget.h"
#include "backgroundWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    bool eventFilter(QObject *object, QEvent *event) override;
    bool event(QEvent *event) override;

private:
    Ui::MainWindow *ui;

    QIcon m_icon;
    QTranslator *m_translator;
    QStackedLayout *m_stackedLayout;
    QStackedWidget *m_stackedWidget;
    BackgroundWidget *m_backgroundWidget;

    void init();
    void signalsProcess();
};

#endif // SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SINGLEPLAYER_MAINWINDOW_H_
