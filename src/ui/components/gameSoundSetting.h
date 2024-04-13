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
#include <unordered_set>
#include "settingBase.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class GameSoundSetting;
}
QT_END_NAMESPACE

class GameSoundSetting : public QWidget, public SettingBase {
    Q_OBJECT

public:
    explicit GameSoundSetting(QWidget *parent = nullptr);
    ~GameSoundSetting() override;

    void saveSettings() override;

signals:
    void sigSettingsChanged();

protected:
    bool event(QEvent *event) override;

private:
    Ui::GameSoundSetting *ui;

    void init();
    void signalsProcess();
};

#endif // SUDOKU_SOLO_SRC_UI_COMPONENTS_GAMESOUNDSETTING_H_
