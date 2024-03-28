/**
 ******************************************************************************
 * @file           : gameManager.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/21
 ******************************************************************************
 */

#include <mutex>
#include <QDir>
#include "gameManager.h"
#include "gameSettings.h"

GameManager::GameManager() {
    m_saveJsonDocument = nullptr;
    m_jsonParseError = new QJsonParseError;
    m_jsonObjectRoot = nullptr;
    m_jsonSavesArray = nullptr;

    init();
}

GameManager::~GameManager() {
    delete m_saveJsonDocument;
    delete m_jsonParseError;
    delete m_jsonObjectRoot;
    delete m_jsonSavesArray;
}

GameManager *GameManager::getInstance() {
    static GameManager *instance;
    static std::mutex mutex; // 静态互斥锁，你也可以用其他锁
    // 第一次检查，不加锁判断，主要检查实例是否已经创建（无性能消耗）
    if (instance == nullptr) { //[这个判断，主要是为了避免以后真正创建实例后，每次判断都会导致的先加锁问题，这种方式java单列也是这样，老方法了]
        // 如果实例没有创建，此处开始加锁，保证只有一个线程，可以往下进行创建实例。
        std::lock_guard<std::mutex> lock(mutex);
        // 第二次检查，这是加锁了的
        if (instance == nullptr) {
            // 如果实例真的没有创建，则开始创建
            instance = new GameManager;
            // 注册析构函数，确保在程序退出时释放单例对象的内存
            std::atexit([]() {
                delete instance;
            });
        }
    }
    return instance;
}

std::vector<SudokuGameData> GameManager::loadWithPath(const QString &saveFilePath) {
    QFile loadFile(saveFilePath);
    if (!loadFile.exists()) {
        // Todo log: 文件不存在

        return std::vector<SudokuGameData>{};
    } else {
        if (!loadFile.open(QIODevice::ReadOnly)) {
            // Todo log：文件打开失败
            return {};
        }
    }

    QJsonParseError jsonParseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(loadFile.readAll(), &jsonParseError);
    if (jsonParseError.error != QJsonParseError::NoError) {
        // Todo log: json解析错误
        return {};
    }

    QJsonArray jsonArray = jsonDocument.object()[m_saves].toArray();
    std::vector<SudokuGameData> sudokuGameData;

    for (auto iter = jsonArray.constBegin(); iter != jsonArray.constEnd(); ++iter) {
        SudokuGameData data = loadWithJsonObject(iter->toObject());
        sudokuGameData.push_back(data);
    }

    return sudokuGameData;
}

void GameManager::save(const Sudoku::SudokuMatrix &originalMatrix, const Sudoku::SudokuMatrix &answerMatrix, const Sudoku::SudokuMatrix &workMatrix, const int &timeSpent) {
    if (m_jsonFilePath.isEmpty()) {
        setSaveFilePath(QString("./save/") + QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss") + ".json");
    }

    QJsonObject jsonObjectNew;
    jsonObjectNew[m_gameDateTime] = QDateTime::currentDateTime().toString(Qt::DateFormat::ISODateWithMs);
    jsonObjectNew[m_gameTimeSpent] = timeSpent;

    QString id;
    QString qStringOriginalMatrix;
    QString qStringAnswerMatrix;
    QString qStringWorkMatrix;
    for (int row = 0; row < originalMatrix.getRowCount(); ++row) {
        for (int col = 0; col < originalMatrix.getColumnCount(); ++col) {
            id.push_back(QString::number(originalMatrix.getValue(row, col)));
            qStringOriginalMatrix.push_back(QString::number(originalMatrix.getValue(row, col)));
            qStringAnswerMatrix.push_back(QString::number(answerMatrix.getValue(row, col)));
            qStringWorkMatrix.push_back(QString::number(workMatrix.getValue(row, col)));
        }
    }

    const Sudoku::SudokuMatrix::SudokuMatrixType type = originalMatrix.getSudokuType();

    std::shared_lock<std::shared_mutex> sharedLock(m_mutex);
    QJsonArray::iterator jsonArrayIter;
    if (m_jsonSavesArray == nullptr) {
        m_jsonSavesArray = new QJsonArray;
        jsonArrayIter = m_jsonSavesArray->begin();

        jsonObjectNew[m_gameId] = id;
        jsonObjectNew[m_sudokuMatrixType] = type;
        jsonObjectNew[m_sudokuMatrixSize] = originalMatrix.getMatrixSize();
        jsonObjectNew[m_originalMatrix] = qStringOriginalMatrix;
        jsonObjectNew[m_answerMatrix] = qStringAnswerMatrix;
        jsonObjectNew[m_workerMatrix] = qStringWorkMatrix;
    } else {
        jsonArrayIter = m_jsonSavesArray->begin();
        while (jsonArrayIter != m_jsonSavesArray->end()) {
            if ((*jsonArrayIter)[m_gameId].toString() == id) {
                break;
            }
            jsonArrayIter++;
        }

        if (jsonArrayIter != m_jsonSavesArray->end()) {
            // 如果id已经在存档中，则只需要更新workMatrix即可
            jsonObjectNew[m_workerMatrix] = qStringWorkMatrix;
        } else {
            // 如果id不在存档中，则存储所有信息
            jsonObjectNew[m_gameId] = id;
            jsonObjectNew[m_sudokuMatrixType] = type;
            jsonObjectNew[m_sudokuMatrixSize] = originalMatrix.getMatrixSize();
            jsonObjectNew[m_originalMatrix] = qStringOriginalMatrix;
            jsonObjectNew[m_answerMatrix] = qStringAnswerMatrix;
            jsonObjectNew[m_workerMatrix] = qStringWorkMatrix;
        }
    }

    // 如果此次存储的id已在文件中
    if (jsonArrayIter != m_jsonSavesArray->end()) {
        qsizetype index = jsonArrayIter - m_jsonSavesArray->begin();
        QJsonObject newJsonObject = (*m_jsonSavesArray)[index].toObject();
        newJsonObject[m_gameDateTime] = jsonObjectNew[m_gameDateTime];
        newJsonObject[m_gameTimeSpent] = jsonObjectNew[m_gameTimeSpent];
        newJsonObject[m_workerMatrix] = jsonObjectNew[m_workerMatrix];
        (*m_jsonSavesArray)[index] = newJsonObject;
    } else { // 将新的游戏数据添加到当前存档
        m_jsonSavesArray->append(jsonObjectNew);
    }

    (*m_jsonObjectRoot)[m_saves] = *m_jsonSavesArray;

    m_saveJsonDocument->setObject(*m_jsonObjectRoot);

    GameSettings::getInstance()->setSetting(GameSettings::getInstance()->m_lastGameSaveName, QJsonValue(m_jsonFilePath));
    GameSettings::getInstance()->setSetting(GameSettings::getInstance()->m_lastGameId, QJsonValue(id));

    std::thread saveFileThread(&GameManager::writeTofile, this);
    saveFileThread.detach();
}

void GameManager::setSaveFilePath(const QString &filePath) {
    // 设置游戏存档路径
    m_jsonFilePath = filePath;
    QFile saveFile(m_jsonFilePath);

    // 如果文件不存在
    if (!saveFile.exists()) {
        if (!saveFile.open(QIODevice::ReadWrite)) {
            // Todo 日志输出文件创建失败
            //            qDebug() << "Failed to create file:" << saveFile.errorString();
            return;
        }
        saveFile.close();
    }

    // 文件存在则只读打开
    if (!saveFile.isOpen()) {
        if (!saveFile.open(QIODevice::ReadOnly)) {
            // Todo log: 以只读模式打开文件失败
            return;
        }
    }
    m_saveJsonDocument = new QJsonDocument(QJsonDocument::fromJson(saveFile.readAll(), m_jsonParseError));
    saveFile.close();

    // 如果解析没有错误
    if (m_jsonParseError->error == QJsonParseError::ParseError::NoError) {
        if (!m_saveJsonDocument->isEmpty()) {
            m_jsonObjectRoot = new QJsonObject(m_saveJsonDocument->object());
            m_jsonSavesArray = new QJsonArray((*m_saveJsonDocument)[m_saves].toArray());

            // Begin 将游戏存档中游戏时间最靠后的一次游戏设为最后游玩游戏
            QJsonObject maxJsonObject;
            if (!m_jsonSavesArray->isEmpty()) {
                maxJsonObject = m_jsonSavesArray->first().toObject();
                for (QJsonValueRef jsonValue : *m_jsonSavesArray) {
                    QJsonObject curJsonObject = jsonValue.toObject();
                    if (QDateTime::fromString(maxJsonObject[m_gameDateTime].toString(), Qt::DateFormat::ISODateWithMs)
                        < QDateTime::fromString(curJsonObject[m_gameDateTime].toString(), Qt::DateFormat::ISODateWithMs)) {
                        maxJsonObject = curJsonObject;
                    }
                }

                GameSettings::getInstance()->setSetting(GameSettings::getInstance()->m_lastGameSaveName, m_jsonFilePath);
                GameSettings::getInstance()->setSetting(GameSettings::getInstance()->m_lastGameId, maxJsonObject[m_gameId]);
            }
            // End

        } else {
            m_jsonObjectRoot = new QJsonObject;
        }
    } else {
        m_jsonObjectRoot = new QJsonObject;
    }

    (*m_jsonObjectRoot)[m_saveDateTime] = QDateTime::currentDateTime().toString(Qt::DateFormat::ISODateWithMs);
}

void GameManager::init() {
    QDir dir("./save");

    if (!dir.exists()) {
        if (!dir.mkdir(".")) {
            // Todo 日志输出创建目录失败
        }
    }
}

void GameManager::writeTofile() {
    QFile file(m_jsonFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        // Todo 日志输出文件打开失败
        //        qDebug() << __FUNCTION__ << ":" << __LINE__;
    } else {
        QTextStream stream(&file);
        stream.setEncoding(QStringConverter::Encoding::Utf8);

        std::shared_lock<std::shared_mutex> sharedLock(m_mutex);
        stream << m_saveJsonDocument->toJson(QJsonDocument::JsonFormat::Indented);
        file.close();
    }
}

SudokuGameData GameManager::loadWithId(const QString &gameId) {
    for (QJsonValueRef jsonValue : *m_jsonSavesArray) {
        if (jsonValue.isObject()) {
            QJsonObject jsonObject = jsonValue.toObject();
            if (jsonObject[m_gameId] == gameId) {
                return loadWithJsonObject(jsonObject);
            }
        }
    }

    return {};
}

SudokuGameData GameManager::loadWithJsonObject(const QJsonObject &jsonObject) {
    SudokuGameData sudoData;
    Sudoku::SudokuMatrix::SudokuMatrixType matrixType;
    switch (jsonObject[m_sudokuMatrixType].toInt()) {
    case Sudoku::SudokuMatrix::FourFour:
        matrixType = Sudoku::SudokuMatrix::FourFour;
        break;
    case Sudoku::SudokuMatrix::SixSix:
        matrixType = Sudoku::SudokuMatrix::SixSix;
        break;
    case Sudoku::SudokuMatrix::NineNine:
        matrixType = Sudoku::SudokuMatrix::NineNine;
        break;
    }

    if (jsonObject[m_originalMatrix].toString().size() != Sudoku::SudokuMatrix::getMatrixSize(matrixType)) {
        // Todo log:
        return {};
    }

    sudoData.m_originalMatrix = std::make_shared<Sudoku::SudokuMatrix>(Sudoku::SudokuMatrix(matrixType));
    sudoData.m_answerMatrix = std::make_shared<Sudoku::SudokuMatrix>(Sudoku::SudokuMatrix(matrixType));
    sudoData.m_workMatrix = std::make_shared<Sudoku::SudokuMatrix>(Sudoku::SudokuMatrix(matrixType));

    for (int index = 0; index < jsonObject[m_originalMatrix].toString().size(); ++index) {
        int row = index / sudoData.m_originalMatrix->getRowCount();
        int col = index % sudoData.m_originalMatrix->getColumnCount();
        int originalMatrixValue = QString(jsonObject[m_originalMatrix].toString()[index]).toInt();
        int answerMatrixValue = QString(jsonObject[m_answerMatrix].toString()[index]).toInt();
        int workMatrixValue = QString(jsonObject[m_workerMatrix].toString()[index]).toInt();

        sudoData.m_originalMatrix->setValue(row, col, originalMatrixValue, true);
        sudoData.m_answerMatrix->setValue(row, col, answerMatrixValue, true);
        sudoData.m_workMatrix->setValue(row, col, workMatrixValue, false);
    }

    sudoData.m_gameDatetime = QDateTime(QDateTime::fromString(jsonObject[m_gameDateTime].toString(), Qt::DateFormat::ISODateWithMs));
    sudoData.m_gameSpentTime = jsonObject[m_gameTimeSpent].toInt();

    return sudoData;
}
