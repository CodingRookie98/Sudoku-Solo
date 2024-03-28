/**
 ******************************************************************************
 * @file           : sudokuMatrixWidget.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/23
 ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_SudokuMatrixWidget.h" resolved

#include "sudokuMatrixWidget.h"
#include "ui_SudokuMatrixWidget.h"

SudokuMatrixWidget::SudokuMatrixWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::SudokuMatrixWidget) {
    ui->setupUi(this);

    m_gridLayoutBoard = new QGridLayout;
    m_gridLayoutBoxes = new std::vector<std::vector<QGridLayout *>>;
    m_buttonGroup = nullptr;
    m_buttons = new std::vector<std::vector<BtnSudokuUnit *>>;

    init();
    signalsProcess();
}

SudokuMatrixWidget::~SudokuMatrixWidget() {
    delete ui;
    deleteButtons();
    delete m_buttons;
    deleteBoxes();
    delete m_gridLayoutBoxes;
    delete m_gridLayoutBoard;
    delete m_buttonGroup;
}

void SudokuMatrixWidget::buildMatrix(const Sudoku::SudokuMatrix &matrix) {
    m_sudokuMatrix = std::make_unique<Sudoku::SudokuMatrix>(matrix);

    updateBoard();
    updateBoxes();
    updateButtons();
    updateButtonGroup();
}

void SudokuMatrixWidget::init() {
    m_gridLayoutBoard->setAlignment(Qt::AlignmentFlag::AlignCenter);
    this->setLayout(m_gridLayoutBoard);
    m_gridLayoutBoard->setSpacing(10);
}

void SudokuMatrixWidget::signalsProcess() {
}

void SudokuMatrixWidget::updateBoard() {
}

void SudokuMatrixWidget::updateBoxes() {
    deleteBoxes();

    m_gridLayoutBoxes->resize(m_sudokuMatrix->getRowCount() / m_sudokuMatrix->getBoxRowCount());
    for (int i = 0; i < m_sudokuMatrix->getRowCount() / m_sudokuMatrix->getBoxRowCount(); ++i) {
        std::vector<QGridLayout *> row;
        row.resize(m_sudokuMatrix->getColumnCount() / m_sudokuMatrix->getBoxColumnCount());
        for (int j = 0; j < m_sudokuMatrix->getColumnCount() / m_sudokuMatrix->getBoxColumnCount(); ++j) {
            row.at(j) = new QGridLayout();
            row.at(j)->setSpacing(2);
            m_gridLayoutBoard->addLayout(row.at(j), i, j);
        }
        m_gridLayoutBoxes->at(i) = row;
    }
}

void SudokuMatrixWidget::updateButtons() {
    deleteButtons();

    m_buttons->resize(m_sudokuMatrix->getRowCount());
    for (int i = 0; i < m_sudokuMatrix->getRowCount(); ++i) {
        std::vector<BtnSudokuUnit *> row;
        row.resize(m_sudokuMatrix->getColumnCount());
        for (int j = 0; j < m_sudokuMatrix->getColumnCount(); ++j) {
            QString text;
            if (m_sudokuMatrix->getValue(i, j) == 0) {
                text = "";
            } else {
                text = QString::number(m_sudokuMatrix->getValue(i, j));
            }

            row.at(j) = new BtnSudokuUnit(nullptr, i, j, text);
            row.at(j)->setCheckable(true);
            auto pGridLayout = m_gridLayoutBoxes->at(i / m_sudokuMatrix->getBoxRowCount())
                                   .at(j / m_sudokuMatrix->getBoxColumnCount());
            pGridLayout->addWidget(row.at(j), i % m_sudokuMatrix->getBoxRowCount(),
                                   j % m_sudokuMatrix->getBoxColumnCount(), Qt::AlignCenter);
        }
        m_buttons->at(i) = row;
    }
}

void SudokuMatrixWidget::deleteBoxes() {
    for (auto &row : *m_gridLayoutBoxes) {
        for (auto &gridLayout : row) {
            delete gridLayout;
        }
    }
    m_gridLayoutBoxes->clear();
}

void SudokuMatrixWidget::deleteButtons() {
    for (auto &row : *m_buttons) {
        for (auto &button : row) {
            delete button;
        }
    }
    m_buttons->clear();
}

void SudokuMatrixWidget::updateButtonGroup() {
    delete m_buttonGroup;
    m_buttonGroup = new QButtonGroup;

    for (auto &row : *m_buttons) {
        for (auto &button : row) {
            m_buttonGroup->addButton(button);
        }
    }

    m_buttonGroup->setExclusive(true);

    connect(m_buttonGroup, &QButtonGroup::buttonClicked, this, [&](QAbstractButton *abstractButton) {
        emit sigUnitClicked(qobject_cast<BtnSudokuUnit *>(abstractButton));
    });
}

bool SudokuMatrixWidget::setMatrixValue(const int &rowIndex, const int &columnIndex, const int &value) {
    if (value < 0 || value > m_sudokuMatrix->getNums().size()) {
        return false;
    }
    if (value == 0) {
        m_buttons->at(rowIndex).at(columnIndex)->setText("");
    } else {
        m_buttons->at(rowIndex).at(columnIndex)->setText(QString::number(value));
    }
    m_sudokuMatrix->setValue(rowIndex, columnIndex, value, false);

    if (m_sudokuMatrix->isSudokuFilled()) {
        emit sigIsFilled();
    }

    return true;
}

bool SudokuMatrixWidget::setButtonEnable(const int &rowIndex, const int &columnIndex, const bool &enable) {
    if (rowIndex >= m_buttons->size() || columnIndex >= m_buttons->front().size()) {
        return false;
    }
    m_buttons->at(rowIndex).at(columnIndex)->setEnabled(enable);
    return true;
}

Sudoku::SudokuMatrix SudokuMatrixWidget::getSudokuMatrix() const {
    if (m_sudokuMatrix == nullptr) {
        throw __FUNCTIONW__ + __LINE__ + wchar_t(" matrix is nullptr");
    }
    return *m_sudokuMatrix;
}

bool SudokuMatrixWidget::isSolved() const {
    if (m_sudokuMatrix->isSudokuFilled() && m_sudokuMatrix->getInvalidPositions().empty()) {
        return true;
    }
    return false;
}

BtnSudokuUnit *SudokuMatrixWidget::getCheckedButton() const {
    if (m_buttonGroup == nullptr) {
        return nullptr;
    }
    return qobject_cast<BtnSudokuUnit *>(m_buttonGroup->checkedButton());
}
