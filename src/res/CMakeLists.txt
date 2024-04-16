target_sources(Sudoku-Solo PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/res.qrc
        ${CMAKE_CURRENT_SOURCE_DIR}/logo.rc
)

# 获取目标的 CMAKE_RUNTIME_OUTPUT_DIRECTORY 属性
get_target_property(RUNTIME_OUTPUT_DIR Sudoku-Solo RUNTIME_OUTPUT_DIRECTORY)
# 设置源目录和目标目录
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/background)
set(TARGET_DIR ${RUNTIME_OUTPUT_DIR}/background)
## 添加一个自定义目标
add_custom_target(copy_background_files
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_DIR} ${TARGET_DIR}
        COMMENT "Copying background files"
)