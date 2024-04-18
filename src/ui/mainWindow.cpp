/**
 ******************************************************************************
 * @file           : mainwindow.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/3
 ******************************************************************************
 */

#include "mainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    m_icon = QIcon(":/svg/svg/Icon.svg");
    m_translator = new QTranslator(this);

    m_stackedLayout = new QStackedLayout();
    m_stackedWidget = new StackedWidget();
    m_backgroundWidget = new BackgroundWidget();

    init();
    signalsProcess();
}

MainWindow::~MainWindow() {
    delete ui;
    delete m_translator;
    delete m_stackedLayout;
    delete m_stackedWidget;
    delete m_backgroundWidget;
}

void MainWindow::init() {
    this->setWindowIcon(m_icon);
    this->setWindowFlag(Qt::FramelessWindowHint);

    m_stackedLayout->addWidget(m_backgroundWidget);
    m_stackedLayout->addWidget(m_stackedWidget);
    m_stackedLayout->setContentsMargins(0, 0, 0, 0);
    m_stackedLayout->setStackingMode(QStackedLayout::StackAll);
    ui->gridLayout->addLayout(m_stackedLayout, 0, 0);

    if (m_translator->load(":/language/zh_CN.qm")) {
        QApplication::installTranslator(m_translator);
    }
}

void MainWindow::signalsProcess() {
}

bool MainWindow::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
        return true;
    }

    return QMainWindow::eventFilter(object, event);
}

bool MainWindow::event(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }

    return QMainWindow::event(event);
}
