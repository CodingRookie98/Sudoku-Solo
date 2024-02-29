/**
 ******************************************************************************
 * @file           : wdgetSudokuGrid.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/3
 ******************************************************************************
 */

#include <filesystem>
#include <QErrorMessage>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include "sudokuGridWidget.h"
#include "ui_sudokuGridWidget.h"

SudokuGridWidget::SudokuGridWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::SudokuGridWidget) {
    ui->setupUi(this);

    board = new QGridLayout(this);
    boxesGridLayout = nullptr;
    btns = nullptr;
    btnGroup = new QButtonGroup(this);
    choiceDialog = new NumberChoiceDialog(this);
    sudokuMatrixAnswer = nullptr;
    sudokuMatrixWorker = nullptr;
    generator_ = new Sudoku::SudokuGenerator();
    solverBase_ = nullptr;
    inputSudokuDialog_ = nullptr;

    this->setLayout(board);

    init();
    signalsProcess();
}

SudokuGridWidget::~SudokuGridWidget() {
    delete ui;

    deleteBtns();
    deleteBoxes();

    delete board;
    delete btnGroup;
    delete choiceDialog;
    sudokuMatrixAnswer.reset();
    sudokuMatrixWorker.reset();
    delete generator_;
    delete solverBase_;
    delete inputSudokuDialog_;
}

void SudokuGridWidget::init() {
    board->setAlignment(Qt::AlignCenter);
    board->setSpacing(10);
    btnGroup->setExclusive(true);
}

void SudokuGridWidget::signalsProcess() {
    connect(btnGroup, &QButtonGroup::buttonClicked, this, &SudokuGridWidget::handleButtonClicked);
}

void SudokuGridWidget::handleButtonClicked(QAbstractButton *btn) {
    QPoint buttonBottomRight(QCursor::pos().x(), QCursor::pos().y());
    choiceDialog->move(buttonBottomRight);
    choiceDialog->show();
    connect(choiceDialog, &NumberChoiceDialog::choiceMade, btn, [&](const int &num) {
        auto btn = btnGroup->checkedButton();
        if (num == 0) {
            btn->setText("");
        } else {
            btn->setText(QString::number(num));
        }
        auto *btnUnit = qobject_cast<BtnSudokuUnit *>(btn);
        sudokuMatrixWorker->setValue(btnUnit->getPosForMatrix().first, btnUnit->getPosForMatrix().second, num, false);

        // 消除按钮浅红色背景
        if (btnUnit->isInvalided) {
            btnUnit->isInvalided = false;
            BtnSudokuUnit btnSudokuUnit;
            btnUnit->setStyleSheet(btnSudokuUnit.styleSheet());
        }
    });
}

void SudokuGridWidget::generateRandomSudoku(const Sudoku::SudokuMatrix::SudokuMatrixTypes &sudokuType,
                                            const Sudoku::SudokuGenerator::GenerateMethod &generateMethod,
                                            const int &num) {
    sudokuMatrixAnswer.reset();
    sudokuMatrixAnswer = std::make_shared<Sudoku::SudokuMatrix>(Sudoku::SudokuMatrix(sudokuType));
    *sudokuMatrixAnswer = generator_->generate(*sudokuMatrixAnswer, generateMethod);

    sudokuMatrixWorker.reset();
    sudokuMatrixWorker = std::make_shared<Sudoku::SudokuMatrix>(Sudoku::SudokuMatrix(*sudokuMatrixAnswer));
    sudokuMatrixWorker->removeNumbers(sudokuMatrixWorker->getMatrixSize() - num);

    initBoard();
    updateBtns();
    initChoiceDialog();
}

void SudokuGridWidget::updateBtns() {
    auto invalidPos = sudokuMatrixWorker->getInvalidPositions();
    for (int row = 0; row < sudokuMatrixWorker->getRowCount(); ++row) {
        for (int col = 0; col < sudokuMatrixWorker->getColumnCount(); ++col) {
            auto btn = btns->at(row).at(col);
            auto value = sudokuMatrixWorker->getValue(row, col);

            if (value != 0) {
                btn->setText(QString::number(value));
                btn->setEnabled(false);

                if (invalidPos.contains(std::pair<int, int>(row, col))) {
                    btn->setEnabled(true);
                    btn->setStyleSheet("background-color: lightcoral;");
                }
            }
        }
    }
}

void SudokuGridWidget::deleteBtns() {
    if (btns != nullptr) {
        for (auto &qvec : *btns) {
            for (auto &btn : qvec) {
                delete btn;
            }
        }
    }
    delete btns;
}

void SudokuGridWidget::deleteBoxes() {
    if (boxesGridLayout != nullptr) {
        for (QGridLayout *gridLayout : *boxesGridLayout) {
            delete gridLayout;
        }
    }
    delete boxesGridLayout;
}

// 检查当前数独的合法性
void SudokuGridWidget::checkCurrentSudoku() {
    auto invalidPositions = sudokuMatrixWorker->getInvalidPositions();
    // 数独全部填满且有效
    if (invalidPositions.empty() && sudokuMatrixWorker->isSudokuFilled()) {
        for (auto &vec : *btns) {
            for (auto &btn : vec) {
                btn->setEnabled(false);
            }
        }
        return;
        // Todo 显示本局结束画面面
    }

    for (const auto &pos : invalidPositions) {
        auto *btn = btns->at(pos.first).at(pos.second);
        // 若不合法将按钮背景颜色设为浅红色
        btn->setStyleSheet("background-color: lightcoral;");
        btn->isInvalided = true;
    }
}

void SudokuGridWidget::solveCurrentSudoku() {
    if (solverBase_ == nullptr) {
        solverBase_ = new Sudoku::SolverDL(*sudokuMatrixWorker);
    } else {
        delete solverBase_;
        solverBase_ = new Sudoku::SolverDL(*sudokuMatrixWorker);
    }

    if (sudokuMatrixAnswer != nullptr && sudokuMatrixAnswer->isSudokuFilled()) {
        sudokuMatrixWorker.reset();
        sudokuMatrixWorker = std::make_shared<Sudoku::SudokuMatrix>(Sudoku::SudokuMatrix(*sudokuMatrixAnswer));
        updateBtns();
        emit sigCurrentSudokuIsSolved();
    } else if (solverBase_->solve()) {
        sudokuMatrixAnswer.reset();
        sudokuMatrixWorker.reset();
        sudokuMatrixAnswer = std::make_shared<Sudoku::SudokuMatrix>(Sudoku::SudokuMatrix(solverBase_->getAnswer()));
        sudokuMatrixWorker = std::make_shared<Sudoku::SudokuMatrix>(Sudoku::SudokuMatrix(solverBase_->getAnswer()));
        updateBtns();
        emit sigCurrentSudokuIsSolved();
    } else {
        QMessageBox messageBox(this);
        messageBox.setModal(true);
        messageBox.setText("当前数独无解");
        messageBox.exec();
    }
}

void SudokuGridWidget::saveCurrentSudoku() {
    checkCurrentSudoku();
    if (!sudokuMatrixWorker->getInvalidPositions().empty()) {
        QMessageBox messageBox(this);
        messageBox.setModal(true);
        messageBox.setText("当前数独含有无效值，若保存则不保存无效值");
        messageBox.addButton(QMessageBox::StandardButton::Apply);
        messageBox.addButton(QMessageBox::StandardButton::Cancel);
        messageBox.setDefaultButton(QMessageBox::StandardButton::Apply);
        auto ret = messageBox.exec();
        switch (ret) {
        case QMessageBox::StandardButton::Apply:
            break;
        case QMessageBox::StandardButton::Cancel:
            return;
        default:
            break;
        }
    }

    std::stringstream stringStream;
    sudokuMatrixWorker->printToOstreram(stringStream);
    std::string stringContent(stringStream.str());

    QString fileName = QFileDialog::getSaveFileName(this);
    QFile file(fileName, this);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream textStream(&file);
    textStream << stringContent.c_str();
    file.close();
}

void SudokuGridWidget::readSudokuFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this);
    QFile file(fileName, this);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Todo 日志输出文件打开失败
        //        qDebug() << "Open file failed";
        return;
    }

    QTextStream textStream(&file);
    QString fileContent = textStream.readAll();
    std::vector<int> nums;
    for (auto &ch : fileContent) {
        if (ch.isNumber()) {
            int num = QString(ch).toInt();
            nums.emplace_back(num);
        }
    }
    file.close();

    Sudoku::SudokuMatrix *matrix = nullptr;
    if (nums.size() == Sudoku::SudokuMatrix::getMatrixSize(Sudoku::SudokuMatrix::SudokuMatrixTypes::Four_Four)) {
        matrix = new Sudoku::SudokuMatrix(Sudoku::SudokuMatrix::SudokuMatrixTypes::Four_Four);
    } else if (nums.size() == Sudoku::SudokuMatrix::getMatrixSize(Sudoku::SudokuMatrix::SudokuMatrixTypes::Six_Six)) {
        matrix = new Sudoku::SudokuMatrix(Sudoku::SudokuMatrix::SudokuMatrixTypes::Six_Six);
    } else if (nums.size() == Sudoku::SudokuMatrix::getMatrixSize(Sudoku::SudokuMatrix::SudokuMatrixTypes::Nine_Nine)) {
        matrix = new Sudoku::SudokuMatrix(Sudoku::SudokuMatrix::SudokuMatrixTypes::Nine_Nine);
    } else {
        return;
    }

    sudokuMatrixWorker.reset();
    sudokuMatrixWorker = std::make_shared<Sudoku::SudokuMatrix>(*matrix);
    delete matrix;
    if (sudokuMatrixWorker == nullptr) {
        // Todo 日志输出文件内容不是合法的数独
        //        qDebug() << "The file content is not a legal Sudoku";
        return;
    }

    for (int row = 0; row < sudokuMatrixWorker->getRowCount(); ++row) {
        for (int col = 0; col < sudokuMatrixWorker->getColumnCount(); ++col) {
            int value = nums.at(row * sudokuMatrixWorker->getColumnCount() + col);
            sudokuMatrixWorker->setValue(row, col, value, true);
        }
    }
    if (sudokuMatrixAnswer != nullptr) {
        sudokuMatrixAnswer.reset();
    }

    initBoard();
    updateBtns();
    initChoiceDialog();

    emit sigShowWidget();

    if (sudokuMatrixWorker->isSudokuFilled()) {
        emit sigCurrentSudokuIsSolved();
    }
}

void SudokuGridWidget::initChoiceDialog() {
    if (sudokuMatrixWorker == nullptr) {
        // Todo 日志: 初始化选择对话框时sudokuMatrixWorker is nullPointer
        return;
    }
    choiceDialog->setChoices(sudokuMatrixWorker->getBoxRowCount(), sudokuMatrixWorker->getBoxColumnCount(), sudokuMatrixWorker->getNums());
}

void SudokuGridWidget::initBtns() {
    if (sudokuMatrixWorker != nullptr && btns != nullptr && sudokuMatrixWorker->getMatrixSize() == btns->size()) {
        return;
    }

    if (btns != nullptr) {
        for (auto &vecBtns : *btns) {
            for (auto &btn : vecBtns) {
                btnGroup->removeButton(btn);
            }
        }
    }

    deleteBtns();

    btns = new std::vector<std::vector<BtnSudokuUnit *>>(sudokuMatrixWorker->getRowCount(), std::vector<BtnSudokuUnit *>(sudokuMatrixWorker->getColumnCount(), nullptr));
    for (int row = 0; row < sudokuMatrixWorker->getRowCount(); ++row) {
        for (int col = 0; col < sudokuMatrixWorker->getColumnCount(); ++col) {
            auto *btn = new BtnSudokuUnit(this, row, col, "");
            auto &box = boxesGridLayout->at(row / sudokuMatrixWorker->getBoxRowCount() * sudokuMatrixWorker->getBoxRowCount()
                                            + col / sudokuMatrixWorker->getBoxColumnCount());
            btn->setCheckable(true);
            btns->at(row).at(col) = btn;
            btnGroup->addButton(btn);
            box->addWidget(btn, row % sudokuMatrixWorker->getBoxRowCount(), col % sudokuMatrixWorker->getBoxColumnCount(), Qt::AlignCenter);
        }
    }
}

void SudokuGridWidget::initBoxes() {
    if (sudokuMatrixWorker != nullptr && boxesGridLayout != nullptr && sudokuMatrixWorker->getBoxCount() == boxesGridLayout->size()) {
        return;
    }
    deleteBoxes();
    boxesGridLayout = new std::vector<QGridLayout *>(sudokuMatrixWorker->getBoxCount(), nullptr);
    for (int row = 0; row < sudokuMatrixWorker->getRowCount() / sudokuMatrixWorker->getBoxRowCount(); ++row) {
        for (int col = 0; col < sudokuMatrixWorker->getColumnCount() / sudokuMatrixWorker->getBoxColumnCount(); ++col) {
            auto *grid_layout = new QGridLayout();

            boxesGridLayout->at(row * (sudokuMatrixWorker->getColumnCount() / sudokuMatrixWorker->getBoxColumnCount()) + col) = grid_layout;
            grid_layout->setSpacing(2);
            board->addLayout(grid_layout, row, col, Qt::AlignCenter);
        }
    }
}

void SudokuGridWidget::initBoard() {
    initBoxes();
    initBtns();
}

void SudokuGridWidget::promptAnswer() {
    QAbstractButton *checkedBtn = btnGroup->checkedButton();
    if (checkedBtn == nullptr) {
        QMessageBox messageBox(this);
        messageBox.setModal(true);
        messageBox.setText("Please click where you need the answer");
        messageBox.exec();
    } else {
        if (sudokuMatrixAnswer == nullptr || !sudokuMatrixAnswer->isSudokuFilled()) {
            solveCurrentSudoku();
        }
        auto *btn = qobject_cast<BtnSudokuUnit *>(checkedBtn);
        auto pos = btn->getPosForMatrix();
        btn->setText(QString::number(sudokuMatrixAnswer->getValue(pos.first, pos.second)));
    }
}

void SudokuGridWidget::inputSudoku() {
    if (inputSudokuDialog_ == nullptr) {
        inputSudokuDialog_ = new InputSudokuDialog(this);
    }
    
    connect(inputSudokuDialog_, &InputSudokuDialog::sigInputSudokuMatrix, this, [&](const Sudoku::SudokuMatrix &sudokuMatrix) {
      sudokuMatrixAnswer.reset();
      sudokuMatrixWorker.reset();
      sudokuMatrixWorker = std::make_shared<Sudoku::SudokuMatrix>(sudokuMatrix);
      
      initBoard();
      updateBtns();
      initChoiceDialog();
      
      inputSudokuDialog_->close();
      
      emit sigShowWidget();
    });
    
    inputSudokuDialog_->exec();
}
