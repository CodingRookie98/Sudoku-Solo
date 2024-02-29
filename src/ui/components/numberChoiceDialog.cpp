/**
 ******************************************************************************
 * @file           : numberChoiceDialog.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/3
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_NumberChoiceDialog.h" resolved

#include "numberChoiceDialog.h"
#include "ui_NumberChoiceDialog.h"

NumberChoiceDialog::NumberChoiceDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::NumberChoiceDialog), btnGroupNumChoices(new QButtonGroup(this)), btnChoices(nullptr) {
    ui->setupUi(this);

    init();
    signalsProcess();
}

NumberChoiceDialog::~NumberChoiceDialog() {
    delete ui;

    delete_btnChoices();
    delete btnGroupNumChoices;
}

void NumberChoiceDialog::init() {
    ui->gridLayout->setSpacing(1);
    ui->gridLayout->setAlignment(Qt::AlignCenter);
    btnGroupNumChoices->setExclusive(true);
    this->setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
    this->setWindowFlag(Qt::FramelessWindowHint);
    //    this->setWindowOpacity(0.5);                      // 设置窗口透明度

    font.setPointSize(14);
    font.setBold(true);

}

void NumberChoiceDialog::signalsProcess() {
    connect(btnGroupNumChoices, &QButtonGroup::buttonClicked, this, [=, this](QAbstractButton *btn) {
        emit choiceMade(btn->text().toInt());
        this->hide();
    });

    connect(ui->btnHide, &QPushButton::clicked, this, &QDialog::hide);
    connect(ui->btnClear, &QPushButton::clicked, this, [=, this]() {
        emit choiceMade(0);
    });
}

void NumberChoiceDialog::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        offset = event->pos();
    }
    if (event->button() == Qt::RightButton) {
        this->hide();
    }
}

void NumberChoiceDialog::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        // 移动窗口位置
        move(event->globalPosition().toPoint() - offset);
    }
}
void NumberChoiceDialog::setChoices(const int &rowCount, const int &columnCount, const std::vector<int> &nums) {
    delete_btnChoices();

    btnChoices = new std::vector<std::vector<QPushButton *>>(rowCount, std::vector<QPushButton *>(columnCount, nullptr));
    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < columnCount; ++j) {
            auto *btn = new QPushButton(QString::number(nums.at(i * columnCount + j)));
            btn->setFixedSize(40, 40);
            btn->setFont(font);
            //            btn->setWindowOpacity(1);
            //            btn->setStyleSheet("background-color: white;");

            btnChoices->at(i).at(j) = btn;
            ui->gridLayout->addWidget(btn, i, j);
            btnGroupNumChoices->addButton(btn);
        }
    }

    this->setFixedSize(this->layout()->sizeHint());
}

void NumberChoiceDialog::delete_btnChoices() {
    if (btnChoices != nullptr) {
        for (auto &vecBtn : *btnChoices) {
            for (auto &btn : vecBtn) {
                btnGroupNumChoices->removeButton(btn);
                delete btn;
            }
        }
        delete btnChoices;
    }
}
