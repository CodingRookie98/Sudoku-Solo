/**
 ******************************************************************************
 * @file           : btnsudokuunit.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/3
 ******************************************************************************
 */

#ifndef SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SUDOKU_BTNSUDOKUUNIT_H_
#define SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SUDOKU_BTNSUDOKUUNIT_H_

#include <QPushButton>
#include "numberChoiceDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class BtnSudokuUnit;
}
QT_END_NAMESPACE

class BtnSudokuUnit : public QPushButton {
    Q_OBJECT

public:
    explicit BtnSudokuUnit(QWidget* parent = nullptr, const int& row = -1, const int& col = -1, const QString& qstr = "");
    ~BtnSudokuUnit() override;
    
    std::pair<int, int> getPosForMatrix() const;
    
    bool isInvalided = false;
protected:
    
    void resizeEvent(QResizeEvent* event) override;
private:
    Ui::BtnSudokuUnit* ui;
    std::pair<int, int> pos;
    
    void init();
    void signalsProcess();
};

#endif // SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SUDOKU_BTNSUDOKUUNIT_H_
