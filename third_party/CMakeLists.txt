include(ExternalProject)

ExternalProject_Add(
        aliyun-oss-cpp-sdk
        URL https://github.com/aliyun/aliyun-oss-cpp-sdk/archive/refs/tags/1.10.0.zip
        PREFIX ${CMAKE_BINARY_DIR}/third_party/aliyun-oss-cpp-sdk
        CMAKE_ARGS -DBUILD_SHARED_LIBS=ON -DBUILD_SAMPLE=OFF -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/third_party/install/aliyun-oss-cpp-sdk -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_RUNTIME_OUTPUT_DIRECTORY} -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=ON
        DOWNLOAD_EXTRACT_TIMESTAMP true
        BUILD_ALWAYS true
)
target_include_directories(Sudoku-Solo PRIVATE ${CMAKE_BINARY_DIR}/third_party/install/aliyun-oss-cpp-sdk/include)
