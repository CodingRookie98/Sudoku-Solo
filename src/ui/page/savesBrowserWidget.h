/**
 ******************************************************************************
 * @file           : savesBrowserWidget.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/21
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_UI_PAGE_SAVESBROWSERWIDGET_H_
#define SUDOKU_SOLO_SRC_UI_PAGE_SAVESBROWSERWIDGET_H_

#include <QWidget>
#include <QFileSystemModel>
#include <QModelIndex>
#include <QFileSystemWatcher>
#include <unordered_set>
#include "gameManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SavesBrowserWidget;
}
QT_END_NAMESPACE

class SavesBrowserWidget : public QWidget {
    Q_OBJECT

public:
    explicit SavesBrowserWidget(QWidget *parent = nullptr);
    ~SavesBrowserWidget() override;

    void updateBtnNextAndBtnPreStatus();

signals:
    void sigBackToHome();
    void sigStartGame();

private:
    Ui::SavesBrowserWidget *ui;
    QFileSystemModel *m_fileSystemModel;
    std::vector<SudokuGameData> *m_sudokuGameData;
    unsigned int m_indexForGameData;
    QModelIndex *m_currentModelIndex;
    QFileSystemWatcher *m_fileSystemWatcher;

    void init();
    void signalsProcess();
    void creatNewSave();
    void deleteSave(const QString &filePath);
    void updateTabWidget();
    void loadGame();
    void updateLabelTextAndBtnStatus();
};

#endif // SUDOKU_SOLO_SRC_UI_PAGE_SAVESBROWSERWIDGET_H_
