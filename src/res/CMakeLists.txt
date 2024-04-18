target_sources(Sudoku-Solo PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/res.qrc
        ${CMAKE_CURRENT_SOURCE_DIR}/logo.rc
)

# 获取目标的 CMAKE_RUNTIME_OUTPUT_DIRECTORY 属性
get_target_property(RUNTIME_OUTPUT_DIR Sudoku-Solo RUNTIME_OUTPUT_DIRECTORY)
# 设置源目录和目标目录
set(BACKGROUND_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/background)
set(BACKGROUND_TARGET_DIR ${RUNTIME_OUTPUT_DIR}/background)
set(MUSIC_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/music)
set(MUSIC_TARGET_DIR ${RUNTIME_OUTPUT_DIR}/music)
## 添加一个自定义目标
add_custom_target(copy_resource_files
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${BACKGROUND_SOURCE_DIR} ${BACKGROUND_TARGET_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${MUSIC_SOURCE_DIR} ${MUSIC_TARGET_DIR}
        COMMENT "Copying Resource Files"
)