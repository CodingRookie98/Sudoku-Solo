/**
 ******************************************************************************
 * @file           : gameMessageBox.h
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/4/5
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_UI_COMPONENTS_GAMEMESSAGEBOX_H_
#define SUDOKU_SOLO_SRC_UI_COMPONENTS_GAMEMESSAGEBOX_H_

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class GameMessageBox;
}
QT_END_NAMESPACE

class GameMessageBox : public QDialog {
    Q_OBJECT

public:
    explicit GameMessageBox(QWidget *parent = nullptr);
    ~GameMessageBox() override;

    void setMessage(const QString &message);

protected:
    bool event(QEvent *event) override;

private:
    Ui::GameMessageBox *ui;

    void init();
    void signalsProcess();
};

#endif // SUDOKU_SOLO_SRC_UI_COMPONENTS_GAMEMESSAGEBOX_H_
