/**
 ******************************************************************************
 * @file           : sudokuMatrix.h
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/6
 ******************************************************************************
 */

#ifndef SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SUDOKU_SUDOKUMATRIX_H_
#define SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SUDOKU_SUDOKUMATRIX_H_

#include <vector>
#include <unordered_set>
#include <random>

namespace Sudoku {

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const {
        // 这里只是一个示例，你可以根据实际情况修改哈希计算方式
        return std::hash<T1>()(p.first) ^ std::hash<T2>()(p.second);
    }
};

class SudokuMatrix {
public:
    enum SudokuMatrixType {
        FourFour,
        SixSix,
        NineNine
    };
    explicit SudokuMatrix(const SudokuMatrix::SudokuMatrixType &sudokuType);
    
    ~SudokuMatrix();
    Sudoku::SudokuMatrix::SudokuMatrixType getSudokuType() const;
    bool setValue(const int &row, const int &col, const int &value, const bool &checkIsValid = true);
    int getValue(const int &row, const int &col) const;
    bool isValidValueForPos(const int &row, const int &col, const int &value) const;
    std::unordered_set<std::pair<int, int>, pair_hash> getInvalidPositions() const;
    int getRowCount() const;
    int getColumnCount() const;
    int getBoxRowCount() const;
    int getBoxColumnCount() const;
    int getBoxCount() const;
    std::vector<int> getNums() const;
    static int getMatrixSize(const Sudoku::SudokuMatrix::SudokuMatrixType & type);
    int getMatrixSize() const;
    void printToOstream(std::ostream &ostream);
    void removeNumbers(const int& removeNumCount);
    bool isSudokuFilled() const;
    int getBoxIndex(const int& row, const int& col) const;
    bool isEmpty() const;

private:
    std::vector<std::vector<int>> m_matrix;
    std::unordered_set<std::pair<int, int>, pair_hash> m_invalidPositions;
    std::vector<int> m_nums;
    int m_rowCount;
    int m_columnCount;
    int m_boxRowCount;
    int m_boxColumnCount;
    int m_numSize;
    int m_filledNum = 0;
    Sudoku::SudokuMatrix::SudokuMatrixType m_sudokuType;
    
    void checkCurrentInvalidPos();
};



} // namespace Sudoku

#endif // SUDOKUMASTER_SUDOKUMASTER_CLIENT_SRC_SUDOKU_SUDOKUMATRIX_H_
