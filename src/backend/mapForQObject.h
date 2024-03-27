/**
 ******************************************************************************
 * @file           : mapForQObject.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/15
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_UI_COMPONENTS_MAPFORQOBJECT_H_
#define SUDOKU_SOLO_SRC_UI_COMPONENTS_MAPFORQOBJECT_H_

#include <map>
#include <mutex>
#include <QObject>
#include <typeinfo>

template <typename T>
struct TypeName {
    static constexpr const char *get() {
        return typeid(T).name();
    }
};

class MapForQObject {
public:
    static MapForQObject *getInstance();
    MapForQObject(const MapForQObject &) = delete;            // 处理下构造函数，避免拷贝
    MapForQObject &operator=(const MapForQObject &) = delete; // 把=赋值号重载了，避免赋值

    [[nodiscard]] bool isExist(const std::string &typeName);
    [[nodiscard]] QObject *getObject(const std::string &typeName);
    void registerObject(const std::string &typeName, QObject *qObject);

private:
    // 私有构造函数，防止外部通过构造函数创建实例
    MapForQObject();
    // 私有析构函数，当然这块也可以写自己的清理逻辑
    ~MapForQObject();

    std::unique_ptr<std::map<std::string, QObject *>> m_objects;
    std::mutex m_mutex;
};

#endif // SUDOKU_SOLO_SRC_UI_COMPONENTS_MAPFORQOBJECT_H_
