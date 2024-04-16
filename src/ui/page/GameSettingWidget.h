#ifndef GAMESETTING_H
#define GAMESETTING_H

#include <QWidget>
#include "gameSoundSetting.h"
#include "gameLanguage.h"
#include "settingBase.h"
#include "backgroundSetting.h"

namespace Ui {
class GameSettingWidget;
}

class GameSettingWidget : public QWidget {
    Q_OBJECT

public:
    explicit GameSettingWidget(QWidget *parent = nullptr);
    ~GameSettingWidget();

signals:
    void sigFnished();

protected:
    bool event(QEvent *event) override;

private:
    Ui::GameSettingWidget *ui;
    GameSoundSetting *m_gameSoundSetting;
    GameLanguage *m_gameLanguage;
    BackgroundSetting *m_backgroundSetting;

    std::unordered_set<SettingBase *> *m_settingChangedClass;

    void init();
    void signalsProcess();
    void applySettings();
    void childSignalsProcess();
    void insertToChangedClass(SettingBase *settingBase);
};

#endif // GAMESETTING_H
