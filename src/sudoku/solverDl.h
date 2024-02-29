/**
 ******************************************************************************
 * @file           : solverDl.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/25
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_SUDOKU_SOLVERDL_H_
#define SUDOKU_SOLO_SRC_SUDOKU_SOLVERDL_H_

#include <array>
#include "solverBase.h"
#include "dancingLinks.h"
#include "sudokuMatrix.h"

namespace Sudoku {

class SolverDL : public SolverBase{
public:
    explicit SolverDL(const SudokuMatrix &sudokuMatrix);
    ~SolverDL();
    bool solve() override;
    SudokuMatrix getAnswer() override;
private:
    DancingLinks *dancingLinks_;
    SudokuMatrix *sudokuMatrix_;
    std::vector<std::vector<unsigned int>> *exactCoverMatrix_;
    std::array<int, 4> *constraints_;
    bool isSolved = false;
    
    void buildExactCoverMatrix();
    void computeConstraints(const int& row, const int& col, const int& value) const;
    void coverCell(const int& row, const int& col, const int& value) const;
    void transformExactCoverAns();
};

} // namespace Sudoku

#endif // SUDOKU_SOLO_SRC_SUDOKU_SOLVERDL_H_
