/**
 ******************************************************************************
 * @file           : temp.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/1/10
 ******************************************************************************
 */

#ifndef SUDOKUMASTER_DANCINGLINKS_H
#define SUDOKUMASTER_DANCINGLINKS_H

#include <iostream>
#include <vector>

using namespace std;

// 舞蹈链节点类
struct DLNode {
    DLNode *left, *right, *up, *down;
    DLNode* colHeader; // 列头指针
    int row;
    unsigned int colSize = 0;

    DLNode(int r, DLNode* _colHeader) :
        left(this), right(this), up(this), down(this), colHeader(_colHeader), row(r) {
    }
//    ~DLNode(){
//        left = right = up = down = colHeader = nullptr;
//    }
    
};

// 舞蹈链矩阵类
class DancingLinks {
public:
    DancingLinks();
    ~DancingLinks();
    bool solve();
    bool nodeIsExist(const unsigned int& rowIdx, const unsigned int& colIdx);
    void buildDancingLinks(const std::vector<std::vector<unsigned int>>& matrix);
    void appendRow(const std::vector<unsigned int>& rowIndices, unsigned int rowIdx);
    vector<int> getAnswer() const;
    
private:
    vector<DLNode*> columnHeaders; // 列头数组
    DLNode* root;                  // 舞蹈链根节点
    vector<int> ans;
    
    void init();
    void createColumnHeaders(unsigned int size);
    void deleteEmptyColumn();
    DLNode* getMinSizeColHeader();
    void coverColumn(DLNode* colHeader);
    static void unCoverColumn(DLNode* colHeader);
    static void deleteNode(DLNode* node);
};

#endif