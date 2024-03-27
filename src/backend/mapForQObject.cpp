/**
 ******************************************************************************
 * @file           : mapForQObject.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/15
 ******************************************************************************
 */

#include "mapForQObject.h"

MapForQObject *MapForQObject::getInstance() {
    static MapForQObject *m_instance;
    static std::mutex mutex_; // 静态互斥锁，你也可以用其他锁
    // 第一次检查，不加锁判断，主要检查实例是否已经创建（无性能消耗）
    if (m_instance == nullptr) { //[这个判断，主要是为了避免以后真正创建实例后，每次判断都会导致的先加锁问题，这种方式java单列也是这样，老方法了]
        // 如果实例没有创建，此处开始加锁，保证只有一个线程，可以往下进行创建实例。
        std::lock_guard<std::mutex> lock(mutex_);
        // 第二次检查，这是加锁了的
        if (m_instance == nullptr) {
            // 如果实例真的没有创建，则开始创建
            m_instance = new MapForQObject();
        }
    }
    return m_instance;
}

bool MapForQObject::isExist(const std::string &typeName) {
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    if (m_objects->find(typeName) != m_objects->end()) {
        return true;
    }
    return false;
}

QObject *MapForQObject::getObject(const std::string &typeName) {
    if (isExist(typeName)) {
        return m_objects->at(typeName);
    }
    return nullptr;
}

void MapForQObject::registerObject(const std::string &typeName, QObject *qObject) {
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    m_objects->insert(std::make_pair(typeName, qObject));
}

MapForQObject::MapForQObject() {
    m_objects = std::make_unique<std::map<std::string, QObject *>>();
}

MapForQObject::~MapForQObject() = default;
