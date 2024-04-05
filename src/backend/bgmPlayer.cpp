/**
 ******************************************************************************
 * @file           : bgmPlayer.cpp
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/4/3
 ******************************************************************************
 */

#include <mutex>
#include "bgmPlayer.h"
#include "gameSettings.h"
#include "logger.h"

BGMPlayer *BGMPlayer::getInstance() {
    static BGMPlayer *instance;
    static std::mutex mutex; // 静态互斥锁，你也可以用其他锁
    // 第一次检查，不加锁判断，主要检查实例是否已经创建（无性能消耗）
    if (instance == nullptr) { //[这个判断，主要是为了避免以后真正创建实例后，每次判断都会导致的先加锁问题，这种方式java单列也是这样，老方法了]
        // 如果实例没有创建，此处开始加锁，保证只有一个线程，可以往下进行创建实例。
        std::lock_guard<std::mutex> lock(mutex);
        // 第二次检查，这是加锁了的
        if (instance == nullptr) {
            // 如果实例真的没有创建，则开始创建
            instance = new BGMPlayer();
            // 注册析构函数，确保在程序退出时释放单例对象的内存
            std::atexit([]() {
                delete instance;
            });
        }
    }
    return instance;
}

BGMPlayer::BGMPlayer(QObject *parent) :
    QObject(parent) {
    m_mediaPlayer = new QMediaPlayer;
    m_audioOutput = new QAudioOutput;

    init();
}

BGMPlayer::~BGMPlayer() {
    delete m_mediaPlayer;
    delete m_audioOutput;
}

void BGMPlayer::init() {
    m_mediaPlayer->setAudioOutput(m_audioOutput);
    m_mediaPlayer->setSource(QUrl::fromLocalFile(""));

    QJsonValue value = GameSettings::getInstance()->getSetting(GameSettings::getInstance()->m_soundEffectVolume);
    float volume = 100;

    if (!value.isNull()) {
        volume = (float)value.toInt() / 100;
    }
    m_audioOutput->setVolume(volume);
}

void BGMPlayer::play() {
    if (m_mediaPlayer->error() == QMediaPlayer::NoError) {
        m_mediaPlayer->play();
    } else {
        Logger::getInstance()->log(Logger::Error, QString(__FUNCTION__) + " "
                                                      + QString::number(__LINE__) + " "
                                                      + QString(m_mediaPlayer->errorString()));
    }
}

void BGMPlayer::setVolume(const float &volume) {
    float vol = volume;
    if (vol < 0) {
        vol = 0;
    } else if (vol > 1) {
        vol = 1;
    }
    m_audioOutput->setVolume(volume);
}
