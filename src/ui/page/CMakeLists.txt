set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)

file(GLOB sources CONFIGURE_DEPENDS *.cpp *.h *.ui)
target_sources(Sudoku-Solo PRIVATE ${sources})
target_include_directories(Sudoku-Solo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})