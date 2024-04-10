/**
 ******************************************************************************
 * @file           : gameManager.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/21
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_BACKEND_GAMEMANAGER_H_
#define SUDOKU_SOLO_SRC_BACKEND_GAMEMANAGER_H_

#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>
#include <QString>
#include <vector>
#include <shared_mutex>
#include <QObject>
#include "sudokuMatrix.h"

struct SudokuGameData {
    QDateTime m_gameDatetime;
    unsigned int m_gameSpentTime;
    std::shared_ptr<Sudoku::SudokuMatrix> m_answerMatrix = nullptr;
    std::shared_ptr<Sudoku::SudokuMatrix> m_originalMatrix = nullptr;
    std::shared_ptr<Sudoku::SudokuMatrix> m_workMatrix = nullptr;
};

class GameManager : public QObject {
    Q_OBJECT
public:
    GameManager(const GameManager &) = delete;            // 处理下构造函数，避免拷贝
    GameManager &operator=(const GameManager &) = delete; // 把=赋值号重载了，避免赋值

    static GameManager *getInstance();
    std::vector<SudokuGameData> loadWithPath(const QString &saveFilePath);
    SudokuGameData loadWithJsonObject(const QJsonObject &jsonObject);
    SudokuGameData loadWithId(const QString &gameId);

    void save(const Sudoku::SudokuMatrix &originalMatrix,
              const Sudoku::SudokuMatrix &answerMatrix,
              const Sudoku::SudokuMatrix &workMatrix, const int &timeSpent);
    void setSaveFilePath(const QString &filePath);

signals:
    void lastGameIsExist();

private:
    GameManager(QObject *parent = nullptr);
    ~GameManager() override;

    QJsonDocument *m_saveJsonDocument;
    QJsonParseError *m_jsonParseError;
    QJsonObject *m_jsonObjectRoot;
    QJsonArray *m_jsonSavesArray;
    std::shared_mutex m_mutex;

    QString m_jsonFilePath;

    const QString m_gameId = "GameID";
    const QString m_saveDateTime = "SaveDateTime";
    const QString m_saves = "Saves";
    const QString m_gameDateTime = "GameDateTime";
    const QString m_gameTimeSpent = "GameTimeSpent";
    const QString m_sudokuMatrixType = "SudokuMatrixType";
    const QString m_sudokuMatrixSize = "SudokuMatrixSize";
    const QString m_originalMatrix = "OriginalMatrix";
    const QString m_workerMatrix = "WorkerMatrix";
    const QString m_answerMatrix = "AnswerMatrix";

    void init();
    void writeTofile();
};

#endif // SUDOKU_SOLO_SRC_BACKEND_GAMEMANAGER_H_
