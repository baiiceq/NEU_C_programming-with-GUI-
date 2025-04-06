#pragma once
#ifndef UI_H
#define UI_H

#include <windows.h>


#define IDC_USERNAME 101
#define IDC_PASSWORD 102
#define IDC_CONFIRM_PASSWORD 103
#define IDC_BTN_LOGIN 104
#define IDC_BTN_REGISTER 105
#define IDC_BTN_GOTO_REGISTER 106
#define IDC_BTN_BACK 107
#define IDC_ACCOUNT_TYPE 108

#define ID_BTN_INFO_MANAGE    201
#define ID_BTN_SERVICE        202
#define ID_BTN_QUERY          203
#define ID_BTN_SORT           204
#define ID_BTN_STATISTICS     205
#define ID_BTN_SYSTEM_MAINT   206
#define ID_BTN_EXIT           207


void ShowLoginWindow(HWND hWnd);
void ShowRegisterWindow(HWND hWnd);
void ShowAdminManagementWindow(HWND hWnd);
void ShowExperimenterManagementWindow(HWND hWnd);
void ShowUserManagementWindow(HWND hWnd);
#endif // UI_H