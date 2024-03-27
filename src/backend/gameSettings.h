/**
 ******************************************************************************
 * @file           : gameSettings.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/22
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_BACKEND_GAMESETTINGS_H_
#define SUDOKU_SOLO_SRC_BACKEND_GAMESETTINGS_H_

#include <QString>
#include <QObject>
#include <QJsonObject>
#include <shared_mutex>

class GameSettings : public QObject {
    Q_OBJECT
public:
    GameSettings(const GameSettings &) = delete;
    GameSettings &operator=(const GameSettings &) = delete;
    
    static GameSettings *getInstance();
    void preCheck();
    void setSetting(const QString &key, const QJsonValue &value);
    QJsonValue getSetting(const QString &key);
    
    const QString m_lastGameId = "LastGameID";
    const QString m_lastGameSaveName = "LastGameSaveName";
    
signals:
    void sigLastGameIsEmpty();
    
private:
    explicit GameSettings(QObject *parent = nullptr);
    ~GameSettings() override;
    
    std::shared_mutex m_mutex;
    const QString m_settingsJsonFileName = "./settings.json";
    QJsonObject *m_jsonObjectSettings;
    
    void init();
    void writeToFile();
};

#endif // SUDOKU_SOLO_SRC_BACKEND_GAMESETTINGS_H_
