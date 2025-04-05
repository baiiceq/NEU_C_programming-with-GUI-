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
		// ��������
		CreateWindow(L"STATIC", L"ϵͳά��", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			320, 30, 160, 30, hWnd, NULL, NULL, NULL);
		// ������ʾ��ǰ�û����ľ�̬�ı��ؼ�
		wchar_t usernameText[100];
		swprintf_s(usernameText, 100, L"��ǰ�û�: %s", em->current_account->user_name);
		CreateWindow(L"STATIC", usernameText, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			320, 70, 160, 30, hWnd, NULL, NULL, NULL);
		// ��������ά����ť��˵��
		CreateWindow(L"BUTTON", L"����ά��", WS_VISIBLE | WS_CHILD,
			90, 120, 120, 40, hWnd, (HMENU)ID_BTN_PASSWORD_MAINTAIN, GetModuleHandle(NULL), NULL);
		CreateWindow(L"STATIC", L"�޸ĵ�ǰ�û�������", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			240, 120, 480, 40, hWnd, NULL, NULL, NULL);

		// ���������û����밴ť��˵��
		CreateWindow(L"BUTTON", L"�����û�����", WS_VISIBLE | WS_CHILD,
			90, 180, 120, 40, hWnd, (HMENU)ID_BTN_RESET_PASSWORD, GetModuleHandle(NULL), NULL);
		CreateWindow(L"STATIC", L"�������û��������������", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			240, 180, 480, 40, hWnd, NULL, NULL, NULL);

		// �������ݱ��ݰ�ť��˵��
		CreateWindow(L"BUTTON", L"���ݱ���", WS_VISIBLE | WS_CHILD,
			90, 360, 120, 40, hWnd, (HMENU)ID_BTN_DATA_BACKUP, GetModuleHandle(NULL), NULL);
		CreateWindow(L"STATIC", L"����ǰϵͳ���ݱ��ݱ���", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			240, 360, 480, 40, hWnd, NULL, NULL, NULL);

		// �������ݻָ���ť��˵��
		CreateWindow(L"BUTTON", L"���ݻָ�", WS_VISIBLE | WS_CHILD,
			90, 420, 120, 40, hWnd, (HMENU)ID_BTN_DATA_RESTORE, GetModuleHandle(NULL), NULL);
		CreateWindow(L"STATIC", L"�ӱ����ļ��лָ�ϵͳ����", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			240, 420, 480, 40, hWnd, NULL, NULL, NULL);

		// �����޸ĵ�ǰ�û�����ť��˵��
		CreateWindow(L"BUTTON", L"�޸��û���", WS_VISIBLE | WS_CHILD,
			90, 240, 120, 40, hWnd, (HMENU)ID_BTN_CHANGE_USERNAME, GetModuleHandle(NULL), NULL);
		CreateWindow(L"STATIC", L"�޸ĵ�ǰ�û����û���", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			240, 240, 480, 40, hWnd, NULL, NULL, NULL);

		// �����޸������û��û�����ť��˵��
		CreateWindow(L"BUTTON", L"�޸��û��û���", WS_VISIBLE | WS_CHILD,
			90, 300, 120, 40, hWnd, (HMENU)ID_BTN_CHANGE_USER_USERNAME, GetModuleHandle(NULL), NULL);
		CreateWindow(L"STATIC", L"�޸������û����û���", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			240, 300, 480, 40, hWnd, NULL, NULL, NULL);

		// �������ذ�ť��˵��
		CreateWindow(L"BUTTON", L"����", WS_VISIBLE | WS_CHILD,
			90, 480, 120, 40, hWnd, (HMENU)ID_BTN_BACK, GetModuleHandle(NULL), NULL);
		CreateWindow(L"STATIC", L"������һ���˵�", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			240, 480, 480, 40, hWnd, NULL, NULL, NULL);
		break;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_BTN_PASSWORD_MAINTAIN:
			// ��ʾ����ά������
			ShowPasswordMaintenanceWindow(hWnd);
			break;
		case ID_BTN_RESET_PASSWORD:
			// ��ʾ�����û����봰��
			ShowResetPasswordWindow(hWnd);
			break;
		case ID_BTN_DATA_BACKUP:
			// �������ݱ���
			if (MessageBox(hWnd, L"ȷ��Ҫ���ݵ�ǰϵͳ������", L"���ݱ���", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				if (BackupSystemData(em->current_account->id))
					MessageBox(hWnd, L"���ݱ��ݳɹ���", L"��ʾ", MB_OK | MB_ICONINFORMATION);
				else
					MessageBox(hWnd, L"���ݱ���ʧ�ܣ�", L"����", MB_OK | MB_ICONERROR);
			}
			break;
		case ID_BTN_DATA_RESTORE:
			// �������ݻָ�
			ShowRestoreDataWindow(hWnd);
			break;
		case ID_BTN_CHANGE_USERNAME:
			// ��ʾ�޸ĵ�ǰ�û�������
			ShowChangeUsernameWindow(hWnd);
			break;
		case ID_BTN_CHANGE_USER_USERNAME:
			// ��ʾ�޸������û��û�������
			ShowChangeUserUsernameWindow(hWnd);
			break;
		case ID_BTN_BACK:
		{
			// ������һ���˵�
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

// �޸��Լ����봰�ڹ���
LRESULT CALLBACK PasswordMaintenanceWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hOldPasswordEdit = NULL;
	static HWND hNewPasswordEdit = NULL;
	static HWND hConfirmPasswordEdit = NULL;

	switch (msg)
	{
	case WM_CREATE:
		// ��������ά������Ŀؼ�
		CreateWindow(L"STATIC", L"��ǰ����:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			100, 50, 100, 25, hWnd, NULL, NULL, NULL);
		hOldPasswordEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
			220, 50, 180, 25, hWnd, (HMENU)IDC_OLD_PASSWORD, NULL, NULL);

		CreateWindow(L"STATIC", L"������:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			100, 90, 100, 25, hWnd, NULL, NULL, NULL);
		hNewPasswordEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
			220, 90, 180, 25, hWnd, (HMENU)IDC_NEW_PASSWORD, NULL, NULL);

		CreateWindow(L"STATIC", L"ȷ��������:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			100, 130, 100, 25, hWnd, NULL, NULL, NULL);
		hConfirmPasswordEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
			220, 130, 180, 25, hWnd, (HMENU)IDC_CONFIRM_PASSWORD, NULL, NULL);

		CreateWindow(L"BUTTON", L"ȷ��", WS_VISIBLE | WS_CHILD,
			150, 180, 100, 30, hWnd, (HMENU)IDC_BTN_CONFIRM, NULL, NULL);
		CreateWindow(L"BUTTON", L"ȡ��", WS_VISIBLE | WS_CHILD,
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
				MessageBox(NULL, L"ԭ���벻��ȷ��", L"����", MB_OK | MB_ICONERROR);
				return False;
			}
			if (!IsValidPassword(newPassword)) {
				MessageBox(NULL, L"���벻���Ϲ���", L"����", MB_OK | MB_ICONERROR);
				return False;
			}
			if (wcscmp(newPassword, confirmPassword) != 0) {
				MessageBox(NULL, L"������������벻һ��", L"����", MB_OK | MB_ICONERROR);
				return False;
			}

			wcscpy_s(em->current_account->user_password, USER_PASSWORD_LENGTH, newPassword);
			MessageBox(NULL, L"�����޸ĳɹ�", L"��ʾ", MB_OK);
			DestroyWindow(hWnd);

			/*wchar_t operation[20];
			switch (account->account_type)
			{
			case Admin:
				wcscpy_s(operation, 20, L"�޸Ĺ���Ա����");
				break;
			case User:
				wcscpy_s(operation, 20, L"�޸�һ���û�����");
				break;
			case Experimenter:
				wcscpy_s(operation, 20, L"�޸�ʵ��Ա����");
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

// �����û����봰�ڹ���
LRESULT CALLBACK ResetPasswordWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hUserListView = NULL;
	static HWND hNewPasswordEdit = NULL;

	switch (msg)
	{
	case WM_CREATE:
	{
		CreateWindow(L"STATIC", L"�û��б�:", WS_VISIBLE | WS_CHILD | SS_LEFT,
			50, 30, 100, 20, hWnd, NULL, NULL, NULL);

		// �����û��б���ͼ
		hUserListView = CreateWindowEx(0, WC_LISTVIEW, NULL,
			WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_SINGLESEL,
			50, 60, 500, 200, hWnd, (HMENU)IDC_USER_LISTVIEW, GetModuleHandle(NULL), NULL);

		// �����
		LVCOLUMNW lvc;
		lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

		lvc.pszText = L"�û�ID";
		lvc.cx = 80;
		lvc.iSubItem = 0;
		ListView_InsertColumn(hUserListView, 0, &lvc);

		lvc.pszText = L"�û���";
		lvc.cx = 120;
		lvc.iSubItem = 1;
		ListView_InsertColumn(hUserListView, 1, &lvc);

		lvc.pszText = L"�û�����";
		lvc.cx = 120;
		lvc.iSubItem = 2;
		ListView_InsertColumn(hUserListView, 2, &lvc);

		lvc.pszText = L"������ID";
		lvc.cx = 100;
		lvc.iSubItem = 3;
		ListView_InsertColumn(hUserListView, 3, &lvc);

		// ����û��б�����
		FillUserList(hUserListView);

		CreateWindow(L"STATIC", L"������:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			150, 280, 80, 25, hWnd, NULL, NULL, NULL);
		hNewPasswordEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
			250, 280, 180, 25, hWnd, (HMENU)IDC_NEW_PASSWORD, NULL, NULL);

		CreateWindow(L"BUTTON", L"��������", WS_VISIBLE | WS_CHILD,
			180, 330, 120, 30, hWnd, (HMENU)IDC_BTN_RESET, NULL, NULL);
		CreateWindow(L"BUTTON", L"ȡ��", WS_VISIBLE | WS_CHILD,
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
				MessageBox(hWnd, L"��ѡ��һ���û���", L"��ʾ", MB_OK | MB_ICONINFORMATION);
				break;
			}

			wchar_t userIdText[16];
			ListView_GetItemText(hUserListView, selectedIndex, 0, userIdText, 16);
			int userId = _wtoi(userIdText);

			wchar_t newPassword[50];
			GetWindowText(hNewPasswordEdit, newPassword, 50);
			if (wcslen(newPassword) == 0)
			{
				MessageBox(hWnd, L"�����������룡", L"��ʾ", MB_OK | MB_ICONINFORMATION);
				break;
			}

			if (ResetUserPassword(userId, newPassword))
			{
				MessageBox(hWnd, L"�������óɹ���", L"��ʾ", MB_OK | MB_ICONINFORMATION);
				DestroyWindow(hWnd);
			}
			else
			{
				MessageBox(hWnd, L"��������ʧ�ܣ�", L"����", MB_OK | MB_ICONERROR);
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

//�޸��Լ��û������ڹ���
LRESULT CALLBACK ChangeUsernameWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hNewUsernameEdit = NULL;

	switch (msg)
	{
	case WM_CREATE:
		// �����޸��û�������Ŀؼ�
		CreateWindow(L"STATIC", L"���û���:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			100, 50, 100, 25, hWnd, NULL, NULL, NULL);
		hNewUsernameEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
			220, 50, 180, 25, hWnd, (HMENU)IDC_NEW_USERNAME, NULL, NULL);

		CreateWindow(L"BUTTON", L"ȷ��", WS_VISIBLE | WS_CHILD,
			150, 100, 100, 30, hWnd, (HMENU)IDC_BTN_CONFIRM, NULL, NULL);
		CreateWindow(L"BUTTON", L"ȡ��", WS_VISIBLE | WS_CHILD,
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
				MessageBox(hWnd, L"���������û�����", L"��ʾ", MB_OK | MB_ICONINFORMATION);
				break;
			}

			wcscpy_s(em->current_account->user_name, USER_NMAE_LENGTH, newUsername);
			MessageBox(hWnd, L"�û����޸ĳɹ���", L"��ʾ", MB_OK | MB_ICONINFORMATION);
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

//�޸������û��û������ڹ���
LRESULT CALLBACK ChangeUserUsernameWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hUserListView = NULL;
	static HWND hNewUsernameEdit = NULL;

	switch (msg)
	{
	case WM_CREATE:
	{
		CreateWindow(L"STATIC", L"�û��б�:", WS_VISIBLE | WS_CHILD | SS_LEFT,
			50, 30, 100, 20, hWnd, NULL, NULL, NULL);

		// �����û��б���ͼ
		hUserListView = CreateWindowEx(0, WC_LISTVIEW, NULL,
			WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_SINGLESEL,
			50, 60, 500, 200, hWnd, (HMENU)IDC_USER_LISTVIEW, GetModuleHandle(NULL), NULL);

		// �����
		LVCOLUMNW lvc;
		lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

		lvc.pszText = L"�û�ID";
		lvc.cx = 80;
		lvc.iSubItem = 0;
		ListView_InsertColumn(hUserListView, 0, &lvc);

		lvc.pszText = L"�û���";
		lvc.cx = 120;
		lvc.iSubItem = 1;
		ListView_InsertColumn(hUserListView, 1, &lvc);

		lvc.pszText = L"�û�����";
		lvc.cx = 120;
		lvc.iSubItem = 2;
		ListView_InsertColumn(hUserListView, 2, &lvc);

		lvc.pszText = L"������ID";
		lvc.cx = 100;
		lvc.iSubItem = 3;
		ListView_InsertColumn(hUserListView, 3, &lvc);

		// ����û��б�����
		FillUserList(hUserListView);

		CreateWindow(L"STATIC", L"���û���:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			150, 280, 80, 25, hWnd, NULL, NULL, NULL);
		hNewUsernameEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
			250, 280, 180, 25, hWnd, (HMENU)IDC_NEW_USERNAME, NULL, NULL);

		CreateWindow(L"BUTTON", L"�޸��û���", WS_VISIBLE | WS_CHILD,
			180, 330, 120, 30, hWnd, (HMENU)IDC_BTN_CHANGE_USERNAME, NULL, NULL);
		CreateWindow(L"BUTTON", L"ȡ��", WS_VISIBLE | WS_CHILD,
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
				MessageBox(hWnd, L"��ѡ��һ���û���", L"��ʾ", MB_OK | MB_ICONINFORMATION);
				break;
			}

			wchar_t userIdText[16];
			ListView_GetItemText(hUserListView, selectedIndex, 0, userIdText, 16);
			int userId = _wtoi(userIdText);

			wchar_t newUsername[50];
			GetWindowText(hNewUsernameEdit, newUsername, 50);
			if (wcslen(newUsername) == 0)
			{
				MessageBox(hWnd, L"���������û�����", L"��ʾ", MB_OK | MB_ICONINFORMATION);
				break;
			}

			Account* account = FindById(userId);
			if (account == NULL)
				break;
			wcscpy_s(account->user_name, USER_NMAE_LENGTH, newUsername);
			MessageBox(hWnd, L"�û����޸ĳɹ���", L"��ʾ", MB_OK | MB_ICONINFORMATION);
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

//���ݻָ�����
LRESULT CALLBACK RestoreDataWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hRestoreTimeEdit = NULL;

	switch (msg)
	{
	case WM_CREATE:
		// �������ݻָ�����Ŀؼ�
		CreateWindow(L"STATIC", L"�ָ��ڵ�ʱ��(�����ļ��е�����)", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			140, 20, 280, 25, hWnd, NULL, NULL, NULL);
		hRestoreTimeEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
			170, 50, 180, 25, hWnd, (HMENU)IDC_RESTORE_TIME, NULL, NULL);

		CreateWindow(L"BUTTON", L"ȷ��", WS_VISIBLE | WS_CHILD,
			150, 100, 100, 30, hWnd, (HMENU)IDC_BTN_CONFIRM, NULL, NULL);
		CreateWindow(L"BUTTON", L"ȡ��", WS_VISIBLE | WS_CHILD,
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
				MessageBox(hWnd, L"������ָ��ڵ�ʱ�䣡", L"��ʾ", MB_OK | MB_ICONINFORMATION);
				break;
			}

			if (RestoreSystemData(em->current_account->id, restoreTime))
			{
				MessageBox(hWnd, L"���ݻָ��ɹ���", L"��ʾ", MB_OK | MB_ICONINFORMATION);
				DestroyWindow(hWnd);
			}
			else
			{
				MessageBox(hWnd, L"���ݻָ�ʧ�ܣ�", L"����", MB_OK | MB_ICONERROR);
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

	hwndSystemMaintenance = CreateWindow(L"SystemMaintenanceWindow", L"ϵͳά��",
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

	HWND hwndPasswordMaintenance = CreateWindow(L"PasswordMaintenanceWindow", L"����ά��",
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

	HWND hwndResetPassword = CreateWindow(L"ResetPasswordWindow", L"�����û�����",
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

	HWND hwndChangeUsername = CreateWindow(L"ChangeUsernameWindow", L"�޸��û���",
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

	HWND hwndChangeUserUsername = CreateWindow(L"ChangeUserUsernameWindow", L"�޸��û��û���",
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

	HWND hwndRestoreData = CreateWindow(L"RestoreDataWindow", L"���ݻָ�",
		WS_OVERLAPPED | WS_SYSMENU, 300, 200, 500, 200, hWnd, NULL, GetModuleHandle(NULL), NULL);

	ShowWindow(hwndRestoreData, SW_SHOW);
	UpdateWindow(hwndRestoreData);
}


// ����û��б�
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

		// �û�ID
		wchar_t idBuffer[16];
		swprintf_s(idBuffer, 16, L"%d", account->id);
		lvItem.pszText = idBuffer;
		ListView_InsertItem(hListView, &lvItem);

		// �û���
		ListView_SetItemText(hListView, i, 1, account->user_name);

		// �û�����
		wchar_t* typeText;
		switch (account->account_type)
		{
		case Admin:
			typeText = L"����Ա";
			break;
		case Experimenter:
			typeText = L"ʵ��Ա";
			break;
		case User:
			typeText = L"��ͨ�û�";
			break;
		default:
			typeText = L"δ֪";
			break;
		}
		ListView_SetItemText(hListView, i, 2, typeText);

		// ������ID
		wchar_t roomBuffer[16];
		swprintf_s(roomBuffer, 16, L"%d", account->roomid);
		ListView_SetItemText(hListView, i, 3, roomBuffer);
	}
}

// ����ָ���û�������
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

// ͳ�ƹ��ܴ��ڹ���
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
		// ��������
		CreateWindow(L"STATIC", L"ͳ�ƹ���", WS_VISIBLE | WS_CHILD | SS_CENTER,
			300, 10, 200, 30, hWnd, NULL, NULL, NULL);

		// ��������ѡ��ѡ��ť
		hRadioEquipment = CreateWindow(L"BUTTON", L"�豸ͳ��", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP,
			50, 50, 100, 30, hWnd, (HMENU)IDC_RADIO_STATS_EQUIPMENT, NULL, NULL);
		hRadioAccount = CreateWindow(L"BUTTON", L"�˻�ͳ��", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
			200, 50, 100, 30, hWnd, (HMENU)IDC_RADIO_STATS_ACCOUNT, NULL, NULL);

		// Ĭ��ѡ���豸ͳ��
		SendMessage(hRadioEquipment, BM_SETCHECK, BST_CHECKED, 0);

		// �����豸ͳ�Ʒ����
		hwndEquipmentGroup = CreateWindow(L"BUTTON", L"�豸ͳ��ѡ��", WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
			50, 90, 700, 200, hWnd, NULL, NULL, NULL);

		// �������豸����ͳ��ѡ��
		hCheckByCategory = CreateWindow(L"BUTTON", L"���豸����ͳ��", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			70, 120, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_CATEGORY, NULL, NULL);

		CreateWindow(L"STATIC", L"�豸����:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
			220, 120, 80, 25, hWnd, NULL, NULL, NULL);

		hCategoryCombo = CreateWindow(L"COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
			310, 120, 120, 200, hWnd, (HMENU)IDC_CATEGORY_COMBO, NULL, NULL);

		// ����豸����������
		FillCategoryComboBox(hCategoryCombo);

		// ������ʵ����ͳ��ѡ��
		hCheckByLabroom = CreateWindow(L"BUTTON", L"��ʵ����ͳ��", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			70, 150, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_LABROOM, NULL, NULL);

		CreateWindow(L"STATIC", L"ʵ����:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
			220, 150, 80, 25, hWnd, NULL, NULL, NULL);

		hLabroomCombo = CreateWindow(L"COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
			310, 150, 120, 200, hWnd, (HMENU)IDC_LABROOM_COMBO, NULL, NULL);

		// ���ʵ����������
		FillLabroomComboBox(hLabroomCombo);

		// �������۸�ͳ��ѡ��
		hCheckByPrice = CreateWindow(L"BUTTON", L"���۸�ͳ��", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			70, 180, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_PRICE, NULL, NULL);

		CreateWindow(L"STATIC", L"��ͼ۸�:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
			220, 180, 80, 25, hWnd, NULL, NULL, NULL);

		hEditMinPrice = CreateWindow(L"EDIT", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
			310, 180, 80, 25, hWnd, (HMENU)IDC_EDIT_MIN_PRICE, NULL, NULL);

		CreateWindow(L"STATIC", L"��߼۸�:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
			400, 180, 80, 25, hWnd, NULL, NULL, NULL);

		hEditMaxPrice = CreateWindow(L"EDIT", L"10000", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
			490, 180, 80, 25, hWnd, (HMENU)IDC_EDIT_MAX_PRICE, NULL, NULL);

		// ����������ͳ��ѡ��
		hCheckByDate = CreateWindow(L"BUTTON", L"������ͳ��", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			70, 210, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_DATE, NULL, NULL);

		CreateWindow(L"STATIC", L"��ʼ����:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
			220, 210, 80, 25, hWnd, NULL, NULL, NULL);

		hEditStartDate = CreateWindow(L"EDIT", L"20200101", WS_VISIBLE | WS_CHILD | WS_BORDER,
			310, 210, 80, 25, hWnd, (HMENU)IDC_EDIT_START_DATE, NULL, NULL);

		CreateWindow(L"STATIC", L"��������:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
			400, 210, 80, 25, hWnd, NULL, NULL, NULL);

		hEditEndDate = CreateWindow(L"EDIT", L"20240101", WS_VISIBLE | WS_CHILD | WS_BORDER,
			490, 210, 80, 25, hWnd, (HMENU)IDC_EDIT_END_DATE, NULL, NULL);

		// �����˻�ͳ�Ʒ���򣨳�ʼ���أ�
		hwndAccountGroup = CreateWindow(L"BUTTON", L"�˻�ͳ��ѡ��", WS_CHILD | BS_GROUPBOX,
			50, 90, 700, 200, hWnd, NULL, NULL, NULL);

		// �������˻�����ͳ��ѡ��
		hCheckByAccountType = CreateWindow(L"BUTTON", L"���˻�����ͳ��", WS_CHILD | BS_AUTOCHECKBOX,
			70, 120, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_ACCOUNT_TYPE, NULL, NULL);

		CreateWindow(L"STATIC", L"�˻�����:", WS_CHILD | SS_RIGHT,
			220, 120, 80, 25, hWnd, NULL, NULL, NULL);

		hAccountTypeCombo = CreateWindow(L"COMBOBOX", NULL, WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
			310, 120, 150, 200, hWnd, (HMENU)IDC_ACCOUNT_TYPE_COMBO, NULL, NULL);

		// ����˻�����������
		SendMessage(hAccountTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"����Ա");
		SendMessage(hAccountTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"ʵ��Ա");
		SendMessage(hAccountTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"һ���û�");
		SendMessage(hAccountTypeCombo, CB_SETCURSEL, 0, 0);

		// ������ʵ����ͳ��ѡ������ʵ��Ա��
		hCheckByAccountLabroom = CreateWindow(L"BUTTON", L"��ʵ����ͳ�ƣ���ʵ��Ա��", WS_CHILD | BS_AUTOCHECKBOX,
			70, 150, 200, 25, hWnd, (HMENU)IDC_CHECK_BY_ACCOUNT_LABROOM, NULL, NULL);

		CreateWindow(L"STATIC", L"ʵ����:", WS_CHILD | SS_RIGHT,
			280, 150, 60, 25, hWnd, NULL, NULL, NULL);

		hAccountLabroomCombo = CreateWindow(L"COMBOBOX", NULL, WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
			350, 150, 120, 200, hWnd, (HMENU)IDC_ACCOUNT_LABROOM_COMBO, NULL, NULL);

		// ���ʵ�������������豸ͳ�ƹ�����ͬ���ݣ�
		FillLabroomComboBox(hAccountLabroomCombo);

		// ����ͳ�ƽ���б���ͼ
		hResultListView = CreateWindowEx(0, WC_LISTVIEW, NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT,
			50, 300, 700, 200, hWnd, (HMENU)IDC_STATS_RESULT_LIST, NULL, NULL);

		// �����
		LVCOLUMN lvc;
		lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

		lvc.pszText = L"ͳ����Ŀ";
		lvc.cx = 200;
		lvc.iSubItem = 0;
		ListView_InsertColumn(hResultListView, 0, &lvc);

		lvc.pszText = L"ͳ�ƽ��";
		lvc.cx = 470;
		lvc.iSubItem = 1;
		ListView_InsertColumn(hResultListView, 1, &lvc);

		// ����ִ��ͳ�ư�ť
		CreateWindow(L"BUTTON", L"ִ��ͳ��", WS_VISIBLE | WS_CHILD,
			250, 520, 100, 30, hWnd, (HMENU)IDC_BTN_STATS_EXECUTE, NULL, NULL);

		// �������ذ�ť
		CreateWindow(L"BUTTON", L"����", WS_VISIBLE | WS_CHILD,
			450, 520, 100, 30, hWnd, (HMENU)IDC_BTN_STATS_BACK, NULL, NULL);

		break;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RADIO_STATS_EQUIPMENT:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				// ��ʾ�豸ͳ��ѡ������˻�ͳ��ѡ��
				ShowWindow(hwndEquipmentGroup, SW_SHOW);
				ShowWindow(hwndAccountGroup, SW_HIDE);

				// ��ʾ�豸ͳ�Ƶ������ӿؼ�
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

				// �����˻�ͳ�Ƶ������ӿؼ�
				ShowWindow(hCheckByAccountType, SW_HIDE);
				ShowWindow(hAccountTypeCombo, SW_HIDE);
				ShowWindow(hCheckByAccountLabroom, SW_HIDE);
				ShowWindow(hAccountLabroomCombo, SW_HIDE);
			}
			break;

		case IDC_RADIO_STATS_ACCOUNT:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				// ��ʾ�˻�ͳ��ѡ������豸ͳ��ѡ��
				ShowWindow(hwndEquipmentGroup, SW_HIDE);
				ShowWindow(hwndAccountGroup, SW_SHOW);

				// �����豸ͳ�Ƶ������ӿؼ�
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

				// ��ʾ�˻�ͳ�Ƶ������ӿؼ�
				ShowWindow(hCheckByAccountType, SW_SHOW);
				ShowWindow(hAccountTypeCombo, SW_SHOW);
				ShowWindow(hCheckByAccountLabroom, SW_SHOW);
				ShowWindow(hAccountLabroomCombo, SW_SHOW);
			}
			break;

		case IDC_BTN_STATS_EXECUTE:
			// ��ս���б�
			ListView_DeleteAllItems(hResultListView);

			// �ж����豸ͳ�ƻ����˻�ͳ��
			if (SendMessage(hRadioEquipment, BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				// �豸ͳ��
				ExecuteEquipmentStatistics(hWnd, hResultListView, hCheckByCategory, hCategoryCombo,
					hCheckByLabroom, hLabroomCombo, hCheckByPrice,
					hEditMinPrice, hEditMaxPrice, hCheckByDate,
					hEditStartDate, hEditEndDate);
			}
			else
			{
				// �˻�ͳ��
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

// ����豸���������
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

// ���ʵ����������
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

// ִ���豸ͳ��
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

	// ���豸����ͳ��
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

	// ��ʵ����ͳ��
	if (SendMessage(hCheckByLabroom, BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		count.countByRoom = 1;
		int selectedIndex = SendMessage(hLabroomCombo, CB_GETCURSEL, 0, 0);
		if (selectedIndex != CB_ERR)
		{
			wchar_t buffer[100];
			SendMessage(hLabroomCombo, CB_GETLBTEXT, selectedIndex, (LPARAM)buffer);
			// ������ʵ����ID
			count.room_id = _wtoi(buffer);
		}
	}

	// ���۸�ͳ��
	if (SendMessage(hCheckByPrice, BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		count.countByPrice = 1;
		wchar_t minPrice[32], maxPrice[32];
		GetWindowText(hEditMinPrice, minPrice, 32);
		GetWindowText(hEditMaxPrice, maxPrice, 32);
		count.min_price = _wtoi(minPrice);
		count.max_price = _wtoi(maxPrice);
	}

	// ������ͳ��
	if (SendMessage(hCheckByDate, BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		count.countByDate = 1;
		wchar_t startDate[DATE_LENGTH], endDate[DATE_LENGTH];
		GetWindowText(hEditStartDate, startDate, DATE_LENGTH);
		GetWindowText(hEditEndDate, endDate, DATE_LENGTH);

		// ת�������ַ���
		char mbStartDate[DATE_LENGTH], mbEndDate[DATE_LENGTH];
		wcstombs_s(NULL, mbStartDate, DATE_LENGTH, startDate, _TRUNCATE);
		wcstombs_s(NULL, mbEndDate, DATE_LENGTH, endDate, _TRUNCATE);

		strcpy_s(count.startDate, DATE_LENGTH, mbStartDate);
		strcpy_s(count.endDate, DATE_LENGTH, mbEndDate);
	}

	// ִ��ͳ��
	CountEquipment(rm->equipment_list, &count);

	// ��ʾͳ�ƽ��
	int itemIndex = 0;

	// ���ͳ������
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
			lvi.pszText = L"�豸����";
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
			lvi.pszText = L"����ʵ����";
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
		lvi.pszText = L"�۸�Χ";
		ListView_InsertItem(hListView, &lvi);

		swprintf(buffer, 256, L"%d Ԫ - %d Ԫ", count.min_price, count.max_price);
		ListView_SetItemText(hListView, itemIndex, 1, buffer);
		itemIndex++;
	}

	if (count.countByDate >= 0)
	{
		lvi.mask = LVIF_TEXT;
		lvi.iItem = itemIndex;
		lvi.iSubItem = 0;
		lvi.pszText = L"���ڷ�Χ";
		ListView_InsertItem(hListView, &lvi);

		wchar_t startDate[DATE_LENGTH], endDate[DATE_LENGTH];
		mbstowcs_s(NULL, startDate, DATE_LENGTH, count.startDate, _TRUNCATE);
		mbstowcs_s(NULL, endDate, DATE_LENGTH, count.endDate, _TRUNCATE);

		swprintf(buffer, 256, L"%s - %s", startDate, endDate);
		ListView_SetItemText(hListView, itemIndex, 1, buffer);
		itemIndex++;
	}

	// ���ͳ�ƽ��
	lvi.mask = LVIF_TEXT;
	lvi.iItem = itemIndex;
	lvi.iSubItem = 0;
	lvi.pszText = L"ͳ�ƽ��";
	ListView_InsertItem(hListView, &lvi);

	swprintf(buffer, 256, L"���� %d ���豸", count.count);
	ListView_SetItemText(hListView, itemIndex, 1, buffer);
}

// ִ���˻�ͳ��
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

	// ���˻�����ͳ��
	if (SendMessage(hCheckByAccountType, BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		count.countByType = 1;
		int selectedIndex = SendMessage(hAccountTypeCombo, CB_GETCURSEL, 0, 0);
		if (selectedIndex != CB_ERR)
		{
			// ����������ѡ�������˻�����
			switch (selectedIndex)
			{
			case 0: count.type = Admin; break;
			case 1: count.type = Experimenter; break;
			case 2: count.type = User; break;
			default: count.type = Unknow; break;
			}
		}
	}

	// ��ʵ����ͳ�ƣ������ʵ��Ա��
	if (count.type == Experimenter &&
		SendMessage(hCheckByAccountLabroom, BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		count.countByRoom = 1;
		int selectedIndex = SendMessage(hAccountLabroomCombo, CB_GETCURSEL, 0, 0);
		if (selectedIndex != CB_ERR)
		{
			wchar_t buffer[100];
			SendMessage(hAccountLabroomCombo, CB_GETLBTEXT, selectedIndex, (LPARAM)buffer);
			// ������ʵ����ID
			count.room_id = _wtoi(buffer);
		}
	}

	// ִ��ͳ��
	CountAccount(rm->account_list, &count);

	// ��ʾͳ�ƽ��
	int itemIndex = 0;

	// ���ͳ������
	LVITEM lvi;
	wchar_t buffer[256];

	if (count.countByType >= 0)
	{
		lvi.mask = LVIF_TEXT;
		lvi.iItem = itemIndex;
		lvi.iSubItem = 0;
		lvi.pszText = L"�˻�����";
		ListView_InsertItem(hListView, &lvi);

		const wchar_t* typeStr = L"";
		switch (count.type)
		{
		case Admin: typeStr = L"����Ա"; break;
		case Experimenter: typeStr = L"ʵ��Ա"; break;
		case User: typeStr = L"һ���û�"; break;
		default: typeStr = L"δ֪"; break;
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
			lvi.pszText = L"����ʵ����";
			ListView_InsertItem(hListView, &lvi);

			ListView_SetItemText(hListView, itemIndex, 1, labroom->name);
			itemIndex++;
		}
	}

	// ���ͳ�ƽ��
	lvi.mask = LVIF_TEXT;
	lvi.iItem = itemIndex;
	lvi.iSubItem = 0;
	lvi.pszText = L"ͳ�ƽ��";
	ListView_InsertItem(hListView, &lvi);

	swprintf(buffer, 256, L"���� %d ���˻�", count.count);
	ListView_SetItemText(hListView, itemIndex, 1, buffer);
}

// ��ʾͳ�ƹ��ܴ���
void ShowStatisticsWindow(HWND hWnd)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = StatisticsWndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"StatisticsWindow";

	RegisterClass(&wc);

	hwndStatistics = CreateWindow(L"StatisticsWindow", L"ͳ�ƹ���",
		WS_OVERLAPPED | WS_SYSMENU, 100, 50, 800, 600, hWnd, NULL, GetModuleHandle(NULL), NULL);

	ShowWindow(hwndStatistics, SW_SHOW);
	UpdateWindow(hwndStatistics);
}

