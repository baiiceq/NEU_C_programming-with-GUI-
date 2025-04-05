#pragma once
#include <Windows.h>
#include <commctrl.h>
#include "account.h"
#include "resource_manager.h"
#include "configs.h"

// 系统维护相关的控件ID定义
#define ID_BTN_PASSWORD_MAINTAIN     1001
#define ID_BTN_RESET_PASSWORD        1002
#define ID_BTN_DATA_BACKUP           1003
#define ID_BTN_DATA_RESTORE          1004
#define ID_BTN_BACK                  1005
#define ID_BTN_CHANGE_USERNAME	     1006
#define ID_BTN_CHANGE_USER_USERNAME  1007

// 密码维护界面控件ID
#define IDC_OLD_PASSWORD             1101
#define IDC_NEW_PASSWORD             1102
#define IDC_CONFIRM_PASSWORD         1103
#define IDC_BTN_CONFIRM              1104
#define IDC_BTN_CANCEL               1105

// 重置用户密码界面控件ID
#define IDC_USER_LISTVIEW            1201
#define IDC_BTN_RESET                1202

//修改用户名界面控件ID
#define IDC_NEW_USERNAME             1301

//修改其他用户用户名界面控件ID
#define IDC_BTN_CHANGE_USERNAME     1401
//数据恢复窗口ID
#define IDC_RESTORE_TIME                1402

#define ID_BTN_STATISTICS 1010
#define IDC_RADIO_STATS_EQUIPMENT 1011
#define IDC_RADIO_STATS_ACCOUNT 1012
#define IDC_CATEGORY_COMBO 1013
#define IDC_LABROOM_COMBO 1014
#define IDC_CHECK_BY_CATEGORY 1015
#define IDC_CHECK_BY_LABROOM 1016
#define IDC_CHECK_BY_PRICE 1017
#define IDC_CHECK_BY_DATE 1018
#define IDC_CHECK_BY_ACCOUNT_TYPE 1019
#define IDC_CHECK_BY_ACCOUNT_LABROOM 1020
#define IDC_EDIT_MIN_PRICE 1021
#define IDC_EDIT_MAX_PRICE 1022
#define IDC_EDIT_START_DATE 1023
#define IDC_EDIT_END_DATE 1024
#define IDC_ACCOUNT_TYPE_COMBO 1025
#define IDC_ACCOUNT_LABROOM_COMBO 1026
#define IDC_BTN_STATS_EXECUTE 1027
#define IDC_BTN_STATS_BACK 1028
#define IDC_STATS_RESULT_LIST 1029

// 系统维护窗口过程函数
LRESULT CALLBACK SystemMaintenanceWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 密码维护窗口过程函数
LRESULT CALLBACK PasswordMaintenanceWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 重置用户密码窗口过程函数
LRESULT CALLBACK ResetPasswordWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChangeUsernameWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChangeUserUsernameWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK RestoreDataWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 显示系统维护窗口
void ShowSystemMaintenanceWindow(HWND hWnd);

// 显示密码维护窗口
void ShowPasswordMaintenanceWindow(HWND hWnd);

// 显示重置用户密码窗口
void ShowResetPasswordWindow(HWND hWnd);
void ShowChangeUsernameWindow(HWND hWnd);
void ShowChangeUserUsernameWindow(HWND hWnd);
void ShowRestoreDataWindow(HWND hWnd);


// 填充用户列表
void FillUserList(HWND hListView);

// 重置指定用户的密码
bool ResetUserPassword(int userId, const wchar_t* newPassword);

LRESULT CALLBACK StatisticsWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void FillCategoryComboBox(HWND hComboBox);
void FillLabroomComboBox(HWND hComboBox);
void ExecuteEquipmentStatistics(HWND hWnd, HWND hListView, HWND hCheckByCategory, HWND hCategoryCombo,
    HWND hCheckByLabroom, HWND hLabroomCombo, HWND hCheckByPrice,
    HWND hEditMinPrice, HWND hEditMaxPrice, HWND hCheckByDate,
    HWND hEditStartDate, HWND hEditEndDate);
void ExecuteAccountStatistics(HWND hWnd, HWND hListView, HWND hCheckByAccountType,
    HWND hAccountTypeCombo, HWND hCheckByAccountLabroom,
    HWND hAccountLabroomCombo);
void ShowStatisticsWindow(HWND hWnd);
