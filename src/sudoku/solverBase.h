/**
 ******************************************************************************
 * @file           : solverBase.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/25
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_SUDOKU_SOLVERBASE_H_
#define SUDOKU_SOLO_SRC_SUDOKU_SOLVERBASE_H_

#include "sudokuMatrix.h"

namespace Sudoku {

class SolverBase {
public:
    SolverBase();
    virtual ~SolverBase();

    virtual bool solve() = 0;
    virtual SudokuMatrix getAnswer() = 0;
private:
};

} // namespace Sudoku

#endif // SUDOKU_SOLO_SRC_SUDOKU_SOLVERBASE_H_
