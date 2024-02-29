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

namespace Sudoku {

SudokuMatrix::SudokuMatrix(const SudokuMatrixTypes &_sudokuType) {
    sudokuType_ = _sudokuType;
    switch (sudokuType_) {
    case Sudoku::SudokuMatrix::Four_Four:
        rowCount = columnCount = numSize = 4;
        boxRowCount = boxColumnCount = 2;
        break;
    case Sudoku::SudokuMatrix::Six_Six:
        rowCount = columnCount = numSize = 6;
        boxRowCount = 2;
        boxColumnCount = 3;
        break;
    case Sudoku::SudokuMatrix::Nine_Nine:
        rowCount = columnCount = numSize = 9;
        boxRowCount = boxColumnCount = 3;
        break;
    default:
        break;
    }

    matrix = std::vector<std::vector<int>>(rowCount, std::vector<int>(columnCount, 0));

    for (int val = 1; val <= numSize; ++val) {
        nums.push_back(val);
    }
}

SudokuMatrix::~SudokuMatrix() = default;

bool SudokuMatrix::setValue(const int &row, const int &col, const int &value, const bool &checkIsValid) {
    // 若不进行插入合法检查
    if (!checkIsValid) {
        if (value != 0 && matrix.at(row).at(col) == 0) {
            filledNum++;
        } else if (value == 0 && matrix.at(row).at(col) != 0) {
            filledNum--;
        }
        matrix.at(row).at(col) = value;
        // 如果插入的值非法，则将其坐标存储
        if (!isValidValueForPos(row, col, value)) {
            invalidPositions.insert(std::pair<int, int>(row, col));
        } else { // 若合法
            // 若插入的位置在非法位置表里，则移除它
            invalidPositions.erase((std::pair<int, int>(row, col)));
            checkCurrentInvalidPos();
        }
        return true;
    }

    //  合法插入检查
    if (!isValidValueForPos(row, col, value)) {
        return false;
    }

    if (value != 0 && matrix.at(row).at(col) == 0) {
        filledNum++;
    } else if (value == 0 && matrix.at(row).at(col) != 0) {
        filledNum--;
    }
    matrix.at(row).at(col) = value;

    // 若插入的位置在非法位置表里，则移除它
    invalidPositions.erase((std::pair<int, int>(row, col)));
    checkCurrentInvalidPos();
    return true;
}

bool SudokuMatrix::isValidValueForPos(const int &row, const int &col, const int &value) const {
    if (value == 0) {
        return true;
    }

    // 检查同一行
    for (int i = 0; i < columnCount; ++i) {
        if (matrix.at(row).at(i) == value && i != col) {
            return false;
        }
    }

    // 检查同一列
    for (int j = 0; j < rowCount; ++j) {
        if (matrix.at(j).at(col) == value && j != row) {
            return false;
        }
    }

    // 检查同一宫
    int startRow = boxRowCount * (row / boxRowCount);
    int startCol = boxColumnCount * (col / boxColumnCount);
    for (int i = 0; i < boxRowCount; ++i) {
        for (int j = 0; j < boxColumnCount; ++j) {
            if (matrix.at(startRow + i).at(startCol + j) == value && (startRow + i) != row && (startCol + j) != col) {
                return false;
            }
        }
    }

    return true;
}

int SudokuMatrix::getRowCount() const {
    return rowCount;
}

int SudokuMatrix::getColumnCount() const {
    return columnCount;
}

int SudokuMatrix::getBoxRowCount() const {
    return boxRowCount;
}

int SudokuMatrix::getBoxColumnCount() const {
    return boxColumnCount;
}

std::vector<int> SudokuMatrix::getNums() const {
    return nums;
}

void SudokuMatrix::printToOstreram(std::ostream &ostream) {
    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < columnCount; ++col) {
            if (!invalidPositions.contains(std::pair<int, int>(row, col))) {
                ostream << std::to_string(matrix.at(row).at(col)) << std::string(", ");
            } else {
                ostream << std::to_string(0) << std::string(", ");
            }
            if ((col + 1) % boxColumnCount == 0) {
                ostream << std::string(" ");
            }
        }
        if ((row + 1) % boxRowCount == 0) {
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
        int row = std::uniform_int_distribution<int>(0, rowCount - 1)(rng);
        int col = std::uniform_int_distribution<int>(0, columnCount - 1)(rng);

        // 如果已经是空的位置，则重新选择位置
        while (matrix.at(row).at(col) == 0) {
            row = std::uniform_int_distribution<int>(0, rowCount - 1)(rng);
            col = std::uniform_int_distribution<int>(0, columnCount - 1)(rng);
        }

        setValue(row, col, 0, true);
    }
}

int SudokuMatrix::getValue(const int &row, const int &col) const {
    return matrix.at(row).at(col);
}

int SudokuMatrix::getBoxCount() const {
    return (rowCount / boxRowCount) * (columnCount / boxColumnCount);
}

Sudoku::SudokuMatrix::SudokuMatrixTypes SudokuMatrix::getSudokuType() const {
    return sudokuType_;
}

int SudokuMatrix::getMatrixSize(const SudokuMatrixTypes &type) {
    int size;
    switch (type) {
    case Sudoku::SudokuMatrix::Four_Four:
        size = 4 * 4;
        break;
    case Sudoku::SudokuMatrix::Six_Six:
        size = 6 * 6;
        break;
    case Sudoku::SudokuMatrix::Nine_Nine:
        size = 9 * 9;
        break;
    default:
        break;
    }
    return size;
}

int SudokuMatrix::getMatrixSize() const {
    return rowCount * columnCount;
}

std::unordered_set<std::pair<int, int>, pair_hash> SudokuMatrix::getInvalidPositions() const {
    return invalidPositions;
}

bool SudokuMatrix::isSudokuFilled() const {
    if (filledNum == rowCount * columnCount) {
        return true;
    }
    return false;
}

// 将检查当前所有不合法位置, 若其中的位置合法，则移除
void SudokuMatrix::checkCurrentInvalidPos() {
    std::vector<std::pair<int,int>> vec;
    for (auto &pos : invalidPositions) {
        if (isValidValueForPos(pos.first, pos.second, matrix.at(pos.first).at(pos.second))) {
            vec.emplace_back(pos);
        }
    }
    for (const auto &pos : vec) {
        invalidPositions.erase(pos);
    }
}

int SudokuMatrix::getBoxIndex(const int &row, const int &col) const {
    return (row / boxRowCount) * boxRowCount + (col / boxColumnCount);
}

bool SudokuMatrix::isEmpty() const {
    if (filledNum == 0) return true;
    else return false;
}
} // namespace Sudoku