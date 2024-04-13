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
#include "sudokuMatrixWidget.h"
#include "GameManager.h"
#include "ui_sudokuGridWidget.h"
#include "sudokuGridWidget.h"
#include "mapForQObject.h"
#include "logger.h"

SudokuGridWidget::SudokuGridWidget(QWidget *parent) :
    SudokuMatrixWidget(parent), ui(new Ui::SudokuGridWidget) {
    ui->setupUi(this);

    m_choiceDialog = new NumberChoiceDialog(this);
    m_sudokuMatrixAnswer = nullptr;
    m_sudokuMatrixOriginal = nullptr;
    m_generator = new Sudoku::SudokuGenerator();
    m_lastClickedBtnSudokuUnit = nullptr;

    init();
    signalsProcess();
}

SudokuGridWidget::~SudokuGridWidget() {
    delete ui;

    delete m_choiceDialog;
    delete m_generator;
    m_lastClickedBtnSudokuUnit = nullptr;
}

void SudokuGridWidget::init() {
    // 将实例注册到MapForQObject, 方便跨实例操作，不用通过单例和中间对象
    MapForQObject::getInstance()->registerObject(TypeName<SudokuGridWidget>::get(), this);
}

void SudokuGridWidget::signalsProcess() {
    connect(this, &SudokuMatrixWidget::sigUnitClicked, this, &SudokuGridWidget::handleButtonClicked);
    connect(this, &SudokuMatrixWidget::sigIsFilled, this, [&] {
        if (this->isSolved()) {
            emit sigMessageReadyToRead(Message::IsFilledAndFinished);
        } else {
            emit sigMessageReadyToRead(Message::IsFilledButNotFinished);
        }
    });
    choiceDialogSignalsProcess();
}

void SudokuGridWidget::handleButtonClicked(BtnSudokuUnit *btn) {
    QPoint buttonBottomRight(QCursor::pos().x(), QCursor::pos().y());
    m_choiceDialog->move(buttonBottomRight);
    m_choiceDialog->show();
    m_lastClickedBtnSudokuUnit = btn;
}

void SudokuGridWidget::generateRandomSudoku(const Sudoku::SudokuMatrix::SudokuMatrixType &sudokuMatrixType) {
    m_sudokuMatrixAnswer.reset();
    m_sudokuMatrixAnswer = std::make_shared<Sudoku::SudokuMatrix>(Sudoku::SudokuMatrix(sudokuMatrixType));
    *m_sudokuMatrixAnswer = m_generator->generate(*m_sudokuMatrixAnswer, Sudoku::SudokuGenerator::GenerateMethod::Recursion);

    Sudoku::SudokuMatrix sudokuMatrixWorker = *m_sudokuMatrixAnswer;
    sudokuMatrixWorker.removeNumbers(sudokuMatrixWorker.getMatrixSize() - getRandomInitialNumber());

    this->buildMatrix(sudokuMatrixWorker);
    for (int i = 0; i < sudokuMatrixWorker.getRowCount(); ++i) {
        for (int j = 0; j < sudokuMatrixWorker.getRowCount(); ++j) {
            if (sudokuMatrixWorker.getValue(i, j) != 0) {
                if (!this->setButtonEnable(i, j, false)) {
                    // log: 设置button状态失败
                    Logger::getInstance()->log(Logger::Error, QString(__FUNCTION__) + " "
                                                                  + QString::number(__LINE__) + " "
                                                                  + "Failed to set button state");
                }
            }
        }
    }

    m_sudokuMatrixOriginal.reset();
    m_sudokuMatrixOriginal = std::make_shared<Sudoku::SudokuMatrix>(Sudoku::SudokuMatrix(sudokuMatrixWorker));

    initChoiceDialog();
}

void SudokuGridWidget::saveCurrentSudoku(const int &timeSpent) {
    GameManager::getInstance()->save(*m_sudokuMatrixOriginal, *m_sudokuMatrixAnswer, this->getSudokuMatrix(), timeSpent);
}

void SudokuGridWidget::initChoiceDialog() {
    if (m_sudokuMatrixOriginal == nullptr) {
        // 日志: 初始化选择对话框时sudokuMatrixWorker is nullPointer
        Logger::getInstance()->log(Logger::Error, QString(__FUNCTION__) + " "
                                                      + QString::number(__LINE__) + " "
                                                      + "sudokuMatrixWorker is nullPointer when initializing selection dialog");
        return;
    }
    m_choiceDialog->setChoices(m_sudokuMatrixOriginal->getBoxRowCount(), m_sudokuMatrixOriginal->getBoxColumnCount(), m_sudokuMatrixOriginal->getNums());
}

void SudokuGridWidget::hintAnswer() {
    auto checkedBtn = this->getCheckedButton();
    if (checkedBtn == nullptr) {
        emit sigMessageReadyToRead(Message::PleaseSelectButtonToHint);
    } else {
        auto pos = checkedBtn->getPosForMatrix();
        this->setMatrixValue(pos.first, pos.second, m_sudokuMatrixAnswer->getValue(pos.first, pos.second));
    }
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
    switch (m_sudokuMatrixAnswer->getSudokuType()) {
    case Sudoku::SudokuMatrix::SudokuMatrixType::FourFour:
        min = 8;
        break;
    case Sudoku::SudokuMatrix::SixSix:
        min = 12;
        break;
    case Sudoku::SudokuMatrix::NineNine:
        min = 17;
        break;
    }

    max = m_sudokuMatrixAnswer->getMatrixSize() - min;
    // 生成随机数
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

void SudokuGridWidget::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key::Key_Escape) {
        m_choiceDialog->close();
    }
    QWidget::keyReleaseEvent(event);
}

void SudokuGridWidget::setGameMatrix(std::shared_ptr<Sudoku::SudokuMatrix> answer,
                                     std::shared_ptr<Sudoku::SudokuMatrix> original,
                                     std::shared_ptr<Sudoku::SudokuMatrix> work) {
    m_sudokuMatrixAnswer = answer;
    m_sudokuMatrixOriginal = original;
    this->buildMatrix(*work);

    initChoiceDialog();
}

void SudokuGridWidget::choiceDialogSignalsProcess() {
    connect(m_choiceDialog, &NumberChoiceDialog::choiceMade, this, [&](const int &num) {
        if (num == 0) {
            m_lastClickedBtnSudokuUnit->setText("");
        } else {
            m_lastClickedBtnSudokuUnit->setText(QString::number(num));
        }
        this->setMatrixValue(m_lastClickedBtnSudokuUnit->getPosForMatrix().first, m_lastClickedBtnSudokuUnit->getPosForMatrix().second, num);
    });
}
