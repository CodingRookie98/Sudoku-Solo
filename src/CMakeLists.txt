add_subdirectory(ui)
add_subdirectory(sudoku)
add_subdirectory(res)
target_sources(Sudoku-Solo PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)