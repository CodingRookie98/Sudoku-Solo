/**
 ******************************************************************************
 * @file           : webEngineView.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/15
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_UI_COMPONENTS_WEBENGINEVIEW_H_
#define SUDOKU_SOLO_SRC_UI_COMPONENTS_WEBENGINEVIEW_H_

#include <QWebEngineView>

QT_BEGIN_NAMESPACE
namespace Ui {
class WebEngineView;
}
QT_END_NAMESPACE

class WebEngineView : public QWebEngineView {
    Q_OBJECT

public:
    explicit WebEngineView(QWidget *parent = nullptr);
    ~WebEngineView() override;

    void setHtml(const QString &path);
signals:
    void sigKeyEscRelease();

private:
    Ui::WebEngineView *ui;
    void init();
};

#endif // SUDOKU_SOLO_SRC_UI_COMPONENTS_WEBENGINEVIEW_H_
