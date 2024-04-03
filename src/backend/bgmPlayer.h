/**
 ******************************************************************************
 * @file           : bgmPlayer.h
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/4/3
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_BACKEND_BGMPLAYER_H_
#define SUDOKU_SOLO_SRC_BACKEND_BGMPLAYER_H_

#include <QMediaPlayer>
#include <QAudioOutput>

class BGMPlayer : public QObject {
    Q_OBJECT
public:
    BGMPlayer(const BGMPlayer &) = delete;
    BGMPlayer &operator=(const BGMPlayer &) = delete;

    static BGMPlayer *getInstance();
    void play();
    void setVolume(const float &volume);

private:
    explicit BGMPlayer(QObject *parent = nullptr);
    ~BGMPlayer() override;

    QMediaPlayer *m_mediaPlayer;
    QAudioOutput *m_audioOutput;

    void init();
};

#endif // SUDOKU_SOLO_SRC_BACKEND_BGMPLAYER_H_
