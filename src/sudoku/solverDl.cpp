/**
 ******************************************************************************
 * @file           : solverDl.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/25
 ******************************************************************************
 */

#include "solverDl.h"

namespace Sudoku {
SolverDL::SolverDL(const SudokuMatrix &sudokuMatrix) {
    dancingLinks_ = new DancingLinks();
    sudokuMatrix_ = new SudokuMatrix(sudokuMatrix);
    exactCoverMatrix_ = new std::vector<std::vector<unsigned int>>(sudokuMatrix_->getMatrixSize() * sudokuMatrix_->getNums().size(),
                                                                   std::vector<unsigned int>(sudokuMatrix_->getMatrixSize() * 4, 0));
    constraints_ = new std::array<int, 4>();
    buildExactCoverMatrix();
    dancingLinks_->buildDancingLinks(*exactCoverMatrix_);
}

SolverDL::~SolverDL() {
    delete dancingLinks_;
    delete sudokuMatrix_;
    delete exactCoverMatrix_;
    delete constraints_;
}

bool SolverDL::solve() {
    if (isSolved)
        return true;
    else {
        if (dancingLinks_->solve()) {
            isSolved = true;
            return true;
        } else
            return false;
    }
}

SudokuMatrix SolverDL::getAnswer() {
    if (solve()) {
        transformExactCoverAns();
    }
    return *sudokuMatrix_;
}

void SolverDL::buildExactCoverMatrix() {
    // 将非法位置的值全部置0
    if (!sudokuMatrix_->getInvalidPositions().empty()) {
        auto invalidPos = sudokuMatrix_->getInvalidPositions();
        for (const auto &pos : invalidPos) {
            sudokuMatrix_->setValue(pos.first, pos.second, 0);
        }
    }

    for (int row = 0; row < sudokuMatrix_->getRowCount(); ++row) {
        for (int col = 0; col < sudokuMatrix_->getColumnCount(); ++col) {
            int value = sudokuMatrix_->getValue(row, col);
            if (value != 0) {
                coverCell(row, col, value);
            } else {
                for (auto &num : sudokuMatrix_->getNums()) {
                    coverCell(row, col, num);
                }
            }
        }
    }
}

void SolverDL::computeConstraints(const int &row, const int &col, const int &value) const {
    int v = value - 1;
    int boxIdx = sudokuMatrix_->getBoxIndex(row, col);

    constraints_->at(0) = row * sudokuMatrix_->getColumnCount() + col;                                              // 约束：单元格唯一性
    constraints_->at(1) = (int)(sudokuMatrix_->getMatrixSize() + row * sudokuMatrix_->getNums().size() + v);        // 约束：行唯一性
    constraints_->at(2) = (int)(sudokuMatrix_->getMatrixSize() * 2 + col * sudokuMatrix_->getNums().size() + v);    // 约束：列唯一性
    constraints_->at(3) = (int)(sudokuMatrix_->getMatrixSize() * 3 + boxIdx * sudokuMatrix_->getNums().size() + v); // 约束：宫唯一性
    //    std::cout << __FUNCTION__ << __LINE__ << "\n";
}

void SolverDL::coverCell(const int &row, const int &col, const int &value) const {
    int rowIdx = (int)(row * sudokuMatrix_->getColumnCount() * sudokuMatrix_->getNums().size()
                       + col * sudokuMatrix_->getNums().size() + value - 1);
    computeConstraints(row, col, value);
    for (const auto &idx : *constraints_) {
        exactCoverMatrix_->at(rowIdx).at(idx) = 1;
    }
}

void SolverDL::transformExactCoverAns() {
    for (const auto &rowIdx : dancingLinks_->getAnswer()) {
        int value = (int)(rowIdx % sudokuMatrix_->getNums().size() + 1);
        int row = (int)(rowIdx / sudokuMatrix_->getNums().size() / sudokuMatrix_->getColumnCount());
        int col = (int)(rowIdx / sudokuMatrix_->getNums().size() % sudokuMatrix_->getColumnCount());
        if (sudokuMatrix_->getValue(row, col) == 0) {
            sudokuMatrix_->setValue(row, col, value);
        }
    }
}
} // namespace Sudoku