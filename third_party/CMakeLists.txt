include(ExternalProject)
include(FetchContent)

ExternalProject_Add(
        aliyun-oss-cpp-sdk
        GIT_REPOSITORY https://github.com/aliyun/aliyun-oss-cpp-sdk.git
        GIT_TAG master
        PREFIX ${CMAKE_BINARY_DIR}/third_party/aliyun-oss-cpp-sdk
        CMAKE_ARGS -DBUILD_SHARED_LIBS=ON -DBUILD_SAMPLE=OFF -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/third_party/install/aliyun-oss-cpp-sdk -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_RUNTIME_OUTPUT_DIRECTORY} -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=ON -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        DOWNLOAD_EXTRACT_TIMESTAMP true
        BUILD_ALWAYS true
        BUILD_IN_SOURCE false
        BINARY_DIR ${CMAKE_BINARY_DIR}/third_party/aliyun-oss-cpp-sdk-build
)

target_include_directories(Sudoku-Solo PRIVATE ${CMAKE_BINARY_DIR}/third_party/install/aliyun-oss-cpp-sdk/include)
add_dependencies(Sudoku-Solo aliyun-oss-cpp-sdk)

FetchContent_Declare(
        QCefView-sdk
        GIT_REPOSITORY https://github.com/CefView/QCefView.git
        GIT_TAG main
        SOURCE_DIR ${CMAKE_BINARY_DIR}/third_party/QCefView-sdk
        BINARY_DIR ${CMAKE_BINARY_DIR}/third_party/QCefView-sdk-build
)

FetchContent_MakeAvailable(QCefView-sdk)
target_include_directories(Sudoku-Solo PRIVATE ${CMAKE_BINARY_DIR}/third_party/QCefView-sdk/include)
add_dependencies(Sudoku-Solo QCefView)