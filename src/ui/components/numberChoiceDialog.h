/**
 ******************************************************************************
 * @file           : numberChoiceDialog.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/3
 ******************************************************************************
 */

#ifndef SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SUDOKU_NUMBERCHOICEDIALOG_H_
#define SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SUDOKU_NUMBERCHOICEDIALOG_H_

#include <QDialog>
#include <QPushButton>
#include <QButtonGroup>
#include <QMouseEvent>
#include <QHBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class NumberChoiceDialog;
}
QT_END_NAMESPACE

class NumberChoiceDialog : public QDialog {
    Q_OBJECT

public:
    explicit NumberChoiceDialog(QWidget *parent = nullptr);
    ~NumberChoiceDialog() override;
    
    void setChoices(const int& rowCount, const int&columnCount, const std::vector<int>& nums);
    
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Ui::NumberChoiceDialog *ui;
    QHBoxLayout *hLayout;
    std::vector<std::vector<QPushButton *>> *btnChoices;
    QButtonGroup *btnGroupNumChoices;
    QPoint offset;
    QFont font;

    void init();
    void signalsProcess();
    void delete_btnChoices();
signals:
    void choiceMade(const int& num);
};

#endif // SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SUDOKU_NUMBERCHOICEDIALOG_H_
