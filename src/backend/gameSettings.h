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
#include <QTranslator>

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
    const QString m_soundEffectVolume = "soundEffectVolume";
    const QString m_backgroundMusicVolume = "backgroundMusicVolume";
    const QString m_language = "language";
    const QString m_backgroundWebPath = "backgroundWebPath";

signals:
    void sigLastGameIsEmpty();

private:
    explicit GameSettings(QObject *parent = nullptr);
    ~GameSettings() override;

    std::shared_mutex m_mutex;
    const QString m_settingsJsonFileName = "./settings.json";
    QJsonObject *m_jsonObjectSettings;
    QTranslator *m_translator;

    void init();
    void writeToFile();
    void checkLastGameSave();
    void checkLanguage();
};

#endif // SUDOKU_SOLO_SRC_BACKEND_GAMESETTINGS_H_
