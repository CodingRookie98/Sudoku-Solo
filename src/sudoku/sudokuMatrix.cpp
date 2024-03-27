/**
 ******************************************************************************
 * @file           : sudokuMatrix.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/6
 ******************************************************************************
 */

#include "sudokuMatrix.h"
#include <string>
#include <fstream>

namespace Sudoku {

SudokuMatrix::SudokuMatrix(const SudokuMatrixType &sudokuType) {
    m_sudokuType = sudokuType;
    switch (m_sudokuType) {
    case Sudoku::SudokuMatrix::FourFour:
        m_rowCount = m_columnCount = m_numSize = 4;
        m_boxRowCount = m_boxColumnCount = 2;
        break;
    case Sudoku::SudokuMatrix::SixSix:
        m_rowCount = m_columnCount = m_numSize = 6;
        m_boxRowCount = 2;
        m_boxColumnCount = 3;
        break;
    case Sudoku::SudokuMatrix::NineNine:
        m_rowCount = m_columnCount = m_numSize = 9;
        m_boxRowCount = m_boxColumnCount = 3;
        break;
    default:
        break;
    }

    m_matrix = std::vector<std::vector<int>>(m_rowCount, std::vector<int>(m_columnCount, 0));

    for (int val = 1; val <= m_numSize; ++val) {
        m_nums.push_back(val);
    }
}

SudokuMatrix::~SudokuMatrix() = default;

bool SudokuMatrix::setValue(const int &row, const int &col, const int &value, const bool &checkIsValid) {
    // 若不进行插入合法检查
    if (!checkIsValid) {
        if (value != 0 && m_matrix.at(row).at(col) == 0) {
            m_filledNum++;
        } else if (value == 0 && m_matrix.at(row).at(col) != 0) {
            m_filledNum--;
        }
        m_matrix.at(row).at(col) = value;
        // 如果插入的值非法，则将其坐标存储
        if (!isValidValueForPos(row, col, value)) {
            m_invalidPositions.insert(std::pair<int, int>(row, col));
        } else { // 若合法
            // 若插入的位置在非法位置表里，则移除它
            m_invalidPositions.erase((std::pair<int, int>(row, col)));
            checkCurrentInvalidPos();
        }
        return true;
    }

    //  合法插入检查
    if (!isValidValueForPos(row, col, value)) {
        return false;
    }

    if (value != 0 && m_matrix.at(row).at(col) == 0) {
        m_filledNum++;
    } else if (value == 0 && m_matrix.at(row).at(col) != 0) {
        m_filledNum--;
    }
    m_matrix.at(row).at(col) = value;

    // 若插入的位置在非法位置表里，则移除它
    m_invalidPositions.erase((std::pair<int, int>(row, col)));
    checkCurrentInvalidPos();
    return true;
}

bool SudokuMatrix::isValidValueForPos(const int &row, const int &col, const int &value) const {
    if (value == 0) {
        return true;
    }

    // 检查同一行
    for (int i = 0; i < m_columnCount; ++i) {
        if (m_matrix.at(row).at(i) == value && i != col) {
            return false;
        }
    }

    // 检查同一列
    for (int j = 0; j < m_rowCount; ++j) {
        if (m_matrix.at(j).at(col) == value && j != row) {
            return false;
        }
    }

    // 检查同一宫
    int startRow = m_boxRowCount * (row / m_boxRowCount);
    int startCol = m_boxColumnCount * (col / m_boxColumnCount);
    for (int i = 0; i < m_boxRowCount; ++i) {
        for (int j = 0; j < m_boxColumnCount; ++j) {
            if (m_matrix.at(startRow + i).at(startCol + j) == value && (startRow + i) != row && (startCol + j) != col) {
                return false;
            }
        }
    }

    return true;
}

int SudokuMatrix::getRowCount() const {
    return m_rowCount;
}

int SudokuMatrix::getColumnCount() const {
    return m_columnCount;
}

int SudokuMatrix::getBoxRowCount() const {
    return m_boxRowCount;
}

int SudokuMatrix::getBoxColumnCount() const {
    return m_boxColumnCount;
}

std::vector<int> SudokuMatrix::getNums() const {
    return m_nums;
}

void SudokuMatrix::printToOstream(std::ostream &ostream) {
    for (int row = 0; row < m_rowCount; ++row) {
        for (int col = 0; col < m_columnCount; ++col) {
            if (m_invalidPositions.find(std::pair<int, int>(row, col)) == m_invalidPositions.end()) { // c++ 17
                //            if (!m_invalidPositions.contains(std::pair<int, int>(row, col))) { // c++ 20
                ostream << std::to_string(m_matrix.at(row).at(col)) << std::string(", ");
            } else {
                ostream << std::to_string(0) << std::string(", ");
            }
            if ((col + 1) % m_boxColumnCount == 0) {
                ostream << std::string(" ");
            }
        }
        if ((row + 1) % m_boxRowCount == 0) {
            ostream << std::string("\n\n");
        } else {
            ostream << std::string("\n");
        }
    }
}

void SudokuMatrix::removeNumbers(const int &removeNumCount) {
    // 移除一定数量的数字以生成题目
    std::mt19937 rng(std::random_device{}());

    for (int i = 0; i < removeNumCount; ++i) {
        int row = std::uniform_int_distribution<int>(0, m_rowCount - 1)(rng);
        int col = std::uniform_int_distribution<int>(0, m_columnCount - 1)(rng);

        // 如果已经是空的位置，则重新选择位置
        while (m_matrix.at(row).at(col) == 0) {
            row = std::uniform_int_distribution<int>(0, m_rowCount - 1)(rng);
            col = std::uniform_int_distribution<int>(0, m_columnCount - 1)(rng);
        }

        setValue(row, col, 0, true);
    }
}

int SudokuMatrix::getValue(const int &row, const int &col) const {
    return m_matrix.at(row).at(col);
}

int SudokuMatrix::getBoxCount() const {
    return (m_rowCount / m_boxRowCount) * (m_columnCount / m_boxColumnCount);
}

Sudoku::SudokuMatrix::SudokuMatrixType SudokuMatrix::getSudokuType() const {
    return m_sudokuType;
}

int SudokuMatrix::getMatrixSize(const SudokuMatrixType &type) {
    int size;
    switch (type) {
    case Sudoku::SudokuMatrix::FourFour:
        size = 4 * 4;
        break;
    case Sudoku::SudokuMatrix::SixSix:
        size = 6 * 6;
        break;
    case Sudoku::SudokuMatrix::NineNine:
        size = 9 * 9;
        break;
    default:
        break;
    }
    return size;
}

int SudokuMatrix::getMatrixSize() const {
    return m_rowCount * m_columnCount;
}

std::unordered_set<std::pair<int, int>, pair_hash> SudokuMatrix::getInvalidPositions() const {
    return m_invalidPositions;
}

bool SudokuMatrix::isSudokuFilled() const {
    if (m_filledNum == m_rowCount * m_columnCount) {
        return true;
    }
    return false;
}

// 将检查当前所有不合法位置, 若其中的位置合法，则移除
void SudokuMatrix::checkCurrentInvalidPos() {
    std::vector<std::pair<int,int>> vec;
    for (auto &pos : m_invalidPositions) {
        if (isValidValueForPos(pos.first, pos.second, m_matrix.at(pos.first).at(pos.second))) {
            vec.emplace_back(pos);
        }
    }
    for (const auto &pos : vec) {
        m_invalidPositions.erase(pos);
    }
}

int SudokuMatrix::getBoxIndex(const int &row, const int &col) const {
    return (row / m_boxRowCount) * m_boxRowCount + (col / m_boxColumnCount);
}

bool SudokuMatrix::isEmpty() const {
    if (m_filledNum == 0) return true;
    else return false;
}
} // namespace Sudoku