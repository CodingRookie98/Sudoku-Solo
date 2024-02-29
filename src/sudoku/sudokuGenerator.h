/**
 ******************************************************************************
 * @file           : sudokuGenerator.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/5
 ******************************************************************************
 */

#ifndef SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SUDOKU_SUDOKUGENERATOR_H_
#define SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SUDOKU_SUDOKUGENERATOR_H_

#include <algorithm>
#include <random>
#include <vector>
#include <stdexcept>
#include <format>
#include "sudokuMatrix.h"

namespace Sudoku {

class SudokuGenerator {
public:
    enum GenerateMethod {
        Recursion
    };
    explicit SudokuGenerator();
    virtual ~SudokuGenerator();

    SudokuMatrix generate(const SudokuMatrix& _matrix, const GenerateMethod & _algorithm);

private:
    SudokuMatrix* matrix_;
    std::vector<int> nums;
    SudokuGenerator::GenerateMethod algorithm_;
    std::mt19937 rng;

    bool methodRecursion(int row, int col);
};

} // namespace Sudoku
#endif // SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SUDOKU_SUDOKUGENERATOR_H_
