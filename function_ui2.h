#pragma once
#include <Windows.h>
#include <commctrl.h>
#include "account.h"
#include "resource_manager.h"
#include "configs.h"

// ϵͳά����صĿؼ�ID����
#define ID_BTN_PASSWORD_MAINTAIN     1001
#define ID_BTN_RESET_PASSWORD        1002
#define ID_BTN_DATA_BACKUP           1003
#define ID_BTN_DATA_RESTORE          1004
#define ID_BTN_BACK                  1005
#define ID_BTN_CHANGE_USERNAME	     1006
#define ID_BTN_CHANGE_USER_USERNAME  1007

// ����ά������ؼ�ID
#define IDC_OLD_PASSWORD             1101
#define IDC_NEW_PASSWORD             1102
#define IDC_CONFIRM_PASSWORD         1103
#define IDC_BTN_CONFIRM              1104
#define IDC_BTN_CANCEL               1105

// �����û��������ؼ�ID
#define IDC_USER_LISTVIEW            1201
#define IDC_BTN_RESET                1202

//�޸��û�������ؼ�ID
#define IDC_NEW_USERNAME             1301

//�޸������û��û�������ؼ�ID
#define IDC_BTN_CHANGE_USERNAME     1401

// ϵͳά�����ڹ��̺���
LRESULT CALLBACK SystemMaintenanceWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// ����ά�����ڹ��̺���
LRESULT CALLBACK PasswordMaintenanceWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// �����û����봰�ڹ��̺���
LRESULT CALLBACK ResetPasswordWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChangeUsernameWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChangeUserUsernameWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// ��ʾϵͳά������
void ShowSystemMaintenanceWindow(HWND hWnd);

// ��ʾ����ά������
void ShowPasswordMaintenanceWindow(HWND hWnd);

// ��ʾ�����û����봰��
void ShowResetPasswordWindow(HWND hWnd);
void ShowChangeUsernameWindow(HWND hWnd);
void ShowChangeUserUsernameWindow(HWND hWnd);

// ����û��б�
void FillUserList(HWND hListView);

// ����ָ���û�������
bool ResetUserPassword(int userId, const wchar_t* newPassword);
