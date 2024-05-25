cmake_minimum_required(VERSION 3.5)

project(Sudoku-Solo VERSION 0.3.0 LANGUAGES CXX)

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
)
find_package(spdlog CONFIG REQUIRED)
find_package(CURL REQUIRED)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
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


target_link_libraries(Sudoku-Solo PRIVATE Sudoku)
target_link_libraries(Sudoku-Solo PRIVATE
        Qt6::Core
        Qt6::Multimedia
)
target_link_libraries(Sudoku-Solo PRIVATE
        spdlog::spdlog
        CURL::libcurl
)

add_subdirectory(src)
add_subdirectory(translations)
add_subdirectory(third_party)

if (WIN32)
    # link alibabacloud-oss-cpp-sdk
    target_link_libraries(Sudoku-Solo PRIVATE ${CMAKE_BINARY_DIR}/third_party/install/aliyun-oss-cpp-sdk/lib/alibabacloud-oss-cpp-sdk.lib)

    # link QCefView
    set_target_properties(QCefView PROPERTIES IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/third_party/QCefView-sdk-build/src/${CMAKE_BUILD_TYPE}/QCefView.lib)
    target_link_libraries(Sudoku-Solo PRIVATE QCefView)
elseif (UNIX)
    target_link_libraries(Sudoku-Solo PRIVATE ${CMAKE_BINARY_DIR}/third_party/install/aliyun-oss-cpp-sdk/lib/alibabacloud-oss-cpp-sdk.so)
endif ()

set_target_properties(Sudoku-Solo PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Sudoku-Solo)
endif ()
