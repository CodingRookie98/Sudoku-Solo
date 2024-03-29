/**
 ******************************************************************************
 * @file           : gameButton.h
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/28
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_UI_COMPONENTS_GAMEBUTTON_H_
#define SUDOKU_SOLO_SRC_UI_COMPONENTS_GAMEBUTTON_H_

#include <QPushButton>
#include <QSoundEffect>

QT_BEGIN_NAMESPACE
namespace Ui {
class GameButton;
}
QT_END_NAMESPACE

class GameButton : public QPushButton {
    Q_OBJECT

public:
    explicit GameButton(QWidget *parent = nullptr);
    ~GameButton() override;

protected:
    void enterEvent(QEnterEvent *event) override;

private:
    Ui::GameButton *ui;

    void init();
    void signalsProcess();
};

#endif // SUDOKU_SOLO_SRC_UI_COMPONENTS_GAMEBUTTON_H_
