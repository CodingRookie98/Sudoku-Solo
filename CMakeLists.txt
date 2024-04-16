cmake_minimum_required(VERSION 3.5)

project(Sudoku-Solo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS
        Core
        Multimedia
        WebEngineWidgets
        Multimedia
)
find_package(spdlog CONFIG REQUIRED)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Sudoku-Solo
            MANUAL_FINALIZATION
    )
else ()
    add_executable(Sudoku-Solo)
endif ()

target_link_libraries(Sudoku-Solo PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
)

add_library(Sudoku SHARED)

target_link_libraries(Sudoku-Solo PRIVATE Sudoku)
target_link_libraries(Sudoku-Solo PRIVATE
        Qt6::Core
        Qt6::Multimedia
        Qt6::WebEngineWidgets
        Qt6::Multimedia
)
target_link_libraries(Sudoku-Solo PRIVATE spdlog::spdlog)

add_subdirectory(src)
add_subdirectory(translations)

set_target_properties(Sudoku-Solo PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

install(TARGETS Sudoku-Solo
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Sudoku-Solo)
endif ()
