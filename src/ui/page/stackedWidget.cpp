/**
 ******************************************************************************
 * @file           : stackedWidget.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/12
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_StackedWidget.h" resolved

#include "stackedWidget.h"
#include "GameSettingWidget.h"
#include "ui_StackedWidget.h"
#include "mapForQObject.h"
#include <qapplication.h>

StackedWidget::StackedWidget(QWidget *parent) :
    QStackedWidget(parent), ui(new Ui::StackedWidget) {
    ui->setupUi(this);

    m_mapWidgets = new std::map<QWidget *, int>;
    m_stackWidgets = new std::stack<QWidget *>;
    m_homeWidget = new HomeWidget();
    m_gamePrepareWidget = nullptr;
    m_gamePauseWidget = nullptr;
    m_gamingWidget = nullptr;
    m_gameSettingWidget = nullptr;

    init();
    signalsProcess();
}

StackedWidget::~StackedWidget() {
    delete ui;

    for (const auto &val : *m_mapWidgets) {
        delete val.first;
    }
    delete m_mapWidgets;
}

void StackedWidget::init() {
    m_mapWidgets->insert(std::make_pair(qobject_cast<QWidget *>(m_homeWidget), this->addWidget(m_homeWidget)));
    MapForQObject::getInstance()->registerObject(TypeName<StackedWidget>::get(), this);
}

void StackedWidget::signalsProcess() {
    if (m_homeWidget != nullptr) {
        homeWidgetSignalsProcess();
    }
}

void StackedWidget::gamePrepareWidgetSignalsProcess() {
    connect(m_gamePrepareWidget, &GamePrepareWidget::sigCancel, this, [&] {
        this->setCurrentIndex(m_mapWidgets->at(qobject_cast<QWidget *>(m_homeWidget)));
    });

    connect(m_gamePrepareWidget, &GamePrepareWidget::sigStart, this, [&](const Sudoku::SudokuMatrix::SudokuMatrixType &sudokuMatrixType) {
        if (m_gamingWidget == nullptr) {
            m_gamingWidget = new GamingWidget;
            m_mapWidgets->insert(std::make_pair(qobject_cast<QWidget *>(m_gamingWidget), this->addWidget(m_gamingWidget)));
            gamingWidgetSignalsProcess();
        }

        m_gamingWidget->generateSudokuMatrix(sudokuMatrixType);
        this->setCurrentIndex(m_mapWidgets->at(qobject_cast<QWidget *>(m_gamingWidget)));
    });
}

void StackedWidget::switchWidget(QWidget *widget) {
    this->setCurrentIndex(m_mapWidgets->at(widget));
}

void StackedWidget::gamePauseWidgetSignalsProcess() {
    connect(m_gamePauseWidget, &GamePauseWidget::sigKeyEscRelease, this, [&] {
        this->setCurrentIndex(m_mapWidgets->at(qobject_cast<QWidget *>(m_gamingWidget)));
        m_gamingWidget->startTimeCount();
    });

    connect(m_gamePauseWidget, &GamePauseWidget::sigBackToGame, this, [&] {
        this->setCurrentIndex(m_mapWidgets->at(qobject_cast<QWidget *>(m_gamingWidget)));
    });

    connect(m_gamePauseWidget, &GamePauseWidget::sigSaveGame, this, [&] {
        m_gamingWidget->saveGame();
    });

    connect(m_gamePauseWidget, &GamePauseWidget::sigLoadGameSave, this, [&] {

    });

    connect(m_gamePauseWidget, &GamePauseWidget::sigGameSetting, this, [&] {
        if (m_gameSettingWidget == nullptr) {
            m_gameSettingWidget = new GameSettingWidget;
            m_mapWidgets->insert(std::make_pair(qobject_cast<QWidget *>(m_gameSettingWidget), this->addWidget(m_gameSettingWidget)));
            gameSettingWidgetSignalsProcess();
        }

        m_stackWidgets->push(m_gamePauseWidget);
        this->setCurrentIndex(m_mapWidgets->at(qobject_cast<QWidget *>(m_gameSettingWidget)));
    });

    connect(m_gamePauseWidget, &GamePauseWidget::sigHome, this, [&] {
        this->setCurrentIndex(m_mapWidgets->at(qobject_cast<QWidget *>(m_homeWidget)));
    });

    connect(m_gamePauseWidget, &GamePauseWidget::sigExitGame, this, [&] {
        // Todo 通知保存游戏状态

        QApplication::closeAllWindows();
    });
}

void StackedWidget::gamingWidgetSignalsProcess() {
    connect(m_gamingWidget, &GamingWidget::sigKeyEscRelease, this, [&] {
        if (m_gamePauseWidget == nullptr) {
            m_gamePauseWidget = new GamePauseWidget;
            m_mapWidgets->insert(std::make_pair(qobject_cast<QWidget *>(m_gamePauseWidget), this->addWidget(m_gamePauseWidget)));
            gamePauseWidgetSignalsProcess();
        }
        this->setCurrentIndex(m_mapWidgets->at(qobject_cast<QWidget *>(m_gamePauseWidget)));
    });
}

void StackedWidget::gameSettingWidgetSignalsProcess() {
    connect(m_gameSettingWidget, &GameSettingWidget::sigFnished, this, [&] {
        QWidget *preWidget = m_stackWidgets->top();
        m_stackWidgets->pop();
        this->setCurrentIndex(m_mapWidgets->at(preWidget));
    });
}

void StackedWidget::homeWidgetSignalsProcess() {
    // 点击新游戏
    connect(m_homeWidget, &HomeWidget::sigNewGame, this, [&] {
        if (m_gamePrepareWidget == nullptr) {
            m_gamePrepareWidget = new GamePrepareWidget;
            m_mapWidgets->insert(std::make_pair(qobject_cast<QWidget *>(m_gamePrepareWidget), this->addWidget(m_gamePrepareWidget)));
            gamePrepareWidgetSignalsProcess();
        }
        this->setCurrentIndex(m_mapWidgets->at(qobject_cast<QWidget *>(m_gamePrepareWidget)));
    });

    connect(m_homeWidget, &HomeWidget::sigGameSetting, this, [&] {
        if (m_gameSettingWidget == nullptr) {
            m_gameSettingWidget = new GameSettingWidget;
            m_mapWidgets->insert(std::make_pair(qobject_cast<QWidget *>(m_gameSettingWidget), this->addWidget(m_gameSettingWidget)));
            gameSettingWidgetSignalsProcess();
        }

        m_stackWidgets->push(m_homeWidget);
        this->setCurrentIndex(m_mapWidgets->at(qobject_cast<QWidget *>(m_gameSettingWidget)));
    });
}
