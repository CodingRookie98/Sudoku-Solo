/**
  ******************************************************************************
  * @file           : savesBrowserWidget.h
  * @author         : hui
  * @brief          : None
  * @attention      : None
  * @date           : 2024/3/21
  ******************************************************************************
  */

#ifndef SUDOKU_SOLO_SRC_UI_PAGE_SAVESBROWSERWIDGET_H_
#define SUDOKU_SOLO_SRC_UI_PAGE_SAVESBROWSERWIDGET_H_

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class SavesBrowserWidget; }
QT_END_NAMESPACE

class SavesBrowserWidget : public QWidget {
Q_OBJECT

public:
    explicit SavesBrowserWidget(QWidget *parent = nullptr);
    ~SavesBrowserWidget() override;

private:
    Ui::SavesBrowserWidget *ui;
};


#endif //SUDOKU_SOLO_SRC_UI_PAGE_SAVESBROWSERWIDGET_H_
