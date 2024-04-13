/**
 ******************************************************************************
 * @file           : gameLanguage.h
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/4/11
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_UI_COMPONENTS_GAMELANGUAGE_H_
#define SUDOKU_SOLO_SRC_UI_COMPONENTS_GAMELANGUAGE_H_

#include <QWidget>
#include <QTranslator>
#include "settingBase.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class GameLanguage;
}
QT_END_NAMESPACE

class GameLanguage : public QWidget, public SettingBase {
    Q_OBJECT

public:
    explicit GameLanguage(QWidget *parent = nullptr);
    ~GameLanguage() override;

    void saveSettings() override;

signals:
    void sigSettingsChanged();

private:
    Ui::GameLanguage *ui;
    const QString en_US = ":/language/en_US.qm";
    const QString zh_CN = ":/language/zh_CN.qm";

    QTranslator *m_translator;
    void init();
    void signalsProcess();
};

#endif // SUDOKU_SOLO_SRC_UI_COMPONENTS_GAMELANGUAGE_H_
