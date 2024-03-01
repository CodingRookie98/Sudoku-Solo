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

    icon = QIcon(":/svg/Icon.svg");
    translator_ = new QTranslator(this);
    grid_layout_ = new QGridLayout(ui->centralwidget);
    gamingWidget_ = new GamingWidget();

    init();
    signalsProcess();
}

MainWindow::~MainWindow() {
    delete ui;
    delete translator_;
    delete grid_layout_;
    delete gamingWidget_;
}

void MainWindow::init() {
    this->setWindowIcon(icon);

    ui->centralwidget->setLayout(grid_layout_);
    ui->centralwidget->layout()->addWidget(gamingWidget_);

    if (translator_->load(":/language/zh_CN.qm")) {
        QApplication::installTranslator(translator_);
    }
}

void MainWindow::signalsProcess() {
    connect(ui->actionAbout, &QAction::triggered, [&] {
        AboutDialog aboutDialog(this);
        aboutDialog.exec();
    });

    connect(ui->actionEngLish, &QAction::triggered, [&] {
        if (translator_->load(":/language/en_US.qm")) {
            QApplication::installTranslator(translator_);
        }
    });
    connect(ui->actionChinese, &QAction::triggered, [&] {
        if (translator_->load(":/language/zh_CN.qm")) {
            QApplication::installTranslator(translator_);
        }
    });
}

bool MainWindow::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        qDebug() << "update language for MainWindow success";
        ui->retranslateUi(this);
        return true;
    }

    qDebug() << "update language for MainWindow failed";
    return QMainWindow::eventFilter(object, event);
}

bool MainWindow::event(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }

    return QMainWindow::event(event);
}
