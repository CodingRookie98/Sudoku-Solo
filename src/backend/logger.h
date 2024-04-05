/**
 ******************************************************************************
 * @file           : logger.h
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/4/5
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_BACKEND_LOGGER_H_
#define SUDOKU_SOLO_SRC_BACKEND_LOGGER_H_

#include <QString>
#include <QRunnable>
#include "spdlog/spdlog.h"

class Logger : public QRunnable {
public:
    enum LogLevel {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };

    static Logger *getInstance();
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    void log(const Logger::LogLevel &level, const QString &message);

protected:
    void run() override;

private:
    Logger();
    ~Logger() override;

    std::shared_ptr<spdlog::logger> m_logger;
    LogLevel m_level;
    QString *m_message;
};
#endif // SUDOKU_SOLO_SRC_BACKEND_LOGGER_H_
