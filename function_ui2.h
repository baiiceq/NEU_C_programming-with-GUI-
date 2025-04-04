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

// 系统维护窗口过程函数
LRESULT CALLBACK SystemMaintenanceWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 密码维护窗口过程函数
LRESULT CALLBACK PasswordMaintenanceWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 重置用户密码窗口过程函数
LRESULT CALLBACK ResetPasswordWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChangeUsernameWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChangeUserUsernameWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 显示系统维护窗口
void ShowSystemMaintenanceWindow(HWND hWnd);

// 显示密码维护窗口
void ShowPasswordMaintenanceWindow(HWND hWnd);

// 显示重置用户密码窗口
void ShowResetPasswordWindow(HWND hWnd);
void ShowChangeUsernameWindow(HWND hWnd);
void ShowChangeUserUsernameWindow(HWND hWnd);

// 填充用户列表
void FillUserList(HWND hListView);

// 重置指定用户的密码
bool ResetUserPassword(int userId, const wchar_t* newPassword);
