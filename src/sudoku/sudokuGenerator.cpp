/**
 ******************************************************************************
 * @file           : sudokuGenerator.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/5
 ******************************************************************************
 */

#include "sudokuGenerator.h"

namespace Sudoku {

Sudoku::SudokuGenerator::SudokuGenerator() : rng(std::random_device{}()) {
    matrix_ = nullptr;
}

SudokuMatrix SudokuGenerator::generate(const SudokuMatrix &_matrix, const GenerateMethod &_algorithm) {
    if (matrix_ != nullptr) {
        delete matrix_;
    }
    
    matrix_    = new SudokuMatrix(_matrix);
    algorithm_ = _algorithm;
    nums = matrix_->getNums();
    
    switch (algorithm_) {
    case GenerateMethod::Recursion:
        methodRecursion(0, 0);
        break;
    default:
        break;
    }
    return *matrix_;
}

SudokuGenerator::~SudokuGenerator() {
    matrix_ = nullptr;
}

bool SudokuGenerator::methodRecursion(int row, int col) {
    if (row == matrix_->getRowCount()) {
        row = 0;
        if (++col == matrix_->getColumnCount()) {
            return true; // 解决方案找到
        }
    }

    // 生成随机顺序的数字
    std::shuffle(nums.begin(), nums.end(), rng);

    for (int num : nums) {
        bool valid = matrix_->isValidValueForPos(row, col, num);
        if (valid) {
            matrix_->setValue(row, col, num);
            if (methodRecursion(row + 1, col)) {
                return true;
            }
        }
    }

    matrix_->setValue(row, col, 0);
    return false;
}
}// namespace Sudoku
