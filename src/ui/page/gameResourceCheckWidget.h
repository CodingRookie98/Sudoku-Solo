/**
 ******************************************************************************
 * @file           : gameResourceCheckWidget.h
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 24-4-28
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_UI_PAGE_GAMERESOURCECHECKWIDGET_H_
#define SUDOKU_SOLO_SRC_UI_PAGE_GAMERESOURCECHECKWIDGET_H_

#include <QWidget>
#include "resourceDownloader.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class GameResourceCheckWidget;
}
QT_END_NAMESPACE

class GameResourceCheckWidget : public QWidget {
    Q_OBJECT

public:
    explicit GameResourceCheckWidget(QWidget *parent = nullptr);
    ~GameResourceCheckWidget() override;

    void start();
    void progressCallback(size_t increment, int64_t transfered, int64_t total, void *userData);
    
signals:
    void sigCheckResourceFinished();
public slots:
    void updateDownloadProgressBar(const int &value);

private:
    Ui::GameResourceCheckWidget *ui;
    ResourceDownloader *m_resourceDownloader;
    int m_filesSize;
    std::map<int, std::string> *m_neededDownloadFiles;

    void init();
    void signalsProcess();
};

#endif // SUDOKU_SOLO_SRC_UI_PAGE_GAMERESOURCECHECKWIDGET_H_
