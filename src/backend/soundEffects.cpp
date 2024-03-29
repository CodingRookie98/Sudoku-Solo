/**
 ******************************************************************************
 * @file           : soundEffects.cpp
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/29
 ******************************************************************************
 */

#include "soundEffects.h"
#include <mutex>

SoundEffects::SoundEffects() {
    m_soundEffectMap = new std::unordered_map<QString, QSoundEffect *>;

    (*m_soundEffectMap)[m_click_001] = nullptr;
    (*m_soundEffectMap)[m_hover_34] = nullptr;

    for (auto &item : *m_soundEffectMap) {
        auto *soundEffect = new QSoundEffect;
        soundEffect->setSource(QUrl::fromLocalFile(item.first));
        item.second = soundEffect;
    }
}

SoundEffects::~SoundEffects() {
    for (auto &item : *m_soundEffectMap) {
        delete item.second;
        item.second = nullptr;
    }
    delete m_soundEffectMap;
}

SoundEffects *SoundEffects::getInstance() {
    static SoundEffects *instance;
    static std::mutex mutex; // 静态互斥锁，你也可以用其他锁
    // 第一次检查，不加锁判断，主要检查实例是否已经创建（无性能消耗）
    if (instance == nullptr) { //[这个判断，主要是为了避免以后真正创建实例后，每次判断都会导致的先加锁问题，这种方式java单列也是这样，老方法了]
        // 如果实例没有创建，此处开始加锁，保证只有一个线程，可以往下进行创建实例。
        std::lock_guard<std::mutex> lock(mutex);
        // 第二次检查，这是加锁了的
        if (instance == nullptr) {
            // 如果实例真的没有创建，则开始创建
            instance = new SoundEffects;
            // 注册析构函数，确保在程序退出时释放单例对象的内存
            std::atexit([]() {
                delete instance;
            });
        }
    }
    return instance;
}

void SoundEffects::play(const QString &soundEffectName) {
    if (m_soundEffectMap->contains(soundEffectName)) {
        m_soundEffectMap->at(soundEffectName)->play();
    }
}
