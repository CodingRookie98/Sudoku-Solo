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
    title = QString("Sudoku-Solo By CodingRookie");

    actionAbout = new QAction("关于", ui->menuHelp);

    grid_layout_ = new QGridLayout(ui->centralwidget);
    gamingWidget_ = new GamingWidget();

    init();
    signalsProcess();
}

MainWindow::~MainWindow() {
    delete ui;
    delete actionAbout;
    delete grid_layout_;
    delete gamingWidget_;
}

void MainWindow::init() {
    this->setWindowIcon(icon);
    this->setWindowTitle(title);

    ui->menuHelp->addAction(actionAbout);

    ui->centralwidget->setLayout(grid_layout_);
    ui->centralwidget->layout()->addWidget(gamingWidget_);
}

void MainWindow::signalsProcess() {
    connect(actionAbout, &QAction::triggered, [&] {
        AboutDialog aboutDialog(this);
        aboutDialog.exec();
    });
}
