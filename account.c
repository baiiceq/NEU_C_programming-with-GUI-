#include <ctype.h>
#include <string.h>
#include "resource_manager.h"
#include "account.h"
#include "function_ui.h"
#include <CommCtrl.h>

Account* CreateAccount()
{
	Account* new_account = (Account*)malloc(sizeof(Account));
	if (new_account == NULL)
	{
		printf("账户创建失败！\n");
		return NULL;
	}
	return new_account;
}

void DestoryAccount(Account* account)
{
	free(account);
}

bool ValidAccount(Account* account)
{
	if (!IsValidUsername(account->user_name))
	{
		printf("用户名不合法\n");
		return False;
	}
	if (FindByUsername(account->user_name))
	{
		printf("用户名已存在\n");
		return False;
	}
	if (!IsValidPassword(account->user_password))
	{
		printf("密码不合法\n");
		return False;
	}
	return True;
}

bool IsValidUsername(wchar_t* username)
{
	int count = 0;
	while (*username)
	{
		if (!iswalpha(*username) && !iswdigit(*username))
			return False;
		count++;
		if (count > 12)
			return False;
		username++;
	}
	if (count < 4)
		return False;
	return 1;
}

Account* FindByUsername(wchar_t* username)
{
	ResourceManager* resource_manager = GetResourceManage();

	Node* temp = resource_manager->account_list->head->next;
	while (temp)
	{
		Account* account = (Account*)temp->data;
		if (wcscmp(account->user_name, username) == 0)
			return account;
		temp = temp->next;
	}	
	return NULL;
}
Account* FindById(int id)
{
	ResourceManager* resource_manager = GetResourceManage();
	Node* temp = resource_manager->account_list->head->next;
	while (temp)
	{
		Account* account = (Account*)temp->data;
		if (account->id == id)
			return account;
		temp = temp->next;
	}
	return NULL;
}

void _AddAccount(HWND hWnd)
{
	ResourceManager* rm = GetResourceManage();

	wchar_t name[50], password[50];
	int room_id = 0;
	HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_ROOM_ID);

	if (GetWindowTextLength(hEdit) == 0)
	{
		MessageBox(hWnd, L"请输入实验室ID！", L"提示", MB_OK | MB_ICONWARNING);
		return;
	}
	if (!IsEditTextInteger(hEdit))
	{
		MessageBox(hWnd, L"实验室ID应该是一个整数！", L"提示", MB_OK | MB_ICONWARNING);
		return;
	}
	hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME);
	if (GetWindowTextLength(hEdit) == 0)
	{
		MessageBox(hWnd, L"请输入账户名！", L"提示", MB_OK | MB_ICONWARNING);
		return;
	}
	hEdit = GetDlgItem(hWnd, IDC_EDIT_PASSWORD);
	if (GetWindowTextLength(hEdit) == 0)
	{
		MessageBox(hWnd, L"请输入密码！", L"提示", MB_OK | MB_ICONWARNING);
		return;
	}

	room_id = GetInputNumber(hWnd, IDC_EDIT_ROOM_ID);

	GetDlgItemText(hWnd, IDC_EDIT_NAME, name, sizeof(name) / sizeof(wchar_t));
	GetDlgItemText(hWnd, IDC_EDIT_PASSWORD, password, sizeof(password) / sizeof(wchar_t));

	LabRoom* labroom = RoomId_to_LabRoom(room_id);
	if (labroom == NULL)
	{
		MessageBox(hWnd, L"实验室不存在！", L"提示", MB_OK | MB_ICONWARNING);
		return;
	}
	
	Account* new_account = CreateAccount();
	wcscpy_s(new_account->user_name, 50, name);
	wcscpy_s(new_account->user_password, 50, password);
	new_account->account_type = Experimenter;
	new_account->id = GetNewId(AccountID);
	new_account->roomid = room_id;
	new_account->equipment_id_list = CreateLinkedList();

	LinkedList_pushback(labroom->technician_id_list, &new_account->id);
	LinkedList_pushback(rm->account_list, new_account);

	HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
	int index = ListView_GetItemCount(hListView);

	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = index;
	lvItem.iSubItem = 0;

	wchar_t idBuffer[16];
	swprintf_s(idBuffer, 16, L"%d", new_account->id);
	lvItem.pszText = idBuffer;
	ListView_InsertItem(hListView, &lvItem);

	ListView_SetItemText(hListView, index, 1, new_account->user_name);

	ListView_SetItemText(hListView, index, 2, new_account->user_password);

	wchar_t roomBuffer[16];
	swprintf_s(roomBuffer, 16, L"%d", new_account->roomid);
	ListView_SetItemText(hListView, index, 3, roomBuffer);

	MessageBox(hWnd, L"成功添加实验员", L"提示", MB_OK);
}

void _ChangeAccount(HWND hWnd)
{
	HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
	int selectedIndex = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);

	if (selectedIndex == -1)
	{
		MessageBox(hWnd, L"未选择实验员", L"提示", MB_OK);
		return;
	}

	wchar_t idBuffer[16];
	LVITEM lvItem;
	lvItem.iSubItem = 0;
	lvItem.pszText = idBuffer;
	lvItem.cchTextMax = 16;

	ListView_GetItemText(hListView, selectedIndex, 0, idBuffer, 16);


	int result = MessageBox(hWnd, L"确定要修改吗？", L"修改确认", MB_YESNO | MB_ICONQUESTION);
	Account* change_account = FindById(wcstol(idBuffer, NULL, 16));
	if (result == IDYES)
	{
		HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_ROOM_ID_CHANGE);
		if (GetWindowTextLength(hEdit) != 0)
		{
			if (!IsEditTextInteger(hEdit))
			{
				MessageBox(hWnd, L"实验室ID应该是一个整数！", L"提示", MB_OK | MB_ICONWARNING);
				return;
			}
			int room_id = GetInputNumber(hWnd, IDC_EDIT_ROOM_ID_CHANGE);
			LabRoom* labroom = RoomId_to_LabRoom(room_id);
			if (labroom == NULL)
			{
				MessageBox(hWnd, L"实验室不存在！", L"提示", MB_OK | MB_ICONWARNING);
				return;
			}
			Change_roomid(change_account, room_id);
		}
		hEdit = GetDlgItem(hWnd, IDC_EDIT_PASSWORD_CHANGE);
		if (GetWindowTextLength(hEdit) != 0)
		{
			wchar_t password[50];
			GetDlgItemText(hWnd, IDC_EDIT_NAME_CHANGE, password, sizeof(password) / sizeof(wchar_t));
			wcscpy_s(change_account->user_password, 50, password);
		}
		hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME_CHANGE);
		if (GetWindowTextLength(hEdit) != 0)
		{
			wchar_t name[50];
			GetDlgItemText(hWnd, IDC_EDIT_NAME_CHANGE, name, sizeof(name) / sizeof(wchar_t));
			wcscpy_s(change_account->user_name, 50, name);

		}

		wchar_t idBuffer[16];
		swprintf_s(idBuffer, 16, L"%d", change_account->id);
		ListView_SetItemText(hListView, selectedIndex, 0, idBuffer);

		ListView_SetItemText(hListView, selectedIndex, 1, change_account->user_name);

		ListView_SetItemText(hListView, selectedIndex, 2, change_account->user_password);

		wchar_t roomBuffer[16];
		swprintf_s(roomBuffer, 16, L"%d", change_account->roomid);
		ListView_SetItemText(hListView, selectedIndex, 3, roomBuffer);
		MessageBox(hWnd, L"实验员账户已修改", L"修改", MB_OK);
	}
	else
	{
		return;
	}
}

void _DeleteAccount(HWND hWnd)
{
	HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
	int selectedIndex = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);

	if (selectedIndex == -1)
	{
		MessageBox(hWnd, L"未选择实验员", L"提示", MB_OK);
		return;
	}

	wchar_t idBuffer[16];
	LVITEM lvItem;
	lvItem.iSubItem = 0;
	lvItem.pszText = idBuffer;
	lvItem.cchTextMax = 16;

	ListView_GetItemText(hListView, selectedIndex, 0, idBuffer, 16);
	Account* delete_account = FindById(wcstol(idBuffer, NULL, 16));

	wchar_t message[256];
	swprintf_s(message, sizeof(message) / sizeof(wchar_t), L"确定删除 %s 吗？", delete_account->user_name);

	int result = MessageBox(hWnd, message, L"删除确认", MB_YESNO | MB_ICONQUESTION);
	if (result == IDYES)
	{
		LinkedList_delete(GetResourceManage()->equipment_list, selectedIndex);

		HWND hwndListView = GetDlgItem(hWnd, IDC_LISTVIEW);
		ListView_DeleteItem(hwndListView, selectedIndex);
		if (delete_account->roomid != -1)
		{
			LabRoom* labroom = RoomId_to_LabRoom(delete_account->roomid);
			DeleteTechnician(labroom, delete_account->id);
		}
		DestoryAccount(delete_account);

		MessageBox(hWnd, L"设备已删除", L"删除", MB_OK);
	}
	else
	{
		return;
	}
}

bool IsValidPassword(wchar_t* password)
{
	int count = 0;
	while (*password)
	{
		if (!(iswalnum(*password) || *password == L'@' || *password == L'+' || *password == L'?'))
			return False;
		count++;
		if (count > 20)
			return False;
		password++;
	}
	if (count <= 8)
		return False;
	return True;
}

bool IsCorrectAccount(Account* account)
{
	Account* temp = FindByUsername(account->user_name);
	if (temp == NULL)
	{
		printf("用户名不存在\n");
		return False;
	}
	if (wcscmp(temp->user_password, account->user_password) != 0)
	{
		printf("密码错误\n");
		return False;
	}

	account->account_type = temp->account_type;
	return True;
}

void Change_roomid(Account* account, int id)
{
	if (account->account_type != Experimenter)
		return;

	LabRoom* labroom = RoomId_to_LabRoom(id);
	if (!labroom)
		return;
	AddTechnician(labroom, account->id);
}
