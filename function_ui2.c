#include  "function_ui2.h"
#include "equipment_management.h"
#include "systemmaintenance.h"

extern HWND hwndAdminManagement;
extern EquipmentManagement* em;
HWND hwndSystemMaintenance;

LRESULT CALLBACK SystemMaintenanceWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		// 创建标题
		CreateWindow(L"STATIC", L"系统维护", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			320, 30, 160, 30, hWnd, NULL, NULL, NULL);
		// 创建显示当前用户名的静态文本控件
		wchar_t usernameText[100];
		swprintf_s(usernameText, 100, L"当前用户: %s", em->current_account->user_name);
		CreateWindow(L"STATIC", usernameText, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			320, 70, 160, 30, hWnd, NULL, NULL, NULL);
		// 创建密码维护按钮和说明
		CreateWindow(L"BUTTON", L"密码维护", WS_VISIBLE | WS_CHILD,
			90, 120, 120, 40, hWnd, (HMENU)ID_BTN_PASSWORD_MAINTAIN, GetModuleHandle(NULL), NULL);
		CreateWindow(L"STATIC", L"修改当前用户的密码", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			240, 120, 480, 40, hWnd, NULL, NULL, NULL);

		// 创建重置用户密码按钮和说明
		CreateWindow(L"BUTTON", L"重置用户密码", WS_VISIBLE | WS_CHILD,
			90, 180, 120, 40, hWnd, (HMENU)ID_BTN_RESET_PASSWORD, GetModuleHandle(NULL), NULL);
		CreateWindow(L"STATIC", L"对其他用户的密码进行重置", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			240, 180, 480, 40, hWnd, NULL, NULL, NULL);

		// 创建数据备份按钮和说明
		CreateWindow(L"BUTTON", L"数据备份", WS_VISIBLE | WS_CHILD,
			90, 360, 120, 40, hWnd, (HMENU)ID_BTN_DATA_BACKUP, GetModuleHandle(NULL), NULL);
		CreateWindow(L"STATIC", L"将当前系统数据备份保存", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			240, 360, 480, 40, hWnd, NULL, NULL, NULL);

		// 创建数据恢复按钮和说明
		CreateWindow(L"BUTTON", L"数据恢复", WS_VISIBLE | WS_CHILD,
			90, 420, 120, 40, hWnd, (HMENU)ID_BTN_DATA_RESTORE, GetModuleHandle(NULL), NULL);
		CreateWindow(L"STATIC", L"从备份文件中恢复系统数据", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			240, 420, 480, 40, hWnd, NULL, NULL, NULL);

		// 创建修改当前用户名按钮和说明
		CreateWindow(L"BUTTON", L"修改用户名", WS_VISIBLE | WS_CHILD,
			90, 240, 120, 40, hWnd, (HMENU)ID_BTN_CHANGE_USERNAME, GetModuleHandle(NULL), NULL);
		CreateWindow(L"STATIC", L"修改当前用户的用户名", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			240, 240, 480, 40, hWnd, NULL, NULL, NULL);

		// 创建修改其他用户用户名按钮和说明
		CreateWindow(L"BUTTON", L"修改用户用户名", WS_VISIBLE | WS_CHILD,
			90, 300, 120, 40, hWnd, (HMENU)ID_BTN_CHANGE_USER_USERNAME, GetModuleHandle(NULL), NULL);
		CreateWindow(L"STATIC", L"修改其他用户的用户名", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			240, 300, 480, 40, hWnd, NULL, NULL, NULL);

		// 创建返回按钮和说明
		CreateWindow(L"BUTTON", L"返回", WS_VISIBLE | WS_CHILD,
			90, 480, 120, 40, hWnd, (HMENU)ID_BTN_BACK, GetModuleHandle(NULL), NULL);
		CreateWindow(L"STATIC", L"返回上一级菜单", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			240, 480, 480, 40, hWnd, NULL, NULL, NULL);
		break;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_BTN_PASSWORD_MAINTAIN:
			// 显示密码维护窗口
			ShowPasswordMaintenanceWindow(hWnd);
			break;
		case ID_BTN_RESET_PASSWORD:
			// 显示重置用户密码窗口
			ShowResetPasswordWindow(hWnd);
			break;
		case ID_BTN_DATA_BACKUP:
			// 处理数据备份
			if (MessageBox(hWnd, L"确定要备份当前系统数据吗？", L"数据备份", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				if (BackupSystemData(em->current_account->id))
					MessageBox(hWnd, L"数据备份成功！", L"提示", MB_OK | MB_ICONINFORMATION);
				else
					MessageBox(hWnd, L"数据备份失败！", L"错误", MB_OK | MB_ICONERROR);
			}
			break;
		case ID_BTN_DATA_RESTORE:
			// 处理数据恢复
			ShowRestoreDataWindow(hWnd);
			break;
		case ID_BTN_CHANGE_USERNAME:
			// 显示修改当前用户名窗口
			ShowChangeUsernameWindow(hWnd);
			break;
		case ID_BTN_CHANGE_USER_USERNAME:
			// 显示修改其他用户用户名窗口
			ShowChangeUserUsernameWindow(hWnd);
			break;
		case ID_BTN_BACK:
		{
			// 返回上一级菜单
			ShowWindow(hwndAdminManagement, SW_SHOW);
			UpdateWindow(hwndAdminManagement);
			ShowWindow(hwndSystemMaintenance, SW_HIDE);
			break;
		}
		}
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		hwndSystemMaintenance = NULL;
		ShowWindow(hwndAdminManagement, SW_SHOW);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// 修改自己密码窗口过程
LRESULT CALLBACK PasswordMaintenanceWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hOldPasswordEdit = NULL;
	static HWND hNewPasswordEdit = NULL;
	static HWND hConfirmPasswordEdit = NULL;

	switch (msg)
	{
	case WM_CREATE:
		// 创建密码维护界面的控件
		CreateWindow(L"STATIC", L"当前密码:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			100, 50, 100, 25, hWnd, NULL, NULL, NULL);
		hOldPasswordEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
			220, 50, 180, 25, hWnd, (HMENU)IDC_OLD_PASSWORD, NULL, NULL);

		CreateWindow(L"STATIC", L"新密码:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			100, 90, 100, 25, hWnd, NULL, NULL, NULL);
		hNewPasswordEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
			220, 90, 180, 25, hWnd, (HMENU)IDC_NEW_PASSWORD, NULL, NULL);

		CreateWindow(L"STATIC", L"确认新密码:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			100, 130, 100, 25, hWnd, NULL, NULL, NULL);
		hConfirmPasswordEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
			220, 130, 180, 25, hWnd, (HMENU)IDC_CONFIRM_PASSWORD, NULL, NULL);

		CreateWindow(L"BUTTON", L"确定", WS_VISIBLE | WS_CHILD,
			150, 180, 100, 30, hWnd, (HMENU)IDC_BTN_CONFIRM, NULL, NULL);
		CreateWindow(L"BUTTON", L"取消", WS_VISIBLE | WS_CHILD,
			270, 180, 100, 30, hWnd, (HMENU)IDC_BTN_CANCEL, NULL, NULL);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BTN_CONFIRM:
		{
			wchar_t oldPassword[50], newPassword[50], confirmPassword[50];
			GetWindowText(hOldPasswordEdit, oldPassword, 50);
			GetWindowText(hNewPasswordEdit, newPassword, 50);
			GetWindowText(hConfirmPasswordEdit, confirmPassword, 50);
			Account testaccount;
			wcscpy_s(testaccount.user_name, USER_NMAE_LENGTH, em->current_account->user_name);
			wcscpy_s(testaccount.user_password, USER_PASSWORD_LENGTH, oldPassword);

			if (!IsCorrectAccount(&testaccount)) {
				MessageBox(NULL, L"原密码不正确！", L"错误", MB_OK | MB_ICONERROR);
				return False;
			}
			if (!IsValidPassword(newPassword)) {
				MessageBox(NULL, L"密码不符合规则！", L"错误", MB_OK | MB_ICONERROR);
				return False;
			}
			if (wcscmp(newPassword, confirmPassword) != 0) {
				MessageBox(NULL, L"两次输入的密码不一致", L"错误", MB_OK | MB_ICONERROR);
				return False;
			}

			wcscpy_s(em->current_account->user_password, USER_PASSWORD_LENGTH, newPassword);
			MessageBox(NULL, L"密码修改成功", L"提示", MB_OK);
			DestroyWindow(hWnd);

			char operation[20];
			switch (em->current_account->account_type)
			{
			case Admin:
				strcpy_s(operation, 20, "修改管理员密码");
				break;
			case User:
				strcpy_s(operation, 20, "修改一般用户密码");
				break;
			case Experimenter:
				strcpy_s(operation, 20, "修改实验员密码");
				break;
			}
			LogSystemOperation(operation, em->current_account->id);
		}
		break;
		case IDC_BTN_CANCEL:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		ShowWindow(hwndSystemMaintenance, SW_SHOW);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// 重置用户密码窗口过程
LRESULT CALLBACK ResetPasswordWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hUserListView = NULL;
	static HWND hNewPasswordEdit = NULL;

	switch (msg)
	{
	case WM_CREATE:
	{
		CreateWindow(L"STATIC", L"用户列表:", WS_VISIBLE | WS_CHILD | SS_LEFT,
			50, 30, 100, 20, hWnd, NULL, NULL, NULL);

		// 创建用户列表视图
		hUserListView = CreateWindowEx(0, WC_LISTVIEW, NULL,
			WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_SINGLESEL,
			50, 60, 500, 200, hWnd, (HMENU)IDC_USER_LISTVIEW, GetModuleHandle(NULL), NULL);

		// 添加列
		LVCOLUMNW lvc;
		lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

		lvc.pszText = L"用户ID";
		lvc.cx = 80;
		lvc.iSubItem = 0;
		ListView_InsertColumn(hUserListView, 0, &lvc);

		lvc.pszText = L"用户名";
		lvc.cx = 120;
		lvc.iSubItem = 1;
		ListView_InsertColumn(hUserListView, 1, &lvc);

		lvc.pszText = L"用户类型";
		lvc.cx = 120;
		lvc.iSubItem = 2;
		ListView_InsertColumn(hUserListView, 2, &lvc);

		lvc.pszText = L"管理室ID";
		lvc.cx = 100;
		lvc.iSubItem = 3;
		ListView_InsertColumn(hUserListView, 3, &lvc);

		// 填充用户列表数据
		FillUserList(hUserListView);

		CreateWindow(L"STATIC", L"新密码:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			150, 280, 80, 25, hWnd, NULL, NULL, NULL);
		hNewPasswordEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
			250, 280, 180, 25, hWnd, (HMENU)IDC_NEW_PASSWORD, NULL, NULL);

		CreateWindow(L"BUTTON", L"重置密码", WS_VISIBLE | WS_CHILD,
			180, 330, 120, 30, hWnd, (HMENU)IDC_BTN_RESET, NULL, NULL);
		CreateWindow(L"BUTTON", L"取消", WS_VISIBLE | WS_CHILD,
			320, 330, 100, 30, hWnd, (HMENU)IDC_BTN_CANCEL, NULL, NULL);
		break;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BTN_RESET:
		{
			int selectedIndex = ListView_GetNextItem(hUserListView, -1, LVNI_SELECTED);
			if (selectedIndex == -1)
			{
				MessageBox(hWnd, L"请选择一个用户！", L"提示", MB_OK | MB_ICONINFORMATION);
				break;
			}

			wchar_t userIdText[16];
			ListView_GetItemText(hUserListView, selectedIndex, 0, userIdText, 16);
			int userId = _wtoi(userIdText);

			wchar_t newPassword[50];
			GetWindowText(hNewPasswordEdit, newPassword, 50);
			if (wcslen(newPassword) == 0)
			{
				MessageBox(hWnd, L"请输入新密码！", L"提示", MB_OK | MB_ICONINFORMATION);
				break;
			}

			if (ResetUserPassword(userId, newPassword))
			{
				MessageBox(hWnd, L"密码重置成功！", L"提示", MB_OK | MB_ICONINFORMATION);
				DestroyWindow(hWnd);
			}
			else
			{
				MessageBox(hWnd, L"密码重置失败！", L"错误", MB_OK | MB_ICONERROR);
			}
		}
		break;
		case IDC_BTN_CANCEL:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		ShowWindow(hwndSystemMaintenance, SW_SHOW);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//修改自己用户名窗口过程
LRESULT CALLBACK ChangeUsernameWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hNewUsernameEdit = NULL;

	switch (msg)
	{
	case WM_CREATE:
		// 创建修改用户名界面的控件
		CreateWindow(L"STATIC", L"新用户名:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			100, 50, 100, 25, hWnd, NULL, NULL, NULL);
		hNewUsernameEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
			220, 50, 180, 25, hWnd, (HMENU)IDC_NEW_USERNAME, NULL, NULL);

		CreateWindow(L"BUTTON", L"确定", WS_VISIBLE | WS_CHILD,
			150, 100, 100, 30, hWnd, (HMENU)IDC_BTN_CONFIRM, NULL, NULL);
		CreateWindow(L"BUTTON", L"取消", WS_VISIBLE | WS_CHILD,
			270, 100, 100, 30, hWnd, (HMENU)IDC_BTN_CANCEL, NULL, NULL);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BTN_CONFIRM:
		{
			wchar_t newUsername[50];
			GetWindowText(hNewUsernameEdit, newUsername, 50);

			if (wcslen(newUsername) == 0)
			{
				MessageBox(hWnd, L"请输入新用户名！", L"提示", MB_OK | MB_ICONINFORMATION);
				break;
			}
			if (!IsValidUsername(newUsername))
			{
				MessageBox(hWnd, L"用户名不符合规则！", L"错误", MB_OK | MB_ICONERROR);
				break;
			}
			wcscpy_s(em->current_account->user_name, USER_NMAE_LENGTH, newUsername);
			MessageBox(hWnd, L"用户名修改成功！", L"提示", MB_OK | MB_ICONINFORMATION);
			DestroyWindow(hWnd);
		}
		break;
		case IDC_BTN_CANCEL:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		ShowWindow(hwndSystemMaintenance, SW_SHOW);
		SendMessage(hwndSystemMaintenance, WM_CREATE, 0, 0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//修改其他用户用户名窗口过程
LRESULT CALLBACK ChangeUserUsernameWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hUserListView = NULL;
	static HWND hNewUsernameEdit = NULL;

	switch (msg)
	{
	case WM_CREATE:
	{
		CreateWindow(L"STATIC", L"用户列表:", WS_VISIBLE | WS_CHILD | SS_LEFT,
			50, 30, 100, 20, hWnd, NULL, NULL, NULL);

		// 创建用户列表视图
		hUserListView = CreateWindowEx(0, WC_LISTVIEW, NULL,
			WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_SINGLESEL,
			50, 60, 500, 200, hWnd, (HMENU)IDC_USER_LISTVIEW, GetModuleHandle(NULL), NULL);

		// 添加列
		LVCOLUMNW lvc;
		lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

		lvc.pszText = L"用户ID";
		lvc.cx = 80;
		lvc.iSubItem = 0;
		ListView_InsertColumn(hUserListView, 0, &lvc);

		lvc.pszText = L"用户名";
		lvc.cx = 120;
		lvc.iSubItem = 1;
		ListView_InsertColumn(hUserListView, 1, &lvc);

		lvc.pszText = L"用户类型";
		lvc.cx = 120;
		lvc.iSubItem = 2;
		ListView_InsertColumn(hUserListView, 2, &lvc);

		lvc.pszText = L"管理室ID";
		lvc.cx = 100;
		lvc.iSubItem = 3;
		ListView_InsertColumn(hUserListView, 3, &lvc);

		// 填充用户列表数据
		FillUserList(hUserListView);

		CreateWindow(L"STATIC", L"新用户名:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			150, 280, 80, 25, hWnd, NULL, NULL, NULL);
		hNewUsernameEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
			250, 280, 180, 25, hWnd, (HMENU)IDC_NEW_USERNAME, NULL, NULL);

		CreateWindow(L"BUTTON", L"修改用户名", WS_VISIBLE | WS_CHILD,
			180, 330, 120, 30, hWnd, (HMENU)IDC_BTN_CHANGE_USERNAME, NULL, NULL);
		CreateWindow(L"BUTTON", L"取消", WS_VISIBLE | WS_CHILD,
			320, 330, 100, 30, hWnd, (HMENU)IDC_BTN_CANCEL, NULL, NULL);
		break;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BTN_CHANGE_USERNAME:
		{
			int selectedIndex = ListView_GetNextItem(hUserListView, -1, LVNI_SELECTED);
			if (selectedIndex == -1)
			{
				MessageBox(hWnd, L"请选择一个用户！", L"提示", MB_OK | MB_ICONINFORMATION);
				break;
			}

			wchar_t userIdText[16];
			ListView_GetItemText(hUserListView, selectedIndex, 0, userIdText, 16);
			int userId = _wtoi(userIdText);

			wchar_t newUsername[50];
			GetWindowText(hNewUsernameEdit, newUsername, 50);
			if (wcslen(newUsername) == 0)
			{
				MessageBox(hWnd, L"请输入新用户名！", L"提示", MB_OK | MB_ICONINFORMATION);
				break;
			}

			Account* account = FindById(userId);
			if (account == NULL)
				break;
			if (IsValidUsername(newUsername) == False)
			{
				MessageBox(hWnd, L"用户名不符合规则！", L"错误", MB_OK | MB_ICONERROR);
				break;
			}
			wcscpy_s(account->user_name, USER_NMAE_LENGTH, newUsername);
			MessageBox(hWnd, L"用户名修改成功！", L"提示", MB_OK | MB_ICONINFORMATION);
			break;
		}
		break;
		case IDC_BTN_CANCEL:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		ShowWindow(hwndAdminManagement, SW_SHOW);
		break;

	case WM_DESTROY:
		ShowWindow(hwndSystemMaintenance, SW_SHOW);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//数据恢复窗口
LRESULT CALLBACK RestoreDataWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hRestoreTimeEdit = NULL;

	switch (msg)
	{
	case WM_CREATE:
		// 创建数据恢复界面的控件
		CreateWindow(L"STATIC", L"恢复节点时间(备份文件夹的名字)", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			140, 20, 280, 25, hWnd, NULL, NULL, NULL);
		hRestoreTimeEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
			170, 50, 180, 25, hWnd, (HMENU)IDC_RESTORE_TIME, NULL, NULL);

		CreateWindow(L"BUTTON", L"确定", WS_VISIBLE | WS_CHILD,
			150, 100, 100, 30, hWnd, (HMENU)IDC_BTN_CONFIRM, NULL, NULL);
		CreateWindow(L"BUTTON", L"取消", WS_VISIBLE | WS_CHILD,
			270, 100, 100, 30, hWnd, (HMENU)IDC_BTN_CANCEL, NULL, NULL);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BTN_CONFIRM:
		{
			wchar_t restoreTime[50];
			GetWindowText(hRestoreTimeEdit, restoreTime, 50);

			if (wcslen(restoreTime) == 0)
			{
				MessageBox(hWnd, L"请输入恢复节点时间！", L"提示", MB_OK | MB_ICONINFORMATION);
				break;
			}

			if (RestoreSystemData(em->current_account->id, restoreTime))
			{
				MessageBox(hWnd, L"数据恢复成功！", L"提示", MB_OK | MB_ICONINFORMATION);
				DestroyWindow(hWnd);
			}
			else
			{
				MessageBox(hWnd, L"数据恢复失败！", L"错误", MB_OK | MB_ICONERROR);
			}
		}
		break;
		case IDC_BTN_CANCEL:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		ShowWindow(hwndSystemMaintenance, SW_SHOW);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void ShowSystemMaintenanceWindow(HWND hWnd)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = SystemMaintenanceWndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"SystemMaintenanceWindow";

	RegisterClass(&wc);

	hwndSystemMaintenance = CreateWindow(L"SystemMaintenanceWindow", L"系统维护",
		WS_OVERLAPPED | WS_SYSMENU, 200, 100, 800, 600, hWnd, NULL, GetModuleHandle(NULL), NULL);

	ShowWindow(hwndSystemMaintenance, SW_SHOW);
	UpdateWindow(hwndSystemMaintenance);
}

void ShowPasswordMaintenanceWindow(HWND hWnd)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = PasswordMaintenanceWndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"PasswordMaintenanceWindow";

	RegisterClass(&wc);

	HWND hwndPasswordMaintenance = CreateWindow(L"PasswordMaintenanceWindow", L"密码维护",
		WS_OVERLAPPED | WS_SYSMENU, 300, 200, 500, 300, hWnd, NULL, GetModuleHandle(NULL), NULL);

	ShowWindow(hwndPasswordMaintenance, SW_SHOW);
	UpdateWindow(hwndPasswordMaintenance);
}

void ShowResetPasswordWindow(HWND hWnd)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = ResetPasswordWndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"ResetPasswordWindow";

	RegisterClass(&wc);

	HWND hwndResetPassword = CreateWindow(L"ResetPasswordWindow", L"重置用户密码",
		WS_OVERLAPPED | WS_SYSMENU, 250, 150, 600, 400, hWnd, NULL, GetModuleHandle(NULL), NULL);

	ShowWindow(hwndResetPassword, SW_SHOW);
	UpdateWindow(hwndResetPassword);
}

void ShowChangeUsernameWindow(HWND hWnd)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = ChangeUsernameWndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"ChangeUsernameWindow";

	RegisterClass(&wc);

	HWND hwndChangeUsername = CreateWindow(L"ChangeUsernameWindow", L"修改用户名",
		WS_OVERLAPPED | WS_SYSMENU, 300, 200, 500, 200, hWnd, NULL, GetModuleHandle(NULL), NULL);

	ShowWindow(hwndChangeUsername, SW_SHOW);
	UpdateWindow(hwndChangeUsername);
}

void ShowChangeUserUsernameWindow(HWND hWnd)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = ChangeUserUsernameWndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"ChangeUserUsernameWindow";

	RegisterClass(&wc);

	HWND hwndChangeUserUsername = CreateWindow(L"ChangeUserUsernameWindow", L"修改用户用户名",
		WS_OVERLAPPED | WS_SYSMENU, 250, 150, 600, 400, hWnd, NULL, GetModuleHandle(NULL), NULL);

	ShowWindow(hwndChangeUserUsername, SW_SHOW);
	UpdateWindow(hwndChangeUserUsername);
}

void ShowRestoreDataWindow(HWND hWnd)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = RestoreDataWndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"RestoreDataWindow";

	RegisterClass(&wc);

	HWND hwndRestoreData = CreateWindow(L"RestoreDataWindow", L"数据恢复",
		WS_OVERLAPPED | WS_SYSMENU, 300, 200, 500, 200, hWnd, NULL, GetModuleHandle(NULL), NULL);

	ShowWindow(hwndRestoreData, SW_SHOW);
	UpdateWindow(hwndRestoreData);
}
// 填充用户列表
void FillUserList(HWND hListView)
{
	ListView_DeleteAllItems(hListView);

	ResourceManager* rm = GetResourceManage();
	if (!rm || !rm->account_list) return;

	Node* temp = rm->account_list->head;
	size_t count = rm->account_list->size;

	for (size_t i = 0; i < count; i++)
	{
		temp = temp->next;
		if (!temp) break;

		Account* account = (Account*)temp->data;
		if (!account) continue;

		LVITEM lvItem;
		ZeroMemory(&lvItem, sizeof(LVITEM));
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = i;
		lvItem.iSubItem = 0;

		// 用户ID
		wchar_t idBuffer[16];
		swprintf_s(idBuffer, 16, L"%d", account->id);
		lvItem.pszText = idBuffer;
		ListView_InsertItem(hListView, &lvItem);

		// 用户名
		ListView_SetItemText(hListView, i, 1, account->user_name);

		// 用户类型
		wchar_t* typeText;
		switch (account->account_type)
		{
		case Admin:
			typeText = L"管理员";
			break;
		case Experimenter:
			typeText = L"实验员";
			break;
		case User:
			typeText = L"普通用户";
			break;
		default:
			typeText = L"未知";
			break;
		}
		ListView_SetItemText(hListView, i, 2, typeText);

		// 管理室ID
		wchar_t roomBuffer[16];
		swprintf_s(roomBuffer, 16, L"%d", account->roomid);
		ListView_SetItemText(hListView, i, 3, roomBuffer);
	}
}

// 重置指定用户的密码
bool ResetUserPassword(int userId, const wchar_t* newPassword)
{
	Account* account = FindById(userId);
	if (account == NULL)
	{
		return False;
	}
	if (!IsValidPassword(newPassword)) {
		MessageBox(NULL, L"密码不符合规则！", L"错误", MB_OK | MB_ICONERROR);
		return False;
	}
	wcscpy_s(account->user_password, USER_PASSWORD_LENGTH, newPassword);
	return True;
}

LRESULT CALLBACK UserMaintenanceWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hCurrentUsername, hNewUsername, hCurrentPassword, hNewPassword, hConfirmPassword;

	switch (msg)
	{
	case WM_CREATE:
		// 显示当前用户名
		CreateWindow(L"STATIC", L"当前用户名:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 50, 30, 100, 25, hWnd, NULL, NULL, NULL);
		hCurrentUsername = CreateWindow(L"STATIC", em->current_account->user_name, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 160, 30, 180, 25, hWnd, NULL, NULL, NULL);

		// 修改用户名
		CreateWindow(L"STATIC", L"新用户名:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 50, 70, 100, 25, hWnd, NULL, NULL, NULL);
		hNewUsername = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 70, 180, 25, hWnd, NULL, NULL, NULL);

		// 修改用户名按钮
		CreateWindow(L"BUTTON", L"修改用户名", WS_VISIBLE | WS_CHILD, 160, 110, 120, 30, hWnd, (HMENU)IDC_BTN_CHANGE_USERNAME, NULL, NULL);

		// 修改密码
		CreateWindow(L"STATIC", L"当前密码:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 50, 150, 100, 25, hWnd, NULL, NULL, NULL);
		hCurrentPassword = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 160, 150, 180, 25, hWnd, NULL, NULL, NULL);

		CreateWindow(L"STATIC", L"新密码:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 50, 190, 100, 25, hWnd, NULL, NULL, NULL);
		hNewPassword = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 160, 190, 180, 25, hWnd, NULL, NULL, NULL);

		CreateWindow(L"STATIC", L"确认新密码:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 50, 230, 100, 25, hWnd, NULL, NULL, NULL);
		hConfirmPassword = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 160, 230, 180, 25, hWnd, NULL, NULL, NULL);

		// 修改密码按钮
		CreateWindow(L"BUTTON", L"修改密码", WS_VISIBLE | WS_CHILD, 160, 270, 120, 30, hWnd, (HMENU)IDC_BTN_CHANGE_PASSWORD, NULL, NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BTN_CHANGE_USERNAME:
		{
			wchar_t newUsername[50];
			GetWindowText(hNewUsername, newUsername, 50);

			if (wcslen(newUsername) == 0)
			{
				MessageBox(hWnd, L"请输入新用户名！", L"提示", MB_OK | MB_ICONINFORMATION);
				break;
			}
			if (!IsValidUsername(newUsername))
			{
				MessageBox(hWnd, L"用户名不符合规则！", L"错误", MB_OK | MB_ICONERROR);
				break;
			}
			wcscpy_s(em->current_account->user_name, USER_NMAE_LENGTH, newUsername);
			MessageBox(hWnd, L"用户名修改成功！", L"提示", MB_OK | MB_ICONINFORMATION);
			break;
		}
		case IDC_BTN_CHANGE_PASSWORD:
		{
			wchar_t oldPassword[50], newPassword[50], confirmPassword[50];
			GetWindowText(hCurrentPassword, oldPassword, 50);
			GetWindowText(hNewPassword, newPassword, 50);
			GetWindowText(hConfirmPassword, confirmPassword, 50);
			Account testaccount;
			wcscpy_s(testaccount.user_name, USER_NMAE_LENGTH, em->current_account->user_name);
			wcscpy_s(testaccount.user_password, USER_PASSWORD_LENGTH, oldPassword);

			if (!IsCorrectAccount(&testaccount)) {
				MessageBox(NULL, L"原密码不正确！", L"错误", MB_OK | MB_ICONERROR);
				return False;
			}
			if (!IsValidPassword(newPassword)) {
				MessageBox(NULL, L"密码不符合规则！", L"错误", MB_OK | MB_ICONERROR);
				return False;
			}
			if (wcscmp(newPassword, confirmPassword) != 0) {
				MessageBox(NULL, L"两次输入的密码不一致", L"错误", MB_OK | MB_ICONERROR);
				return False;
			}

			wcscpy_s(em->current_account->user_password, USER_PASSWORD_LENGTH, newPassword);
			MessageBox(NULL, L"密码修改成功", L"提示", MB_OK);

			char operation[20];
			switch (em->current_account->account_type)
			{
			case Admin:
				strcpy_s(operation, 20, "修改管理员密码");
				break;
			case User:
				strcpy_s(operation, 20, "修改一般用户密码");
				break;
			case Experimenter:
				strcpy_s(operation, 20, "修改实验员密码");
				break;
			}
			LogSystemOperation(operation, em->current_account->id);
			break;
		}
		}
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		ShowWindow(hwndAdminManagement, SW_SHOW);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void ShowUserMaintenanceWindow(HWND hWnd)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = UserMaintenanceWndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"UserMaintenanceWindow";

	RegisterClass(&wc);

	HWND hwndUserMaintenance = CreateWindow(L"UserMaintenanceWindow", L"系统维护", WS_OVERLAPPED | WS_SYSMENU,
		400, 200, 400, 350, hWnd, NULL, GetModuleHandle(NULL), NULL);

	ShowWindow(hwndUserMaintenance, SW_SHOW);
	UpdateWindow(hwndUserMaintenance);
}
