/**
 ******************************************************************************
 * @file           : aboutWidget.h
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/4/4
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_UI_PAGE_ABOUTWIDGET_H_
#define SUDOKU_SOLO_SRC_UI_PAGE_ABOUTWIDGET_H_

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class AboutWidget;
}
QT_END_NAMESPACE

class AboutWidget : public QWidget {
    Q_OBJECT

public:
    explicit AboutWidget(QWidget *parent = nullptr);
    ~AboutWidget() override;

signals:
    void sigClose();

protected:
    bool event(QEvent *event) override;

private:
    Ui::AboutWidget *ui;

    void init();
    void signalsProcess();
};

#endif // SUDOKU_SOLO_SRC_UI_PAGE_ABOUTWIDGET_H_
