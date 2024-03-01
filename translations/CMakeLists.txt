find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS LinguistTools)
qt_add_translations(Sudoku-Solo TS_FILES en_US.ts zh_CN.ts
        RESOURCE_PREFIX "/language"
)
