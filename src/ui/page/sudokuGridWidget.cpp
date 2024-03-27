/**
 ******************************************************************************
 * @file           : wdgetSudokuGrid.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/3
 ******************************************************************************
 */

#include <QErrorMessage>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <sstream>
#include <QJsonDocument>
#include <QJsonObject>
#include "sudokuGridWidget.h"
#include "solverDl.h"
#include "ui_sudokuGridWidget.h"

SudokuGridWidget::SudokuGridWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::SudokuGridWidget) {
    ui->setupUi(this);

    m_gridLayoutBoard = new QGridLayout(this);
    m_boxesGridLayout = nullptr;
    m_buttons = nullptr;
    m_btnGroup = new QButtonGroup(this);
    m_choiceDialog = new NumberChoiceDialog(this);
    m_sudokuMatrixAnswer = nullptr;
    m_sudokuMatrixWorker = nullptr;
    m_generator = new Sudoku::SudokuGenerator();
    m_solverBase = nullptr;
    m_inputSudokuDialog = nullptr;

    this->setLayout(m_gridLayoutBoard);

    init();
    signalsProcess();
}

SudokuGridWidget::~SudokuGridWidget() {
    delete ui;

    deleteButtons();
    deleteBoxes();

    delete m_gridLayoutBoard;
    delete m_btnGroup;
    delete m_choiceDialog;
    m_sudokuMatrixAnswer.reset();
    m_sudokuMatrixWorker.reset();
    delete m_generator;
    delete m_solverBase;
    delete m_inputSudokuDialog;
}

void SudokuGridWidget::init() {
    m_gridLayoutBoard->setAlignment(Qt::AlignCenter);
    m_gridLayoutBoard->setSpacing(10);
    m_btnGroup->setExclusive(true);
}

void SudokuGridWidget::signalsProcess() {
    connect(m_btnGroup, &QButtonGroup::buttonClicked, this, &SudokuGridWidget::handleButtonClicked);
}

void SudokuGridWidget::handleButtonClicked(QAbstractButton *btn) {
    QPoint buttonBottomRight(QCursor::pos().x(), QCursor::pos().y());
    m_choiceDialog->move(buttonBottomRight);
    m_choiceDialog->exec();
    connect(m_choiceDialog, &NumberChoiceDialog::choiceMade, btn, [&](const int &num) {
        auto btn = m_btnGroup->checkedButton();
        if (num == 0) {
            btn->setText("");
        } else {
            btn->setText(QString::number(num));
        }
        auto *btnUnit = qobject_cast<BtnSudokuUnit *>(btn);
        m_sudokuMatrixWorker->setValue(btnUnit->getPosForMatrix().first, btnUnit->getPosForMatrix().second, num, false);
        checkCurrentSudoku();
    });
}

void SudokuGridWidget::generateRandomSudoku(const Sudoku::SudokuMatrix::SudokuMatrixType &sudokuMatrixType) {
    m_sudokuMatrixAnswer.reset();
    m_sudokuMatrixAnswer = std::make_shared<Sudoku::SudokuMatrix>(Sudoku::SudokuMatrix(sudokuMatrixType));
    *m_sudokuMatrixAnswer = m_generator->generate(*m_sudokuMatrixAnswer, Sudoku::SudokuGenerator::GenerateMethod::Recursion);

    m_sudokuMatrixWorker.reset();
    m_sudokuMatrixWorker = std::make_shared<Sudoku::SudokuMatrix>(Sudoku::SudokuMatrix(*m_sudokuMatrixAnswer));
    m_sudokuMatrixWorker->removeNumbers(m_sudokuMatrixWorker->getMatrixSize() - getRandomInitialNumber());

    initBoard();
    updateButtons();
    initChoiceDialog();
}

void SudokuGridWidget::updateButtons() {
    for (int row = 0; row < m_sudokuMatrixWorker->getRowCount(); ++row) {
        for (int col = 0; col < m_sudokuMatrixWorker->getColumnCount(); ++col) {
            auto btn = m_buttons->at(row).at(col);
            auto value = m_sudokuMatrixWorker->getValue(row, col);

            if (value != 0) {
                btn->setText(QString::number(value));
                btn->setEnabled(false);
            }
        }
    }
}

void SudokuGridWidget::deleteButtons() {
    if (m_buttons != nullptr) {
        for (auto &qvec : *m_buttons) {
            for (auto &btn : qvec) {
                delete btn;
            }
        }
    }
    delete m_buttons;
}

void SudokuGridWidget::deleteBoxes() {
    if (m_boxesGridLayout != nullptr) {
        for (QGridLayout *gridLayout : *m_boxesGridLayout) {
            delete gridLayout;
        }
    }
    delete m_boxesGridLayout;
}

// 检查当前数独的合法性
void SudokuGridWidget::checkCurrentSudoku() {
    if (m_sudokuMatrixWorker->isSudokuFilled() && m_sudokuMatrixWorker->getInvalidPositions().empty()) {
        // 游戏结束
        emit sigMessageReadyToRead(Message::IsFilledAndFinished);
    } else if (m_sudokuMatrixWorker->isSudokuFilled() && !m_sudokuMatrixWorker->getInvalidPositions().empty()) {
        // 数独被填满但不正确
        emit sigMessageReadyToRead(Message::IsFilledButNotFinished);
    }
}

void SudokuGridWidget::solveCurrentSudoku() {
    if (m_solverBase == nullptr) {
        m_solverBase = new Sudoku::SolverDL(*m_sudokuMatrixWorker);
    } else {
        delete m_solverBase;
        m_solverBase = new Sudoku::SolverDL(*m_sudokuMatrixWorker);
    }

    if (m_sudokuMatrixAnswer != nullptr && m_sudokuMatrixAnswer->isSudokuFilled()) {
        m_sudokuMatrixWorker.reset();
        m_sudokuMatrixWorker = std::make_shared<Sudoku::SudokuMatrix>(Sudoku::SudokuMatrix(*m_sudokuMatrixAnswer));
        updateButtons();
        emit sigCurrentSudokuIsSolved();
    } else if (m_solverBase->solve()) {
        m_sudokuMatrixAnswer.reset();
        m_sudokuMatrixWorker.reset();
        m_sudokuMatrixAnswer = std::make_shared<Sudoku::SudokuMatrix>(Sudoku::SudokuMatrix(m_solverBase->getAnswer()));
        m_sudokuMatrixWorker = std::make_shared<Sudoku::SudokuMatrix>(Sudoku::SudokuMatrix(m_solverBase->getAnswer()));
        updateButtons();
        emit sigCurrentSudokuIsSolved();
    } else {
        QMessageBox messageBox(this);
        messageBox.setModal(true);
        messageBox.setText(QApplication::translate(metaObject()->className(), tr("当前数独无解").toStdString().c_str()));
        messageBox.exec();
    }
}

void SudokuGridWidget::saveCurrentSudoku() {
    const QByteArray saveJsonTemplateByteArray = QFile::encodeName(":/json/saveTemplate.json");
    const QJsonDocument saveJsonTemplate = QJsonDocument::fromJson(saveJsonTemplateByteArray);
    QJsonDocument *saveJsonDocument = nullptr;

    QFile saveFile("./save/save.json");
    if (!saveFile.open(QIODevice::ReadWrite)) {
        // Todo 日志输出文件打开失败

        saveJsonDocument = new QJsonDocument(saveJsonTemplate);
        QJsonObject jsonObject = saveJsonDocument->object();
    } else {
        saveJsonDocument = new QJsonDocument(QJsonDocument::fromJson(saveFile.encodeName(saveFile.fileName())));
    }
    saveJsonDocument->object("")
}

void SudokuGridWidget::readSudokuFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this);
    QFile file(fileName, this);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Todo 日志输出文件打开失败
        //        qDebug() << "Open file failed";
        return;
    }

    QTextStream textStream(&file);
    QString fileContent = textStream.readAll();
    std::vector<int> nums;
    for (auto &ch : fileContent) {
        if (ch.isNumber()) {
            int num = QString(ch).toInt();
            nums.emplace_back(num);
        }
    }
    file.close();

    Sudoku::SudokuMatrix *matrix = nullptr;
    if (nums.size() == Sudoku::SudokuMatrix::getMatrixSize(Sudoku::SudokuMatrix::SudokuMatrixType::Four_Four)) {
        matrix = new Sudoku::SudokuMatrix(Sudoku::SudokuMatrix::SudokuMatrixType::Four_Four);
    } else if (nums.size() == Sudoku::SudokuMatrix::getMatrixSize(Sudoku::SudokuMatrix::SudokuMatrixType::Six_Six)) {
        matrix = new Sudoku::SudokuMatrix(Sudoku::SudokuMatrix::SudokuMatrixType::Six_Six);
    } else if (nums.size() == Sudoku::SudokuMatrix::getMatrixSize(Sudoku::SudokuMatrix::SudokuMatrixType::Nine_Nine)) {
        matrix = new Sudoku::SudokuMatrix(Sudoku::SudokuMatrix::SudokuMatrixType::Nine_Nine);
    } else {
        return;
    }

    m_sudokuMatrixWorker.reset();
    m_sudokuMatrixWorker = std::make_shared<Sudoku::SudokuMatrix>(*matrix);
    delete matrix;
    if (m_sudokuMatrixWorker == nullptr) {
        // Todo 日志输出文件内容不是合法的数独
        //        qDebug() << "The file content is not a legal Sudoku";
        return;
    }

    for (int row = 0; row < m_sudokuMatrixWorker->getRowCount(); ++row) {
        for (int col = 0; col < m_sudokuMatrixWorker->getColumnCount(); ++col) {
            int value = nums.at(row * m_sudokuMatrixWorker->getColumnCount() + col);
            m_sudokuMatrixWorker->setValue(row, col, value, true);
        }
    }
    if (m_sudokuMatrixAnswer != nullptr) {
        m_sudokuMatrixAnswer.reset();
    }

    initBoard();
    updateButtons();
    initChoiceDialog();

    emit sigShowWidget();

    if (m_sudokuMatrixWorker->isSudokuFilled()) {
        emit sigCurrentSudokuIsSolved();
    }
}

void SudokuGridWidget::initChoiceDialog() {
    if (m_sudokuMatrixWorker == nullptr) {
        // Todo 日志: 初始化选择对话框时sudokuMatrixWorker is nullPointer
        return;
    }
    m_choiceDialog->setChoices(m_sudokuMatrixWorker->getBoxRowCount(), m_sudokuMatrixWorker->getBoxColumnCount(), m_sudokuMatrixWorker->getNums());
}

void SudokuGridWidget::initButtons() {
    if (m_sudokuMatrixWorker != nullptr && m_buttons != nullptr && m_sudokuMatrixWorker->getMatrixSize() == m_buttons->size()) {
        return;
    }

    if (m_buttons != nullptr) {
        for (auto &vecBtns : *m_buttons) {
            for (auto &btn : vecBtns) {
                m_btnGroup->removeButton(btn);
            }
        }
    }

    deleteButtons();

    m_buttons = new std::vector<std::vector<BtnSudokuUnit *>>(m_sudokuMatrixWorker->getRowCount(), std::vector<BtnSudokuUnit *>(m_sudokuMatrixWorker->getColumnCount(), nullptr));
    for (int row = 0; row < m_sudokuMatrixWorker->getRowCount(); ++row) {
        for (int col = 0; col < m_sudokuMatrixWorker->getColumnCount(); ++col) {
            auto *btn = new BtnSudokuUnit(this, row, col, "");
            auto &box = m_boxesGridLayout->at(row / m_sudokuMatrixWorker->getBoxRowCount() * m_sudokuMatrixWorker->getBoxRowCount()
                                              + col / m_sudokuMatrixWorker->getBoxColumnCount());
            btn->setCheckable(true);
            m_buttons->at(row).at(col) = btn;
            m_btnGroup->addButton(btn);
            box->addWidget(btn, row % m_sudokuMatrixWorker->getBoxRowCount(), col % m_sudokuMatrixWorker->getBoxColumnCount(), Qt::AlignCenter);
        }
    }
}

void SudokuGridWidget::initBoxes() {
    if (m_sudokuMatrixWorker != nullptr && m_boxesGridLayout != nullptr && m_sudokuMatrixWorker->getBoxCount() == m_boxesGridLayout->size()) {
        return;
    }
    deleteBoxes();
    m_boxesGridLayout = new std::vector<QGridLayout *>(m_sudokuMatrixWorker->getBoxCount(), nullptr);
    for (int row = 0; row < m_sudokuMatrixWorker->getRowCount() / m_sudokuMatrixWorker->getBoxRowCount(); ++row) {
        for (int col = 0; col < m_sudokuMatrixWorker->getColumnCount() / m_sudokuMatrixWorker->getBoxColumnCount(); ++col) {
            auto *gridLayout = new QGridLayout();

            m_boxesGridLayout->at(row * (m_sudokuMatrixWorker->getColumnCount() / m_sudokuMatrixWorker->getBoxColumnCount()) + col) = gridLayout;
            gridLayout->setSpacing(2);
            m_gridLayoutBoard->addLayout(gridLayout, row, col, Qt::AlignCenter);
        }
    }
}

void SudokuGridWidget::initBoard() {
    initBoxes();
    initButtons();
}

void SudokuGridWidget::hintAnswer() {
    QAbstractButton *checkedBtn = m_btnGroup->checkedButton();
    if (checkedBtn == nullptr) {
        emit sigMessageReadyToRead(Message::PleaseSelectButtonToHint);
    } else {
        if (m_sudokuMatrixAnswer == nullptr || !m_sudokuMatrixAnswer->isSudokuFilled()) {
            solveCurrentSudoku();
        }
        auto *btn = qobject_cast<BtnSudokuUnit *>(checkedBtn);
        auto pos = btn->getPosForMatrix();
        btn->setText(QString::number(m_sudokuMatrixAnswer->getValue(pos.first, pos.second)));
    }
}

void SudokuGridWidget::inputSudoku() {
    if (m_inputSudokuDialog == nullptr) {
        m_inputSudokuDialog = new InputSudokuDialog(this);
    }

    connect(m_inputSudokuDialog, &InputSudokuDialog::sigInputSudokuMatrix, this, [&](const Sudoku::SudokuMatrix &sudokuMatrix) {
        m_sudokuMatrixAnswer.reset();
        m_sudokuMatrixWorker.reset();
        m_sudokuMatrixWorker = std::make_shared<Sudoku::SudokuMatrix>(sudokuMatrix);

        initBoard();
        updateButtons();
        initChoiceDialog();

        m_inputSudokuDialog->close();

        emit sigShowWidget();
    });

    m_inputSudokuDialog->exec();
}

bool SudokuGridWidget::event(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    return QWidget::event(event);
}

int SudokuGridWidget::getRandomInitialNumber() const {
    // 设置随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());

    // 定义范围
    int min, max;
    switch (m_sudokuMatrixWorker->getSudokuType()) {
    case Sudoku::SudokuMatrix::SudokuMatrixType::Four_Four:
        min = 8;
        break;
    case Sudoku::SudokuMatrix::Six_Six:
        min = 12;
        break;
    case Sudoku::SudokuMatrix::Nine_Nine:
        min = 17;
        break;
    }

    max = m_sudokuMatrixWorker->getMatrixSize() - min;
    // 生成随机数
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}
