#include "category.h"
#include "function_ui.h"
#include <CommCtrl.h>

Category* CreateCategory(wchar_t* name, int disposal_years)
{
    Category* new_category = (Category*)malloc(sizeof(Category));
    if (new_category == NULL)
    {
        printf("���ഴ��ʧ��\n");
        return NULL;
    }

	wcscpy_s(new_category->name, CATEGORY_LENGTH, name);
    new_category->disposal_years = disposal_years;
    new_category->id = GetNewId(CategoryID);

    return new_category;
}

void DestoryCategory(Category* category)
{
}

void AddCategory()
{
    char name[CATEGORY_LENGTH];
    int disposal_years;
    system("cls");
    printf("---          ������豸����          ---\n\n");
    printf("--- ������������->   ");
    scanf_s("%s", name, CATEGORY_LENGTH);
    printf("--- ���뱨������->   ");
    scanf_s("%d", &disposal_years);

    Category* new_category = CreateCategory(name, disposal_years);
    if (new_category == NULL)
        return;

    LinkedList_pushback(GetResourceManage()->category_list, new_category);

    printf("�����ɹ���\n����id: %d\n��������: %s\n��������: %d ��\n", new_category->id, name, disposal_years);
    system("pause");
}

void _AddCategory(HWND hWnd)
{
	ResourceManager* rm = GetResourceManage();

	wchar_t name[50];

	HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME);
	if (GetWindowTextLength(hEdit) == 0)
	{
		MessageBox(hWnd, L"�������������֣�", L"��ʾ", MB_OK | MB_ICONWARNING);
		return;
	}
	GetDlgItemText(hWnd, IDC_EDIT_NAME, name, sizeof(name) / sizeof(wchar_t));

	hEdit = GetDlgItem(hWnd, IDC_EDIT_ROOM_ID);
	if (GetWindowTextLength(hEdit) == 0)
	{
		MessageBox(hWnd, L"���������ͱ������ޣ�", L"��ʾ", MB_OK | MB_ICONWARNING);
		return;
	}

	int year = GetInputNumber(hWnd, IDC_EDIT_ROOM_ID);

	Category* new_category = CreateCategory(name, year);

	LinkedList_pushback(rm->category_list, new_category);

	HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
	int index = ListView_GetItemCount(hListView);

	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = index;
	lvItem.iSubItem = 0;

	wchar_t idBuffer[16];
	swprintf_s(idBuffer, 16, L"%d", new_category->id);
	lvItem.pszText = idBuffer;
	ListView_InsertItem(hListView, &lvItem);

	ListView_SetItemText(hListView, index, 1, new_category->name);
	wchar_t Buffer[16];
	swprintf_s(Buffer, 16, L"%d", new_category->disposal_years);
	ListView_SetItemText(hListView, index, 2, Buffer);


	MessageBox(hWnd, L"�����ɹ���", L"��ʾ", MB_OK | MB_ICONWARNING);
}

void _DeleteCategory(HWND hWnd)
{
	HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
	int selectedIndex = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);
	if (selectedIndex == -1)
	{
		MessageBox(hWnd, L"δѡ������", L"��ʾ", MB_OK);
		return;
	}

	LabRoom* delete_category = LinkedList_at(GetResourceManage()->category_list, selectedIndex);
	wchar_t message[256];
	swprintf_s(message, sizeof(message) / sizeof(wchar_t), L"ȷ��ɾ�� %s ��", delete_category->name);

	int result = MessageBox(hWnd, message, L"ɾ��ȷ��", MB_YESNO | MB_ICONQUESTION);
	if (result == IDYES)
	{
		LinkedList_delete(GetResourceManage()->category_list, selectedIndex);
		Category* none_category = CreateCategory(L"��", 0);

		Node* temp = GetResourceManage()->equipment_list->head;
		while (temp->next)
		{
			temp = temp->next;
			ExperimentalEquipment* ee = (ExperimentalEquipment*)temp->data;
			if (ee->category->id == delete_category->id)
			{
				ee->category = none_category;
			}
		}

		LinkedList_delete(GetResourceManage()->category_list, selectedIndex);
		free(delete_category);

		HWND hwndListView = GetDlgItem(hWnd, IDC_LISTVIEW);
		ListView_DeleteItem(hwndListView, selectedIndex);

		MessageBox(hWnd, L"������ɾ��", L"ɾ��", MB_OK);
	}
	else
	{
		return;
	}
}

void _ChangeCategory(HWND hWnd)
{
	HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
	int selectedIndex = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);

	if (selectedIndex == -1)
	{
		MessageBox(hWnd, L"δѡ������", L"��ʾ", MB_OK);
		return;
	}

	int result = MessageBox(hWnd, L"ȷ��Ҫ�޸���", L"�޸�ȷ��", MB_YESNO | MB_ICONQUESTION);
	Category* Ccategory = LinkedList_at(GetResourceManage()->category_list, selectedIndex);
	if (result == IDYES)
	{
		HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_ROOM_ID_CHANGE);
		if (GetWindowTextLength(hEdit) != 0)
		{
			int year = GetInputNumber(hWnd, IDC_EDIT_ROOM_ID_CHANGE);
			Ccategory->disposal_years = year;
		}
		hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME_CHANGE);
		if (GetWindowTextLength(hEdit) != 0)
		{
			wchar_t name[50];
			GetDlgItemText(hWnd, IDC_EDIT_NAME_CHANGE, name, sizeof(name) / sizeof(wchar_t));
			wcscpy_s(Ccategory->name, CATEGORY_LENGTH, name);
		}

		wchar_t idBuffer[16];
		swprintf_s(idBuffer, 16, L"%d", Ccategory->id);
		ListView_SetItemText(hListView, selectedIndex, 0, idBuffer);

		ListView_SetItemText(hListView, selectedIndex, 1, Ccategory->name);

		wchar_t Buffer[16];
		swprintf_s(Buffer, 16, L"%d", Ccategory->disposal_years);
		ListView_SetItemText(hListView, selectedIndex, 2, Buffer);
		MessageBox(hWnd, L"�豸���޸�", L"�޸�", MB_OK);
	}
	else
	{
		return;
	}
}



Category* FindCategoryById(int id)
{
	Node* temp = GetResourceManage()->category_list->head->next;
	while (temp)
	{
		Category* category = (Category*)temp->data;
		if (category->id == id)
			return category;
		temp = temp->next;
	}
	return NULL;
}

Category* Id_to_Category(int id)
{
    Node* temp = GetResourceManage()->category_list->head;

    while (temp->next)
    {
        temp = temp->next;
        Category* category = (Category*)temp->data;
        if (id == category->id)
            return category;
    }
    return NULL;
}
