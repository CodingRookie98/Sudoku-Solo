/**
 ******************************************************************************
 * @file           : backgroundWidget.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/12
 ******************************************************************************
 */

#ifndef TEST__BACKGROUNDWIDGET_H_
#define TEST__BACKGROUNDWIDGET_H_

#include <QWidget>
#include <QWebEngineView>
#include "webEngineView.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class BackgroundWidget;
}
QT_END_NAMESPACE

class BackgroundWidget : public QWidget {
    Q_OBJECT

public:
    explicit BackgroundWidget(QWidget *parent = nullptr);
    ~BackgroundWidget() override;

    void setBackground(const QString &path);
signals:

private:
    Ui::BackgroundWidget *ui;
    WebEngineView *m_webEngineView;

    void init();
    void signalProcess();
};

#endif // TEST__BACKGROUNDWIDGET_H_
