/**
 ******************************************************************************
 * @file           : logger.cpp
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/4/5
 ******************************************************************************
 */

#include <QThreadPool>
#include "logger.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/async.h"

Logger::Logger() :
    QRunnable() {
    m_logger = spdlog::daily_logger_mt<spdlog::async_factory>("daily_logger", "logs/log.log", 23, 59);
    m_logger->set_level(spdlog::level::level_enum::trace);
    setAutoDelete(false);

    m_message = new QString;
    m_level = LogLevel::Trace;
}

Logger::~Logger() {
    m_logger.reset();
    delete m_message;
}

void Logger::log(const Logger::LogLevel &level, const QString &message) {
    m_level = level;
    *m_message = message;
    QThreadPool::globalInstance()->start(this);
}

Logger *Logger::getInstance() {
    static Logger *instance;
    static std::mutex mutex; // 静态互斥锁，你也可以用其他锁
    // 第一次检查，不加锁判断，主要检查实例是否已经创建（无性能消耗）
    if (instance == nullptr) { //[这个判断，主要是为了避免以后真正创建实例后，每次判断都会导致的先加锁问题，这种方式java单列也是这样，老方法了]
        // 如果实例没有创建，此处开始加锁，保证只有一个线程，可以往下进行创建实例。
        std::lock_guard<std::mutex> lock(mutex);
        // 第二次检查，这是加锁了的
        if (instance == nullptr) {
            // 如果实例真的没有创建，则开始创建
            instance = new Logger;
            // 注册析构函数，确保在程序退出时释放单例对象的内存
            std::atexit([]() {
                delete instance;
            });
        }
    }
    return instance;
}

void Logger::run() {
    switch (m_level) {
    case LogLevel::Trace:
        m_logger->trace(m_message->toStdString());
        break;
    case LogLevel::Debug:
        m_logger->debug(m_message->toStdString());
        break;
    case LogLevel::Info:
        m_logger->info(m_message->toStdString());
        break;
    case LogLevel::Warn:
        m_logger->warn(m_message->toStdString());
        break;
    case LogLevel::Error:
        m_logger->error(m_message->toStdString());
        break;
    case LogLevel::Critical:
        m_logger->critical(m_message->toStdString());
        break;
    default:
        break;
    }
    m_logger->flush();
    m_message->clear();
}
