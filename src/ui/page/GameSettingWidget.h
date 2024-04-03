#ifndef GAMESETTING_H
#define GAMESETTING_H

#include <QWidget>
#include "gameSoundSetting.h"

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

private:
    Ui::GameSettingWidget *ui;
    GameSoundSetting *m_gameSoundSetting;

    void init();
    void signalsProcess();
};

#endif // GAMESETTING_H
