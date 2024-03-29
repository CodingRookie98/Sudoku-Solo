/**
 ******************************************************************************
 * @file           : soundEffects.h
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/29
 ******************************************************************************
 */

#include <QSoundEffect>
#include <unordered_map>

#ifndef SUDOKU_SOLO_SRC_BACKEND_SOUNDEFFECTS_H
#define SUDOKU_SOLO_SRC_BACKEND_SOUNDEFFECTS_H

class SoundEffects {
public:
    static SoundEffects *getInstance();

    SoundEffects(const SoundEffects &) = delete;
    SoundEffects &operator=(const SoundEffects &) = delete;

    void play(const QString &soundEffectName);

    const QString m_hover_34 = ":/audio/audio/hover_34.wav";
    const QString m_click_001 = ":/audio/audio/click_001.wav";

private:
    SoundEffects();
    ~SoundEffects();

    std::unordered_map<QString, QSoundEffect *> *m_soundEffectMap;
};

#endif // SUDOKU_SOLO_SRC_BACKEND_SOUNDEFFECTS_H
