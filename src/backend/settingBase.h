/**
 ******************************************************************************
 * @file           : settingBase.h
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 2024/4/13
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_BACKEND_SETTINGBASE_H_
#define SUDOKU_SOLO_SRC_BACKEND_SETTINGBASE_H_

class SettingBase {
public:
    virtual void saveSettings() = 0;
    virtual ~SettingBase() = default;
};

#endif // SUDOKU_SOLO_SRC_BACKEND_SETTINGBASE_H_
