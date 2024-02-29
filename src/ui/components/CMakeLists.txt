set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)

target_sources(Sudoku-Solo PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/btnSudokuUnit.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/btnSudokuUnit.h
        ${CMAKE_CURRENT_SOURCE_DIR}/btnSudokuUnit.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gameOptionWidget.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/gameOptionWidget.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gameOptionWidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gamingWidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gamingWidget.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gamingWidget.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/numberChoiceDialog.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/numberChoiceDialog.h
        ${CMAKE_CURRENT_SOURCE_DIR}/numberChoiceDialog.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/sudokuGridWidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sudokuGridWidget.h
        ${CMAKE_CURRENT_SOURCE_DIR}/sudokuGridWidget.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/inputSudokuDialog.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/inputSudokuDialog.h
        ${CMAKE_CURRENT_SOURCE_DIR}/inputSudokuDialog.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/aboutDialog.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aboutDialog.h
        ${CMAKE_CURRENT_SOURCE_DIR}/aboutDialog.ui
)
target_include_directories(Sudoku-Solo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})