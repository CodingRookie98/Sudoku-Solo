target_sources(Sudoku PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/dancingLinks.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dancingLinks.h
        ${CMAKE_CURRENT_SOURCE_DIR}/sudokuGenerator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sudokuGenerator.h
        ${CMAKE_CURRENT_SOURCE_DIR}/sudokuMatrix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sudokuMatrix.h
        ${CMAKE_CURRENT_SOURCE_DIR}/solverBase.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/solverBase.h
        ${CMAKE_CURRENT_SOURCE_DIR}/solverDl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/solverDl.h
)
target_include_directories(Sudoku-Solo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
