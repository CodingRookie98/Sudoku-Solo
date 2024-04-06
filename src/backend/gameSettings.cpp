/**
 ******************************************************************************
 * @file           : gameSettings.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/22
 ******************************************************************************
 */

#include <mutex>
#include <QFile>
#include <QJsonDocument>
#include "gameSettings.h"
#include "gameManager.h"
#include "logger.h"

GameSettings::GameSettings(QObject *parent) :
    QObject(parent) {
    m_jsonObjectSettings = nullptr;
    init();
}

GameSettings::~GameSettings() {
    delete m_jsonObjectSettings;
}

GameSettings *GameSettings::getInstance() {
    static GameSettings *instance;
    static std::mutex mutex; // 静态互斥锁，你也可以用其他锁
    // 第一次检查，不加锁判断，主要检查实例是否已经创建（无性能消耗）
    if (instance == nullptr) { //[这个判断，主要是为了避免以后真正创建实例后，每次判断都会导致的先加锁问题，这种方式java单列也是这样，老方法了]
        // 如果实例没有创建，此处开始加锁，保证只有一个线程，可以往下进行创建实例。
        std::lock_guard<std::mutex> lock(mutex);
        // 第二次检查，这是加锁了的
        if (instance == nullptr) {
            // 如果实例真的没有创建，则开始创建
            instance = new GameSettings;
            // 注册析构函数，确保在程序退出时释放单例对象的内存
            std::atexit([]() {
                delete instance;
            });
        }
    }
    return instance;
}

void GameSettings::init() {
}

void GameSettings::preCheck() {
    auto *settingsFile = new QFile(m_settingsJsonFileName);
    if (!settingsFile->open(QIODevice::ReadOnly)) {
        // log: 设置文件创建或打开失败
        Logger::getInstance()->log(Logger::Error, QString(__FUNCTION__) + " "
                                                      + QString::number(__LINE__) + " "
                                                      + settingsFile->errorString());
        emit sigLastGameIsEmpty();
        m_jsonObjectSettings = new QJsonObject;
        return;
    }

    auto *jsonParseError = new QJsonParseError;
    auto *jsonDocument = new QJsonDocument(QJsonDocument::fromJson(settingsFile->readAll(), jsonParseError));

    if (jsonParseError->error != QJsonParseError::NoError) {
        emit sigLastGameIsEmpty();
        // log: 设置文件解析失败
        Logger::getInstance()->log(Logger::Error, QString(__FUNCTION__) + " "
                                                      + QString::number(__LINE__) + " "
                                                      + jsonParseError->errorString());
        m_jsonObjectSettings = new QJsonObject;
        return;
    }

    if (jsonDocument->isEmpty()) {
        emit sigLastGameIsEmpty();
        // log: 设置文件为空
        Logger::getInstance()->log(Logger::Error, QString(__FUNCTION__) + " "
                                                      + QString::number(__LINE__) + " "
                                                      + "jsonDocument is empty");
        m_jsonObjectSettings = new QJsonObject;
        return;
    } else {
        m_jsonObjectSettings = new QJsonObject(jsonDocument->object());
    }

    if (!m_jsonObjectSettings->contains(m_lastGameSaveName)) {
        emit sigLastGameIsEmpty();
    } else {
        if (!m_jsonObjectSettings->value(m_lastGameSaveName).isNull()
            && !m_jsonObjectSettings->value(m_lastGameSaveName).toString().isEmpty()) {
            // 如果上一次保存游戏的存档还在,则设为游戏保存存档
            if (QFile(m_jsonObjectSettings->value(m_lastGameSaveName).toString()).exists()) {
                GameManager::getInstance()->setSaveFilePath(m_jsonObjectSettings->value(m_lastGameSaveName).toString());
            } else {
                // 如果上一次保存游戏的存档不存在
                setSetting(m_lastGameSaveName, QJsonValue());
                setSetting(m_lastGameId, QJsonValue());
                emit sigLastGameIsEmpty();
            }

            // 如果上一次游玩的游戏Id为空或者在存档里面加载不出来
            if (m_jsonObjectSettings->value(m_lastGameId).isNull()
                || m_jsonObjectSettings->value(m_lastGameId).toString().isEmpty()
                || GameManager::getInstance()->loadWithId(m_jsonObjectSettings->value(m_lastGameId).toString()).m_workMatrix == nullptr) {
                emit sigLastGameIsEmpty();
                setSetting(m_lastGameId, QJsonValue());
            }
        } else {
            emit sigLastGameIsEmpty();
            setSetting(m_lastGameId, QJsonValue());
        }
    }

    settingsFile->close();

    delete settingsFile;
    delete jsonParseError;
    delete jsonDocument;
}

void GameSettings::setSetting(const QString &key, const QJsonValue &value) {
    if (m_jsonObjectSettings == nullptr) {
        m_jsonObjectSettings = new QJsonObject;
    }

    m_jsonObjectSettings->insert(key, value);

    std::thread thread1(&GameSettings::writeToFile, this);
    thread1.detach();
}

void GameSettings::writeToFile() {
    if (m_jsonObjectSettings == nullptr) {
        // log: 写入设置到文件时m_jsonObjectSettings为空指针
        Logger::getInstance()->log(Logger::Error, QString(__FUNCTION__) + " "
                                                      + QString::number(__LINE__) + " "
                                                      + " m_jsonObjectSettings is nullptr");
        m_jsonObjectSettings = new QJsonObject;
        return;
    }

    QFile file(m_settingsJsonFileName);
    if (!file.open(QIODevice::WriteOnly)) {
        // 以只写模式打开或创建文件失败
        Logger::getInstance()->log(Logger::Error, QString(__FUNCTION__) + " "
                                                      + QString::number(__LINE__) + " "
                                                      + file.errorString());
        return;
    } else {
        QTextStream stream(&file);
        stream.setEncoding(QStringConverter::Encoding::Utf8);

        std::shared_lock<std::shared_mutex> sharedLock(m_mutex);
        QJsonDocument jsonDocument(*m_jsonObjectSettings);
        stream << jsonDocument.toJson(QJsonDocument::JsonFormat::Indented);
    }
    file.close();
}

QJsonValue GameSettings::getSetting(const QString &key) {
    if (m_jsonObjectSettings->find(key) != m_jsonObjectSettings->end()) {
        return (*m_jsonObjectSettings)[key];
    }
    return {};
}
