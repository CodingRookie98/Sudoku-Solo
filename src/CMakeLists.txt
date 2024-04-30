add_subdirectory(backend)
add_subdirectory(res)
add_subdirectory(sudoku)
add_subdirectory(ui)
target_sources(Sudoku-Solo PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gameApplication.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gameApplication.cpp
)