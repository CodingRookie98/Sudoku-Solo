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
#include <QDir>
#include <random>
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
    m_musicFiles = new std::vector<QString>;

    init();
    signalsProcess();
}

BGMPlayer::~BGMPlayer() {
    delete m_mediaPlayer;
    delete m_audioOutput;
    delete m_musicFiles;
}

void BGMPlayer::init() {
    m_mediaPlayer->setAudioOutput(m_audioOutput);

    QStringList fileList = QDir(m_musicPath).entryList(QStringList() << "*.wav", QDir::Filter::Files);
    foreach (const QString &filePath, fileList) {
        QString absolutePath = std::string(std::filesystem::absolute(m_musicPath.toStdString() + std::string("/") + filePath.toStdString()).string()).c_str();
        m_musicFiles->emplace_back(absolutePath);
    }

    if (!m_musicFiles->empty()) {
        int index = getRandomNum(0, (int)m_musicFiles->size());
        if (QFile(m_musicFiles->at(index)).exists()) {
            m_mediaPlayer->setSource(QUrl::fromLocalFile(m_musicFiles->at(index)));
            m_mediaPlayer->play();
        }
    }

    QJsonValue value = GameSettings::getInstance()->getSetting(GameSettings::getInstance()->m_soundEffectVolume);
    float volume = 100;

    if (!value.isNull()) {
        volume = (float)value.toInt() / 100;
    }
    m_audioOutput->setVolume(volume);
}

void BGMPlayer::signalsProcess() {
    connect(m_mediaPlayer, &QMediaPlayer::playbackStateChanged, this, [&](const QMediaPlayer::PlaybackState &playbackState) {
        qDebug() << "##############";
        if (playbackState == QMediaPlayer::PlaybackState::StoppedState) {
            qDebug() << "@@@@@@@@@@@@@";
            int index = getRandomNum(0, (int)m_musicFiles->size());
            if (QFile(m_musicFiles->at(0)).exists()) {
                m_mediaPlayer->setSource(QUrl::fromLocalFile(m_musicFiles->at(index)));
                m_mediaPlayer->play();
            }
        }
    });
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
    m_audioOutput->setVolume(vol);
    if (vol == 0) {
        m_mediaPlayer->pause();
    } else if (!m_mediaPlayer->isPlaying()) {
        m_mediaPlayer->play();
    }
}

int BGMPlayer::getRandomNum(const int &min, const int &max) {
    // 设置随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());

    // 生成随机数
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}
