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
		//ShowWindow(hwndAdminManagement, SW_SHOW);
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

			/*wchar_t operation[20];
			switch (account->account_type)
			{
			case Admin:
				wcscpy_s(operation, 20, L"修改管理员密码");
				break;
			case User:
				wcscpy_s(operation, 20, L"修改一般用户密码");
				break;
			case Experimenter:
				wcscpy_s(operation, 20, L"修改实验员密码");
				break;
			}
			LogSystemOperation(operation, account->id);*/
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
	wcscpy_s(account->user_password, USER_PASSWORD_LENGTH, newPassword);
	return True;
}

#include "statistic_ui.h"
#include "equipment_management.h" 



extern HWND hwndSystemMaintenance;
extern EquipmentManagement* em;

HWND hwndStatistics = NULL;
HWND hwndEquipmentGroup = NULL;
HWND hwndAccountGroup = NULL;

// 统计功能窗口过程
LRESULT CALLBACK StatisticsWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hRadioEquipment = NULL;
	static HWND hRadioAccount = NULL;
	static HWND hCategoryCombo = NULL;
	static HWND hLabroomCombo = NULL;
	static HWND hCheckByCategory = NULL;
	static HWND hCheckByLabroom = NULL;
	static HWND hCheckByPrice = NULL;
	static HWND hCheckByDate = NULL;
	static HWND hCheckByAccountType = NULL;
	static HWND hCheckByAccountLabroom = NULL;
	static HWND hEditMinPrice = NULL;
	static HWND hEditMaxPrice = NULL;
	static HWND hEditStartDate = NULL;
	static HWND hEditEndDate = NULL;
	static HWND hAccountTypeCombo = NULL;
	static HWND hAccountLabroomCombo = NULL;
	static HWND hResultListView = NULL;

	switch (msg)
	{
	case WM_CREATE:
	{
		// 创建标题
		CreateWindow(L"STATIC", L"统计功能", WS_VISIBLE | WS_CHILD | SS_CENTER,
			300, 10, 200, 30, hWnd, NULL, NULL, NULL);

		// 创建分类选择单选按钮
		hRadioEquipment = CreateWindow(L"BUTTON", L"设备统计", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP,
			50, 50, 100, 30, hWnd, (HMENU)IDC_RADIO_STATS_EQUIPMENT, NULL, NULL);
		hRadioAccount = CreateWindow(L"BUTTON", L"账户统计", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
			200, 50, 100, 30, hWnd, (HMENU)IDC_RADIO_STATS_ACCOUNT, NULL, NULL);

		// 默认选择设备统计
		SendMessage(hRadioEquipment, BM_SETCHECK, BST_CHECKED, 0);

		// 创建设备统计分组框
		hwndEquipmentGroup = CreateWindow(L"BUTTON", L"设备统计选项", WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
			50, 90, 700, 200, hWnd, NULL, NULL, NULL);

		// 创建按设备类型统计选项
		hCheckByCategory = CreateWindow(L"BUTTON", L"按设备类型统计", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			70, 120, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_CATEGORY, NULL, NULL);

		CreateWindow(L"STATIC", L"设备类型:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
			220, 120, 80, 25, hWnd, NULL, NULL, NULL);

		hCategoryCombo = CreateWindow(L"COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
			310, 120, 120, 200, hWnd, (HMENU)IDC_CATEGORY_COMBO, NULL, NULL);

		// 填充设备类型下拉框
		FillCategoryComboBox(hCategoryCombo);

		// 创建按实验室统计选项
		hCheckByLabroom = CreateWindow(L"BUTTON", L"按实验室统计", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			70, 150, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_LABROOM, NULL, NULL);

		CreateWindow(L"STATIC", L"实验室:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
			220, 150, 80, 25, hWnd, NULL, NULL, NULL);

		hLabroomCombo = CreateWindow(L"COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
			310, 150, 120, 200, hWnd, (HMENU)IDC_LABROOM_COMBO, NULL, NULL);

		// 填充实验室下拉框
		FillLabroomComboBox(hLabroomCombo);

		// 创建按价格统计选项
		hCheckByPrice = CreateWindow(L"BUTTON", L"按价格统计", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			70, 180, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_PRICE, NULL, NULL);

		CreateWindow(L"STATIC", L"最低价格:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
			220, 180, 80, 25, hWnd, NULL, NULL, NULL);

		hEditMinPrice = CreateWindow(L"EDIT", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
			310, 180, 80, 25, hWnd, (HMENU)IDC_EDIT_MIN_PRICE, NULL, NULL);

		CreateWindow(L"STATIC", L"最高价格:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
			400, 180, 80, 25, hWnd, NULL, NULL, NULL);

		hEditMaxPrice = CreateWindow(L"EDIT", L"10000", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
			490, 180, 80, 25, hWnd, (HMENU)IDC_EDIT_MAX_PRICE, NULL, NULL);

		// 创建按日期统计选项
		hCheckByDate = CreateWindow(L"BUTTON", L"按日期统计", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			70, 210, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_DATE, NULL, NULL);

		CreateWindow(L"STATIC", L"开始日期:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
			220, 210, 80, 25, hWnd, NULL, NULL, NULL);

		hEditStartDate = CreateWindow(L"EDIT", L"20200101", WS_VISIBLE | WS_CHILD | WS_BORDER,
			310, 210, 80, 25, hWnd, (HMENU)IDC_EDIT_START_DATE, NULL, NULL);

		CreateWindow(L"STATIC", L"结束日期:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
			400, 210, 80, 25, hWnd, NULL, NULL, NULL);

		hEditEndDate = CreateWindow(L"EDIT", L"20240101", WS_VISIBLE | WS_CHILD | WS_BORDER,
			490, 210, 80, 25, hWnd, (HMENU)IDC_EDIT_END_DATE, NULL, NULL);

		// 创建账户统计分组框（初始隐藏）
		hwndAccountGroup = CreateWindow(L"BUTTON", L"账户统计选项", WS_CHILD | BS_GROUPBOX,
			50, 90, 700, 200, hWnd, NULL, NULL, NULL);

		// 创建按账户类型统计选项
		hCheckByAccountType = CreateWindow(L"BUTTON", L"按账户类型统计", WS_CHILD | BS_AUTOCHECKBOX,
			70, 120, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_ACCOUNT_TYPE, NULL, NULL);

		CreateWindow(L"STATIC", L"账户类型:", WS_CHILD | SS_RIGHT,
			220, 120, 80, 25, hWnd, NULL, NULL, NULL);

		hAccountTypeCombo = CreateWindow(L"COMBOBOX", NULL, WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
			310, 120, 150, 200, hWnd, (HMENU)IDC_ACCOUNT_TYPE_COMBO, NULL, NULL);

		// 填充账户类型下拉框
		SendMessage(hAccountTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"管理员");
		SendMessage(hAccountTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"实验员");
		SendMessage(hAccountTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"一般用户");
		SendMessage(hAccountTypeCombo, CB_SETCURSEL, 0, 0);

		// 创建按实验室统计选项（仅针对实验员）
		hCheckByAccountLabroom = CreateWindow(L"BUTTON", L"按实验室统计（仅实验员）", WS_CHILD | BS_AUTOCHECKBOX,
			70, 150, 200, 25, hWnd, (HMENU)IDC_CHECK_BY_ACCOUNT_LABROOM, NULL, NULL);

		CreateWindow(L"STATIC", L"实验室:", WS_CHILD | SS_RIGHT,
			280, 150, 60, 25, hWnd, NULL, NULL, NULL);

		hAccountLabroomCombo = CreateWindow(L"COMBOBOX", NULL, WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
			350, 150, 120, 200, hWnd, (HMENU)IDC_ACCOUNT_LABROOM_COMBO, NULL, NULL);

		// 填充实验室下拉框（与设备统计共用相同数据）
		FillLabroomComboBox(hAccountLabroomCombo);

		// 创建统计结果列表视图
		hResultListView = CreateWindowEx(0, WC_LISTVIEW, NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT,
			50, 300, 700, 200, hWnd, (HMENU)IDC_STATS_RESULT_LIST, NULL, NULL);

		// 添加列
		LVCOLUMN lvc;
		lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

		lvc.pszText = L"统计项目";
		lvc.cx = 200;
		lvc.iSubItem = 0;
		ListView_InsertColumn(hResultListView, 0, &lvc);

		lvc.pszText = L"统计结果";
		lvc.cx = 470;
		lvc.iSubItem = 1;
		ListView_InsertColumn(hResultListView, 1, &lvc);

		// 创建执行统计按钮
		CreateWindow(L"BUTTON", L"执行统计", WS_VISIBLE | WS_CHILD,
			250, 520, 100, 30, hWnd, (HMENU)IDC_BTN_STATS_EXECUTE, NULL, NULL);

		// 创建返回按钮
		CreateWindow(L"BUTTON", L"返回", WS_VISIBLE | WS_CHILD,
			450, 520, 100, 30, hWnd, (HMENU)IDC_BTN_STATS_BACK, NULL, NULL);

		break;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RADIO_STATS_EQUIPMENT:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				// 显示设备统计选项，隐藏账户统计选项
				ShowWindow(hwndEquipmentGroup, SW_SHOW);
				ShowWindow(hwndAccountGroup, SW_HIDE);

				// 显示设备统计的所有子控件
				ShowWindow(hCheckByCategory, SW_SHOW);
				ShowWindow(hCategoryCombo, SW_SHOW);
				ShowWindow(hCheckByLabroom, SW_SHOW);
				ShowWindow(hLabroomCombo, SW_SHOW);
				ShowWindow(hCheckByPrice, SW_SHOW);
				ShowWindow(hEditMinPrice, SW_SHOW);
				ShowWindow(hEditMaxPrice, SW_SHOW);
				ShowWindow(hCheckByDate, SW_SHOW);
				ShowWindow(hEditStartDate, SW_SHOW);
				ShowWindow(hEditEndDate, SW_SHOW);

				// 隐藏账户统计的所有子控件
				ShowWindow(hCheckByAccountType, SW_HIDE);
				ShowWindow(hAccountTypeCombo, SW_HIDE);
				ShowWindow(hCheckByAccountLabroom, SW_HIDE);
				ShowWindow(hAccountLabroomCombo, SW_HIDE);
			}
			break;

		case IDC_RADIO_STATS_ACCOUNT:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				// 显示账户统计选项，隐藏设备统计选项
				ShowWindow(hwndEquipmentGroup, SW_HIDE);
				ShowWindow(hwndAccountGroup, SW_SHOW);

				// 隐藏设备统计的所有子控件
				ShowWindow(hCheckByCategory, SW_HIDE);
				ShowWindow(hCategoryCombo, SW_HIDE);
				ShowWindow(hCheckByLabroom, SW_HIDE);
				ShowWindow(hLabroomCombo, SW_HIDE);
				ShowWindow(hCheckByPrice, SW_HIDE);
				ShowWindow(hEditMinPrice, SW_HIDE);
				ShowWindow(hEditMaxPrice, SW_HIDE);
				ShowWindow(hCheckByDate, SW_HIDE);
				ShowWindow(hEditStartDate, SW_HIDE);
				ShowWindow(hEditEndDate, SW_HIDE);

				// 显示账户统计的所有子控件
				ShowWindow(hCheckByAccountType, SW_SHOW);
				ShowWindow(hAccountTypeCombo, SW_SHOW);
				ShowWindow(hCheckByAccountLabroom, SW_SHOW);
				ShowWindow(hAccountLabroomCombo, SW_SHOW);
			}
			break;

		case IDC_BTN_STATS_EXECUTE:
			// 清空结果列表
			ListView_DeleteAllItems(hResultListView);

			// 判断是设备统计还是账户统计
			if (SendMessage(hRadioEquipment, BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				// 设备统计
				ExecuteEquipmentStatistics(hWnd, hResultListView, hCheckByCategory, hCategoryCombo,
					hCheckByLabroom, hLabroomCombo, hCheckByPrice,
					hEditMinPrice, hEditMaxPrice, hCheckByDate,
					hEditStartDate, hEditEndDate);
			}
			else
			{
				// 账户统计
				ExecuteAccountStatistics(hWnd, hResultListView, hCheckByAccountType,
					hAccountTypeCombo, hCheckByAccountLabroom,
					hAccountLabroomCombo);
			}
			break;

		case IDC_BTN_STATS_BACK:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		hwndStatistics = NULL;
		ShowWindow(hwndSystemMaintenance, SW_SHOW);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// 填充设备类别下拉框
void FillCategoryComboBox(HWND hComboBox)
{
	ResourceManager* rm = GetResourceManage();
	if (!rm || !rm->category_list) return;

	Node* temp = rm->category_list->head->next;
	while (temp)
	{
		Category* category = (Category*)temp->data;
		if (category)
		{
			SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)category->name);
		}
		temp = temp->next;
	}
	SendMessage(hComboBox, CB_SETCURSEL, 0, 0);
}

// 填充实验室下拉框
void FillLabroomComboBox(HWND hComboBox)
{
	ResourceManager* rm = GetResourceManage();
	if (!rm || !rm->laboratory_list) return;

	Node* temp = rm->laboratory_list->head->next;
	while (temp)
	{
		LabRoom* labroom = (LabRoom*)temp->data;
		if (labroom)
		{
			wchar_t buffer[100];
			swprintf(buffer, 100, L"%d - %s", labroom->id, labroom->name);
			SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)buffer);
		}
		temp = temp->next;
	}
	SendMessage(hComboBox, CB_SETCURSEL, 0, 0);
}

// 执行设备统计
void ExecuteEquipmentStatistics(HWND hWnd, HWND hListView, HWND hCheckByCategory, HWND hCategoryCombo,
	HWND hCheckByLabroom, HWND hLabroomCombo, HWND hCheckByPrice,
	HWND hEditMinPrice, HWND hEditMaxPrice, HWND hCheckByDate,
	HWND hEditStartDate, HWND hEditEndDate)
{
	EquipmentsCount count;
	memset(&count, 0, sizeof(EquipmentsCount));

	count.countByCategory = -1;
	count.countByRoom = -1;
	count.countByPrice = -1;
	count.countByDate = -1;
	count.count = 0;

	ResourceManager* rm = GetResourceManage();
	if (!rm) return;

	// 按设备类型统计
	if (SendMessage(hCheckByCategory, BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		count.countByCategory = 1;
		int selectedIndex = SendMessage(hCategoryCombo, CB_GETCURSEL, 0, 0);
		if (selectedIndex != CB_ERR)
		{
			Node* temp = rm->category_list->head->next;
			for (int i = 0; i < selectedIndex && temp; i++)
			{
				temp = temp->next;
			}
			if (temp)
			{
				Category* category = (Category*)temp->data;
				count.categoryId = category->id;
			}
		}
	}

	// 按实验室统计
	if (SendMessage(hCheckByLabroom, BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		count.countByRoom = 1;
		int selectedIndex = SendMessage(hLabroomCombo, CB_GETCURSEL, 0, 0);
		if (selectedIndex != CB_ERR)
		{
			wchar_t buffer[100];
			SendMessage(hLabroomCombo, CB_GETLBTEXT, selectedIndex, (LPARAM)buffer);
			// 解析出实验室ID
			count.room_id = _wtoi(buffer);
		}
	}

	// 按价格统计
	if (SendMessage(hCheckByPrice, BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		count.countByPrice = 1;
		wchar_t minPrice[32], maxPrice[32];
		GetWindowText(hEditMinPrice, minPrice, 32);
		GetWindowText(hEditMaxPrice, maxPrice, 32);
		count.min_price = _wtoi(minPrice);
		count.max_price = _wtoi(maxPrice);
	}

	// 按日期统计
	if (SendMessage(hCheckByDate, BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		count.countByDate = 1;
		wchar_t startDate[DATE_LENGTH], endDate[DATE_LENGTH];
		GetWindowText(hEditStartDate, startDate, DATE_LENGTH);
		GetWindowText(hEditEndDate, endDate, DATE_LENGTH);

		// 转换日期字符串
		char mbStartDate[DATE_LENGTH], mbEndDate[DATE_LENGTH];
		wcstombs_s(NULL, mbStartDate, DATE_LENGTH, startDate, _TRUNCATE);
		wcstombs_s(NULL, mbEndDate, DATE_LENGTH, endDate, _TRUNCATE);

		strcpy_s(count.startDate, DATE_LENGTH, mbStartDate);
		strcpy_s(count.endDate, DATE_LENGTH, mbEndDate);
	}

	// 执行统计
	CountEquipment(rm->equipment_list, &count);

	// 显示统计结果
	int itemIndex = 0;

	// 添加统计条件
	LVITEM lvi;
	wchar_t buffer[256];

	if (count.countByCategory >= 0)
	{
		Category* category = FindCategoryById(count.categoryId);
		if (category)
		{
			lvi.mask = LVIF_TEXT;
			lvi.iItem = itemIndex;
			lvi.iSubItem = 0;
			lvi.pszText = L"设备类型";
			ListView_InsertItem(hListView, &lvi);

			ListView_SetItemText(hListView, itemIndex, 1, category->name);
			itemIndex++;
		}
	}

	if (count.countByRoom >= 0)
	{
		LabRoom* labroom = RoomId_to_LabRoom(count.room_id);
		if (labroom)
		{
			lvi.mask = LVIF_TEXT;
			lvi.iItem = itemIndex;
			lvi.iSubItem = 0;
			lvi.pszText = L"所属实验室";
			ListView_InsertItem(hListView, &lvi);

			ListView_SetItemText(hListView, itemIndex, 1, labroom->name);
			itemIndex++;
		}
	}

	if (count.countByPrice >= 0)
	{
		lvi.mask = LVIF_TEXT;
		lvi.iItem = itemIndex;
		lvi.iSubItem = 0;
		lvi.pszText = L"价格范围";
		ListView_InsertItem(hListView, &lvi);

		swprintf(buffer, 256, L"%d 元 - %d 元", count.min_price, count.max_price);
		ListView_SetItemText(hListView, itemIndex, 1, buffer);
		itemIndex++;
	}

	if (count.countByDate >= 0)
	{
		lvi.mask = LVIF_TEXT;
		lvi.iItem = itemIndex;
		lvi.iSubItem = 0;
		lvi.pszText = L"日期范围";
		ListView_InsertItem(hListView, &lvi);

		wchar_t startDate[DATE_LENGTH], endDate[DATE_LENGTH];
		mbstowcs_s(NULL, startDate, DATE_LENGTH, count.startDate, _TRUNCATE);
		mbstowcs_s(NULL, endDate, DATE_LENGTH, count.endDate, _TRUNCATE);

		swprintf(buffer, 256, L"%s - %s", startDate, endDate);
		ListView_SetItemText(hListView, itemIndex, 1, buffer);
		itemIndex++;
	}

	// 添加统计结果
	lvi.mask = LVIF_TEXT;
	lvi.iItem = itemIndex;
	lvi.iSubItem = 0;
	lvi.pszText = L"统计结果";
	ListView_InsertItem(hListView, &lvi);

	swprintf(buffer, 256, L"共有 %d 件设备", count.count);
	ListView_SetItemText(hListView, itemIndex, 1, buffer);
}

// 执行账户统计
void ExecuteAccountStatistics(HWND hWnd, HWND hListView, HWND hCheckByAccountType,
	HWND hAccountTypeCombo, HWND hCheckByAccountLabroom,
	HWND hAccountLabroomCombo)
{
	AccountCount count;
	memset(&count, 0, sizeof(AccountCount));

	count.countByType = -1;
	count.countByRoom = -1;
	count.count = 0;

	ResourceManager* rm = GetResourceManage();
	if (!rm) return;

	// 按账户类型统计
	if (SendMessage(hCheckByAccountType, BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		count.countByType = 1;
		int selectedIndex = SendMessage(hAccountTypeCombo, CB_GETCURSEL, 0, 0);
		if (selectedIndex != CB_ERR)
		{
			// 根据下拉框选择设置账户类型
			switch (selectedIndex)
			{
			case 0: count.type = Admin; break;
			case 1: count.type = Experimenter; break;
			case 2: count.type = User; break;
			default: count.type = Unknow; break;
			}
		}
	}

	// 按实验室统计（仅针对实验员）
	if (count.type == Experimenter &&
		SendMessage(hCheckByAccountLabroom, BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		count.countByRoom = 1;
		int selectedIndex = SendMessage(hAccountLabroomCombo, CB_GETCURSEL, 0, 0);
		if (selectedIndex != CB_ERR)
		{
			wchar_t buffer[100];
			SendMessage(hAccountLabroomCombo, CB_GETLBTEXT, selectedIndex, (LPARAM)buffer);
			// 解析出实验室ID
			count.room_id = _wtoi(buffer);
		}
	}

	// 执行统计
	CountAccount(rm->account_list, &count);

	// 显示统计结果
	int itemIndex = 0;

	// 添加统计条件
	LVITEM lvi;
	wchar_t buffer[256];

	if (count.countByType >= 0)
	{
		lvi.mask = LVIF_TEXT;
		lvi.iItem = itemIndex;
		lvi.iSubItem = 0;
		lvi.pszText = L"账户类型";
		ListView_InsertItem(hListView, &lvi);

		const wchar_t* typeStr = L"";
		switch (count.type)
		{
		case Admin: typeStr = L"管理员"; break;
		case Experimenter: typeStr = L"实验员"; break;
		case User: typeStr = L"一般用户"; break;
		default: typeStr = L"未知"; break;
		}
		ListView_SetItemText(hListView, itemIndex, 1, (LPWSTR)typeStr);
		itemIndex++;
	}

	if (count.countByRoom >= 0)
	{
		LabRoom* labroom = RoomId_to_LabRoom(count.room_id);
		if (labroom)
		{
			lvi.mask = LVIF_TEXT;
			lvi.iItem = itemIndex;
			lvi.iSubItem = 0;
			lvi.pszText = L"所属实验室";
			ListView_InsertItem(hListView, &lvi);

			ListView_SetItemText(hListView, itemIndex, 1, labroom->name);
			itemIndex++;
		}
	}

	// 添加统计结果
	lvi.mask = LVIF_TEXT;
	lvi.iItem = itemIndex;
	lvi.iSubItem = 0;
	lvi.pszText = L"统计结果";
	ListView_InsertItem(hListView, &lvi);

	swprintf(buffer, 256, L"共有 %d 个账户", count.count);
	ListView_SetItemText(hListView, itemIndex, 1, buffer);
}

// 显示统计功能窗口
void ShowStatisticsWindow(HWND hWnd)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = StatisticsWndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"StatisticsWindow";

	RegisterClass(&wc);

	hwndStatistics = CreateWindow(L"StatisticsWindow", L"统计功能",
		WS_OVERLAPPED | WS_SYSMENU, 100, 50, 800, 600, hWnd, NULL, GetModuleHandle(NULL), NULL);

	ShowWindow(hwndStatistics, SW_SHOW);
	UpdateWindow(hwndStatistics);
}

