#include "experimental_equipment.h"
#include "lab_room.h"
#include "function_ui.h"
#include <CommCtrl.h>


ExperimentalEquipment* CreateExperimentalEquipment(Category* category, wchar_t* name, int room_id, int price, wchar_t* purchase_date)
{
    ExperimentalEquipment* ee = (ExperimentalEquipment*)malloc(sizeof(ExperimentalEquipment));

    if (ee == NULL)
    {
        printf("�����豸ʧ��\n");
        return NULL;
    }

    ee->id = GetNewId(EquipmentID);
    ee->category = category;
    wcscpy_s(ee->name, EQUIPMENT_LENGTH, name, _TRUNCATE);
    ee->price = price;
    ee->room_id = room_id;
    wcscpy_s(ee->purchase_date, DATE_LENGTH, purchase_date, _TRUNCATE);

    return ee;

}

void DestoryExperimentalEquipment(ExperimentalEquipment* experimental_equipment)
{

}

void AddExperimentalEquipment()
{
    system("cls");
    ResourceManager* rm = GetResourceManage();
    size_t category_count = rm->category_list->size;
    if (category_count == 0)
    {
        printf("��ǰ�޷�����豸\n");
        return;
    }
    printf("---      ѡ����Ҫ��ӵ�����        ---\n\n");

    Node* temp = rm->category_list->head;
    for (size_t i = 0; i < category_count; i++)
    {
        temp = temp->next;
        Category* category = (Category*)temp->data;
        printf("--- No%d. %s\n", (int)i + 1, category->name);
    }
    printf("--- 0. ȡ��\n");
    printf("--- ѡ��->");
    int select = 0;
    scanf_s("%d", &select);

    if (select == 0)
    {
        return;
    }
    else if (select > category_count)
    {
        printf("�Ƿ�ָ��\n");
        system("pause");
        return;
    }
    else
    {
        char name[EQUIPMENT_LENGTH];
        int room_id;
        int price;
        char purchase_date[DATE_LENGTH];

        printf("--- �����豸����->   ");
        scanf_s("%s", name, EQUIPMENT_LENGTH);

        printf("--- ��������ʵ����id->  ");
        scanf_s("%d", &room_id);
        LabRoom* labroom = RoomId_to_LabRoom(room_id);
        if (labroom == NULL)
        {
            printf("��ʵ���Ҳ�����\n");
            system("pause");
            return;
        }

        printf("--- �����豸�۸�->  ");
        scanf_s("%d", &price);

        printf("--- ���빺��ʱ�䣨yyyymmdd��->  ");
        scanf_s("%s", purchase_date, DATE_LENGTH);
        Category* category = LinkedList_at(rm->category_list, select);
        ExperimentalEquipment* new_ee = CreateExperimentalEquipment(category, name, room_id, price, purchase_date);
        if (new_ee == NULL)
            return;

        LinkedList_pushback(rm->equipment_list, new_ee);
        LinkedList_pushback(labroom->equipments_list, &(new_ee->id));

        printf("�����ɹ���\n�豸id: %d\n�豸����: %s\n�豸�۸�: %d Ԫ\n�豸����ʱ��: %s", 
            new_ee->id, name, price, purchase_date);
        system("pause");

    }
}

void _AddExperimentalEquipment(HWND hWnd)
{
    ResourceManager* rm = GetResourceManage();

    wchar_t name[50], date[11];
    int room_id = 0, price = 0;
    HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_ROOM_ID); 
    if (GetWindowTextLength(hEdit) == 0)
    {
        MessageBox(hWnd, L"������ʵ����ID��", L"��ʾ", MB_OK | MB_ICONWARNING);
        return;
    }
    hEdit = GetDlgItem(hWnd, IDC_EDIT_PRICE);
    if (GetWindowTextLength(hEdit) == 0)
    {
        MessageBox(hWnd, L"�����빺��۸�", L"��ʾ", MB_OK | MB_ICONWARNING);
        return;
    }
    hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME);
    if (GetWindowTextLength(hEdit) == 0)
    {
        MessageBox(hWnd, L"�������豸���֣�", L"��ʾ", MB_OK | MB_ICONWARNING);
        return;
    }

    room_id = GetInputNumber(hWnd, IDC_EDIT_ROOM_ID);
    price = GetInputNumber(hWnd, IDC_EDIT_PRICE);
    GetDlgItemText(hWnd, IDC_EDIT_NAME, name, sizeof(name) / sizeof(wchar_t));

    LabRoom* labroom = RoomId_to_LabRoom(room_id);
    if (labroom == NULL)
    {
        MessageBox(hWnd, L"ʵ���Ҳ����ڣ�", L"��ʾ", MB_OK | MB_ICONWARNING);
        return;
    }

    HWND hComboBox = GetDlgItem(hWnd, IDC_COMBOX_CATEGORY);
    int index = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
    Category* category = LinkedList_at(rm->category_list, index);


    SYSTEMTIME st;
    HWND hDatePicker = GetDlgItem(hWnd, IDC_DATEPICKER_DATE);
    SendMessage(hDatePicker, DTM_GETSYSTEMTIME, 0, (LPARAM)&st);
    swprintf(date, sizeof(date) / sizeof(wchar_t), L"%04d%02d%02d", st.wYear, st.wMonth, st.wDay);

    ExperimentalEquipment* new_ee = CreateExperimentalEquipment(category, name, room_id, price, date);
    if (new_ee == NULL)
    {
        MessageBox(hWnd, L"����ʧ�ܣ�", L"��ʾ", MB_OK | MB_ICONWARNING);
        return;
    }

    LinkedList_pushback(rm->equipment_list, new_ee);
    LinkedList_pushback(labroom->equipments_list, &(new_ee->id));

    HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
    index = ListView_GetItemCount(hListView);

    LVITEM lvItem;
    lvItem.mask = LVIF_TEXT;
    lvItem.iItem = index; 
    lvItem.iSubItem = 0;

    wchar_t idBuffer[16];
    swprintf_s(idBuffer, 16, L"%d", new_ee->id);
    lvItem.pszText = idBuffer;
    ListView_InsertItem(hListView, &lvItem);

    ListView_SetItemText(hListView, index, 1, new_ee->name);
    
    ListView_SetItemText(hListView, index, 2, new_ee->category->name);

    wchar_t roomBuffer[16];
    swprintf_s(roomBuffer, 16, L"%d", new_ee->room_id);
    ListView_SetItemText(hListView, index, 3, roomBuffer);

    wchar_t priceBuffer[16];
    swprintf_s(priceBuffer, 16, L"%d", new_ee->price);
    ListView_SetItemText(hListView, index, 4, priceBuffer);

    ListView_SetItemText(hListView, index, 5, new_ee->purchase_date)
    MessageBox(hWnd, L"�ɹ�����豸", L"��ʾ", MB_OK);
}

void _DeleteExperimentalEquipment(HWND hWnd)
{
    HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
    int selectedIndex = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);
    if (selectedIndex == -1)
    {
        MessageBox(hWnd, L"δѡ���豸", L"��ʾ", MB_OK);
        return;
    }

    ExperimentalEquipment* delete_ee = LinkedList_at(GetResourceManage()->equipment_list, selectedIndex);
    wchar_t message[256];
    swprintf_s(message, sizeof(message) / sizeof(wchar_t), L"ȷ��ɾ�� %s ��", delete_ee->name);

    int result = MessageBox(hWnd, message, L"ɾ��ȷ��", MB_YESNO | MB_ICONQUESTION);
    if (result == IDYES) 
    {
        LinkedList_delete(GetResourceManage()->equipment_list, selectedIndex);

        HWND hwndListView = GetDlgItem(hWnd, IDC_LISTVIEW);
        ListView_DeleteItem(hwndListView, selectedIndex);

        MessageBox(hWnd, L"�豸��ɾ��", L"ɾ��", MB_OK);
    }
    else 
    {
        return;
    }
}

void _ChangeExperimentalEquipment(HWND hWnd)
{
    HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
    int selectedIndex = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);

    if (selectedIndex == -1)
    {
        MessageBox(hWnd, L"δѡ���豸", L"��ʾ", MB_OK);
        return;
    }

    int result = MessageBox(hWnd, L"ȷ��Ҫ�޸���", L"�޸�ȷ��", MB_YESNO | MB_ICONQUESTION);
    ExperimentalEquipment* change_ee = LinkedList_at(GetResourceManage()->equipment_list, selectedIndex);
    if (result == IDYES)
    {
        HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_ROOM_ID_CHANGE);
        if (GetWindowTextLength(hEdit) != 0)
        {
            int room_id = GetInputNumber(hWnd, IDC_EDIT_ROOM_ID_CHANGE);
            LabRoom* labroom = RoomId_to_LabRoom(room_id);
            if (labroom == NULL)
            {
                MessageBox(hWnd, L"ʵ���Ҳ����ڣ�", L"��ʾ", MB_OK | MB_ICONWARNING);
                return;
            }
            ChangeRoom_id(change_ee, room_id);
        }
        hEdit = GetDlgItem(hWnd, IDC_EDIT_PRICE_CHANGE);
        if (GetWindowTextLength(hEdit) != 0)
        {
            int price = GetInputNumber(hWnd, IDC_EDIT_ROOM_ID_CHANGE);
            ChangePrice(change_ee, price);
        }
        hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME_CHANGE);
        if (GetWindowTextLength(hEdit) != 0)
        {
            wchar_t name[50];
            GetDlgItemText(hWnd, IDC_EDIT_NAME_CHANGE, name, sizeof(name) / sizeof(wchar_t));
            ChangeName(change_ee, name);
        }

        HWND hCheckBox = GetDlgItem(hWnd, IDC_CHECKBOX_1);
        if (SendMessage(hCheckBox, BM_GETCHECK, 0, 0) == BST_CHECKED)
        {
            wchar_t date[11];
            SYSTEMTIME st;
            HWND hDatePicker = GetDlgItem(hWnd, IDC_DATEPICKER_DATE_CHANGE);
            SendMessage(hDatePicker, DTM_GETSYSTEMTIME, 0, (LPARAM)&st);
            swprintf(date, sizeof(date) / sizeof(wchar_t), L"%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
            ChangePurchaseDate(change_ee, date);
        }

        hCheckBox = GetDlgItem(hWnd, IDC_CHECKBOX_2);
        if (SendMessage(hCheckBox, BM_GETCHECK, 0, 0) == BST_CHECKED)
        {
            HWND hComboBox = GetDlgItem(hWnd, IDC_COMBOX_CATEGORY_CHANGE);
            int index = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
            Category* category = LinkedList_at(GetResourceManage()->category_list, index);

            ChangeExperimentalCategory(change_ee, category);
        }

        wchar_t idBuffer[16];
        swprintf_s(idBuffer, 16, L"%d", change_ee->id);
        ListView_SetItemText(hListView, selectedIndex, 0, idBuffer);

        ListView_SetItemText(hListView, selectedIndex, 1, change_ee->name);

        ListView_SetItemText(hListView, selectedIndex, 2, change_ee->category->name);

        wchar_t roomBuffer[16];
        swprintf_s(roomBuffer, 16, L"%d", change_ee->room_id);
        ListView_SetItemText(hListView, selectedIndex, 3, roomBuffer);

        wchar_t priceBuffer[16];
        swprintf_s(priceBuffer, 16, L"%d", change_ee->price);
        ListView_SetItemText(hListView, selectedIndex, 4, priceBuffer);

        ListView_SetItemText(hListView, selectedIndex, 5, change_ee->purchase_date);
        MessageBox(hWnd, L"�豸���޸�", L"�޸�", MB_OK);
    }
    else
    {
        return;
    }
}

bool ChangeName(ExperimentalEquipment* eq, wchar_t* newname)
{
    wcscpy_s(eq->name, EQUIPMENT_LENGTH, newname);
    return True;
}

bool ChangePrice(ExperimentalEquipment* eq, int newprice)
{
    eq->price = newprice;
    return True;
}

bool ChangeRoom_id(ExperimentalEquipment* eq, int newroomid)
{
	//���ڶ�Ӧʵ���ҵ��豸������ɾ�����豸
	LabRoom* lab_room1= RoomId_to_LabRoom(eq->room_id);
	DeleteEquipment(lab_room1, eq->id);

	eq->room_id = newroomid;

	//�����µ�ʵ���ҵ��豸��������Ӹ��豸
	LabRoom* lab_room2 = RoomId_to_LabRoom(eq->room_id);
	AddEquipment(lab_room2, eq->id);
	return True;
}

bool ChangePurchaseDate(ExperimentalEquipment* eq, wchar_t* newdate)
{
    wcscpy_s(eq->purchase_date, DATE_LENGTH, newdate);
    return True;
}

bool ChangeExperimentalCategory(ExperimentalEquipment* eq, Category* newcategory)
{
    eq->category = newcategory;
    return True;
}


//E��ʾequipment�Ĳ���
LinkedList* EFindByName(LinkedList* eqlist,char* name)
{
	if (name == "\n")
	{
		return eqlist;
	}
    LinkedList* list = CreateLinkedList();
    Node* temp = eqlist->head->next;
    while (temp)
    {
        ExperimentalEquipment* eq = (ExperimentalEquipment*)temp->data;
        if (strcmp(eq->name, name))
            LinkedList_pushback(list, eq);
        temp = temp-> next;
    }
	//Ϊ���������ڴ棬��ʱ����������
    if (eqlist != GetResourceManage()->equipment_list)
    {
		destoryLinkedList(eqlist);
    }
    return list;
}

LinkedList* EFindById(LinkedList* eqlist, int id)
{
	if (id == 0)
	{
		return eqlist;
	}
	LinkedList* list = CreateLinkedList();
	Node* temp = eqlist->head->next;
	while (temp)
	{
		ExperimentalEquipment* eq = (ExperimentalEquipment*)temp->data;
		if (eq->id == id)
			LinkedList_pushback(list, eq);
		temp = temp->next;
	}
    //Ϊ���������ڴ棬��ʱ����������
    if (eqlist != GetResourceManage()->equipment_list)
    {
        destoryLinkedList(eqlist);
    }
	return list;
}

LinkedList* EFindByRoom_id(LinkedList* eqlist, int roomid)
{
    if (roomid == 0)
    {
        return eqlist;
    }
	LinkedList* list = CreateLinkedList();
	Node* temp = eqlist->head->next;
	while (temp)
	{
		ExperimentalEquipment* eq = (ExperimentalEquipment*)temp->data;
		if (eq->room_id == roomid)
			LinkedList_pushback(list, eq);
		temp = temp->next;
	}
    //Ϊ���������ڴ棬��ʱ����������
    if (eqlist != GetResourceManage()->equipment_list)
    {
        destoryLinkedList(eqlist);
    }
	return list;
}   

        
LinkedList* EFindByCategory(LinkedList* eqlist, int categoryid)
{
    if (categoryid == 0)
    {
        return eqlist;
    }
	LinkedList* list = CreateLinkedList();
	Node* temp = eqlist->head->next;
	while (temp)
	{
		ExperimentalEquipment* eq = (ExperimentalEquipment*)temp->data;
		if (eq->category->id == categoryid)
			LinkedList_pushback(list, eq);
		temp = temp->next;
	}
    //Ϊ���������ڴ棬��ʱ����������
    if (eqlist != GetResourceManage()->equipment_list)
    {
        destoryLinkedList(eqlist);
    }
	return list;
} 

LinkedList* EFindByDate(LinkedList* eqlist, char* start, char* end)
{
	if (!strcmp(start,"\n") && !strcmp(end,"\n"))
	{
		return eqlist;
	}
	if (!strcmp(start, "\n"))
	{
		strcpy_s(start,15, "0");
	}
    if (!strcmp(end, "\n"))
    {
        strcpy_s(end, 15,"9");
    }
	LinkedList* list = CreateLinkedList();
	Node* temp = eqlist->head->next;
	while (temp)
	{
		ExperimentalEquipment* eq = (ExperimentalEquipment*)temp->data;
		if (strcmp(eq->purchase_date, start) >= 0 && strcmp(eq->purchase_date, end) <= 0)
			LinkedList_pushback(list,eq);
		temp = temp->next;
	}
    //Ϊ���������ڴ棬��ʱ����������
    if (eqlist != GetResourceManage()->equipment_list)
    {
        destoryLinkedList(eqlist);
    }
	return list;
}

LinkedList* EFindByPrice(LinkedList* eqlist, int min, int max)
{
	if (min == 0 && max == 0)
	{
		return eqlist;
	}
    if(max==0)
	{
		max = 999999999;
	}
	LinkedList* list = CreateLinkedList();
	Node* temp = eqlist->head->next;
	while (temp)
	{
		ExperimentalEquipment* eq = (ExperimentalEquipment*)temp->data;
		if (eq->price >= min && eq->price <= max)
			LinkedList_pushback(list, eq);
		temp = temp->next;
	}
    //Ϊ���������ڴ棬��ʱ����������
    if (eqlist != GetResourceManage()->equipment_list)
    {
        destoryLinkedList(eqlist);
    }
	return list;
}