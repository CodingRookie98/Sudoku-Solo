/**
 ******************************************************************************
 * @file           : aboutDialog.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/29
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_UI_COMPONENTS_ABOUTDIALOG_H_
#define SUDOKU_SOLO_SRC_UI_COMPONENTS_ABOUTDIALOG_H_

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class AboutDialog;
}
QT_END_NAMESPACE

class AboutDialog : public QDialog {
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog() override;

private:
    Ui::AboutDialog *ui;
    
    void init();
    void signalsProcess();
};

#endif // SUDOKU_SOLO_SRC_UI_COMPONENTS_ABOUTDIALOG_H_
