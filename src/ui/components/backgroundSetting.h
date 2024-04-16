/**
 ******************************************************************************
 * @file           : backgroundSetting.h
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/4/16
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_UI_COMPONENTS_BACKGROUNDSETTING_H_
#define SUDOKU_SOLO_SRC_UI_COMPONENTS_BACKGROUNDSETTING_H_

#include <QWidget>
#include "settingBase.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class BackgroundSetting;
}
QT_END_NAMESPACE

class BackgroundSetting : public QWidget, public SettingBase {
    Q_OBJECT

public:
    explicit BackgroundSetting(QWidget *parent = nullptr);
    ~BackgroundSetting() override;

    void saveSettings() override;

signals:
    void sigSettingsChanged();

private:
    Ui::BackgroundSetting *ui;
    const QString m_path = "./background";

    void init();
    void signalsProcess();
};

#endif // SUDOKU_SOLO_SRC_UI_COMPONENTS_BACKGROUNDSETTING_H_
