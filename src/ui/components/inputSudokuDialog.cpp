/**
 ******************************************************************************
 * @file           : inputSudokuDialog.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/28
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_InputSudokuDialog.h" resolved

#include "inputSudokuDialog.h"
#include "ui_InputSudokuDialog.h"

InputSudokuDialog::InputSudokuDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::InputSudokuDialog) {
    ui->setupUi(this);

    init();
    signalsProcess();
}

InputSudokuDialog::~InputSudokuDialog() {
    delete ui;
}

void InputSudokuDialog::init() {
    ui->plainTextEdit->setPlaceholderText("请在此输入数独矩阵\n"
                                          "数字之间请用空格\" \"或逗号 \",\" 隔开, \"0\" 表示待解决格子");
    setModal(true);
}

void InputSudokuDialog::signalsProcess() {
    connect(ui->btnInput, &QPushButton::clicked, this, &InputSudokuDialog::processInputText);
    connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, this, [&] {
        ui->label->setText("");
    });
}

void InputSudokuDialog::processInputText() {
    QString text = ui->plainTextEdit->toPlainText();
    if (text.isEmpty()) {
        ui->label->setText("请在输入框中输入数独矩阵");
        return;
    } else {
        std::vector<int> nums;
        for (const auto &ch : text) {
            if (!ch.isNumber()) {
                continue;
            }
            int num = QString(ch).toInt();
            nums.emplace_back(num);
        }
        
        int size = nums.size();
        Sudoku::SudokuMatrix::SudokuMatrixTypes sudokuMatrixType;
        if (size == Sudoku::SudokuMatrix::getMatrixSize(Sudoku::SudokuMatrix::Four_Four)) {
            sudokuMatrixType = Sudoku::SudokuMatrix::Four_Four;
        } else if (size == Sudoku::SudokuMatrix::getMatrixSize(Sudoku::SudokuMatrix::Six_Six)) {
            sudokuMatrixType = Sudoku::SudokuMatrix::Six_Six;
        } else if (size == Sudoku::SudokuMatrix::getMatrixSize(Sudoku::SudokuMatrix::Nine_Nine)) {
            sudokuMatrixType = Sudoku::SudokuMatrix::Nine_Nine;
        } else {
            ui->label->setText("输入的矩阵类型不在支持范围内，支持的矩阵有 4*4 6*6 9*9");
            return;
        }
        
        Sudoku::SudokuMatrix sudokuMatrix(sudokuMatrixType);
        for (int row = 0; row < sudokuMatrix.getRowCount(); ++row) {
            for (int col = 0; col < sudokuMatrix.getColumnCount(); ++col) {
                int value = nums.at(row * sudokuMatrix.getColumnCount() + col);
                sudokuMatrix.setValue(row, col, value);
            }
        }
        
        emit sigInputSudokuMatrix(sudokuMatrix);
    }
}
