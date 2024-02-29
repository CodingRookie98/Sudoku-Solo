/**
 ******************************************************************************
 * @file           : dancingLinks.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/1/11
 ******************************************************************************
 */

#include "dancingLinks.h"

void DancingLinks::createColumnHeaders(const unsigned int size) {
    init();

    auto *newNode   = new DLNode(-1, nullptr);
    DLNode *preNode = newNode;
    newNode->left = newNode->right = root;
    newNode->down = newNode->up = newNode;
    root->left                  = newNode;
    root->right                 = newNode;
    columnHeaders.push_back(root->right);

    for (int i = 0; i < size - 1; ++i) {
        newNode        = new DLNode(-1, nullptr);
        newNode->left  = root;
        newNode->right = preNode;
        root->right    = newNode;
        preNode->left  = newNode;
        newNode->up    = newNode;
        newNode->down  = newNode;
        columnHeaders.push_back(newNode);
        preNode = newNode;
    }
}

void DancingLinks::appendRow(const vector<unsigned int> &rowIndices, const unsigned int rowIdx) {
    DLNode *prev      = nullptr;
    DLNode *firstNode = nullptr;

    for (int index : rowIndices) {
        DLNode *colHeader = columnHeaders[index];
        auto *newNode     = new DLNode((int)rowIdx, colHeader);

        if (prev == nullptr) {
            prev      = newNode;
            firstNode = newNode;
        } else {
            prev->right   = newNode;
            newNode->left = prev;
            prev          = newNode;
        }

        DLNode *headerTop = colHeader->up;
        newNode->up       = headerTop;
        headerTop->down   = newNode;
        colHeader->up     = newNode;
        newNode->down     = colHeader;
        colHeader->colSize++;
    }

    firstNode->left = prev;
    prev->right     = firstNode;
}

void DancingLinks::coverColumn(DLNode *colHeader) {
    colHeader->right->left = colHeader->left;
    colHeader->left->right = colHeader->right;

    for (DLNode *downNode = colHeader->down; downNode != colHeader; downNode = downNode->down) {
        for (DLNode *rightNode = downNode->right; rightNode != downNode; rightNode = rightNode->right) {
            rightNode->down->up = rightNode->up;
            rightNode->up->down = rightNode->down;
            rightNode->colHeader->colSize--;
        }
    }
}

void DancingLinks::unCoverColumn(DLNode *colHeader) {
    for (DLNode *downNode = colHeader->down; downNode != colHeader; downNode = downNode->down) {
        for (DLNode *rightNode = downNode->right; rightNode != downNode; rightNode = rightNode->right) {
            rightNode->down->up = rightNode;
            rightNode->up->down = rightNode;
            rightNode->colHeader->colSize++;
        }
    }

    colHeader->right->left = colHeader;
    colHeader->left->right = colHeader;
}

DLNode *DancingLinks::getMinSizeColHeader() {
    DLNode *minColHeader = nullptr;
    int min              = INT_MAX;
    for (DLNode *col = root->right; col != root; col = col->right) {
        if (min > col->colSize) {
            min          = col->colSize;
            minColHeader = col;
        }
    }
    return minColHeader;
}

bool DancingLinks::solve() {
    if (root->right == root) return true;

    DLNode *minSizeColHeader = getMinSizeColHeader();
    coverColumn(minSizeColHeader);

    for (DLNode *rowNode = minSizeColHeader->down; rowNode != minSizeColHeader; rowNode = rowNode->down) {
        ans.push_back(rowNode->row);
        for (DLNode *rightNode = rowNode->right; rightNode != rowNode; rightNode = rightNode->right) {
            coverColumn(rightNode->colHeader);
        }
        if (solve()) return true;
        ans.pop_back();
        for (DLNode *uncoverNode = rowNode->right; uncoverNode != rowNode; uncoverNode = uncoverNode->right) {
            unCoverColumn(uncoverNode->colHeader);
        }
    }
    unCoverColumn(minSizeColHeader);
    return false;
}

void DancingLinks::deleteEmptyColumn() {
    for (DLNode *col = root->right; col != root; col = col->right) {
        if (col->colSize == 0) {
            DLNode *newNode = col->down;
            while (newNode != col) {
                newNode->right->left = newNode->left;
                newNode->left->right = newNode->right;
                newNode              = newNode->down;
            }
            col->left->right = col->right;
            col->right->left = col->left;
        }
    }
}

void DancingLinks::buildDancingLinks(const vector<std::vector<unsigned int>> &matrix) {
    int cols = matrix[0].size();
    this->createColumnHeaders(cols);

    for (int i = 0; i < matrix.size(); ++i) {
        std::vector<unsigned int> indices;
        for (int j = 0; j < cols; ++j) {
            if (matrix[i][j] == 1) {
                indices.push_back(j);
            }
        }
        if (!indices.empty()) {
            this->appendRow(indices, i);
        }
    }
    deleteEmptyColumn();
}

bool DancingLinks::nodeIsExist(const unsigned int &rowIdx, const unsigned int &colIdx) {
    DLNode *colHeader = columnHeaders.at(colIdx);
    for (DLNode *node = colHeader->down; node != colHeader; node = node->down) {
        if (node->row == rowIdx) {
            return true;
        }
    }
    return false;
}
DancingLinks::DancingLinks() :
    root(new DLNode(-1, nullptr)) {
}

vector<int> DancingLinks::getAnswer() const {
    return ans;
}

void DancingLinks::init() {
    for (auto &colNode = root->left; colNode != root; colNode = colNode->left) {
        for (auto &rowNode = colNode->down; rowNode != colNode; rowNode = colNode->down) {
            deleteNode(rowNode);
        }
        deleteNode(colNode);
    }
    root->right = nullptr;
    columnHeaders.clear();
}

void DancingLinks::deleteNode(DLNode *node) {
    node->right = node->left = node->up = node->down = nullptr;
    delete node;
}

DancingLinks::~DancingLinks() {
    for (auto &colNode : columnHeaders) {
        DLNode *downNode = colNode->down;
        while (downNode != colNode) {
            auto tmpNode = downNode->down;
            deleteNode(downNode);
            downNode = tmpNode;
        }
        deleteNode(colNode);
    }
    delete root;
}
