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
#include "sudokuGridWidget.h"
#include "gamingWidget.h"
#include "aboutDialog.h"

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
    
    QIcon icon;
    QTranslator *translator_;
    QGridLayout *grid_layout_;
    GamingWidget *gamingWidget_;
    
    void init();
    void signalsProcess();
};

#endif // SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SINGLEPLAYER_MAINWINDOW_H_
