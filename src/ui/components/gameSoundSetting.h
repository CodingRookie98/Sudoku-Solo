/**
 ******************************************************************************
 * @file           : gameSoundSetting.h
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/4/3
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_UI_COMPONENTS_GAMESOUNDSETTING_H_
#define SUDOKU_SOLO_SRC_UI_COMPONENTS_GAMESOUNDSETTING_H_

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class GameSoundSetting;
}
QT_END_NAMESPACE

class GameSoundSetting : public QWidget {
    Q_OBJECT

public:
    explicit GameSoundSetting(QWidget *parent = nullptr);
    ~GameSoundSetting() override;

    void saveSettings() const;

private:
    Ui::GameSoundSetting *ui;

    void init();
    void signalsProcess();
};

#endif // SUDOKU_SOLO_SRC_UI_COMPONENTS_GAMESOUNDSETTING_H_
