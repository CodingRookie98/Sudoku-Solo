target_sources(Sudoku-Solo PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/mainWindow.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/mainWindow.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mainWindow.ui
)
target_include_directories(Sudoku-Solo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(components)