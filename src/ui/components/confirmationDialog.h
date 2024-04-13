/**
 ******************************************************************************
 * @file           : confirmationDialog.h
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/4/6
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_UI_COMPONENTS_CONFIRMATIONDIALOG_H_
#define SUDOKU_SOLO_SRC_UI_COMPONENTS_CONFIRMATIONDIALOG_H_

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class ConfirmationDialog;
}
QT_END_NAMESPACE

class ConfirmationDialog : public QDialog {
    Q_OBJECT

public:
    explicit ConfirmationDialog(QWidget *parent = nullptr);
    ~ConfirmationDialog() override;

    void setText(const QString &text);
signals:
    void sigYes();
    void sigNo();

protected:
    bool event(QEvent *event) override;

private:
    Ui::ConfirmationDialog *ui;

    void init();
    void signalsProcess();
};

#endif // SUDOKU_SOLO_SRC_UI_COMPONENTS_CONFIRMATIONDIALOG_H_
