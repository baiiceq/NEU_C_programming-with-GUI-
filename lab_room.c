#include "lab_room.h"
#include <CommCtrl.h>


LabRoom* CreateLabRoom(wchar_t* name)
{
    LabRoom* new_labroom = (LabRoom*)malloc(sizeof(LabRoom));

    if (new_labroom == NULL)
    {
        printf("����ʵ����ʧ��\n");
        return NULL;
    }

	wcscpy_s(new_labroom->name, LABROOM_LENGTH, name);
    new_labroom->id = GetNewId(RoomID);
    new_labroom->equipments_list = CreateLinkedList();
    new_labroom->technician_id_list = CreateLinkedList();

    return new_labroom;
}

void DestoryLabRoom(LabRoom* lab_room)
{
	destoryLinkedList(lab_room->equipments_list);
	destoryLinkedList(lab_room->technician_id_list);
	free(lab_room);
}

void AddLabRoom()
{
    char name[LABROOM_LENGTH];
    system("cls");
    printf("---          �����ʵ����          ---\n\n");
    printf("--- ������������->   ");
    scanf_s("%s", name, LABROOM_LENGTH);

    LabRoom* new_labroom= CreateLabRoom(name);
    if (new_labroom == NULL)
        return;

    LinkedList_pushback(GetResourceManage()->laboratory_list, new_labroom);

    printf("�����ɹ���\nʵ����id: %d\nʵ��������: %s\n", new_labroom->id, name);
    system("pause");
}

void _AddLabRoom(HWND hWnd)
{
	ResourceManager* rm = GetResourceManage();

	wchar_t name[50];

	HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME);
	if (GetWindowTextLength(hEdit) == 0)
	{
		MessageBox(hWnd, L"������ʵ�������֣�", L"��ʾ", MB_OK | MB_ICONWARNING);
		return;
	}
	GetDlgItemText(hWnd, IDC_EDIT_NAME, name, sizeof(name) / sizeof(wchar_t));

	LabRoom* new_labroom = CreateLabRoom(name);

	if (new_labroom == NULL)
	{
		MessageBox(hWnd, L"����ʧ�ܣ�", L"��ʾ", MB_OK | MB_ICONWARNING);
		return;
	}

	LinkedList_pushback(rm->laboratory_list, new_labroom);

	HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
	int index = ListView_GetItemCount(hListView);

	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = index;
	lvItem.iSubItem = 0;

	wchar_t idBuffer[16];
	swprintf_s(idBuffer, 16, L"%d", new_labroom->id);
	lvItem.pszText = idBuffer;
	ListView_InsertItem(hListView, &lvItem);

	ListView_SetItemText(hListView, index, 1, new_labroom->name);

	MessageBox(hWnd, L"�����ɹ���", L"��ʾ", MB_OK | MB_ICONWARNING);
}

void _DeleteLabRoom(HWND hWnd)
{
	HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
	int selectedIndex = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);
	if (selectedIndex == -1)
	{
		MessageBox(hWnd, L"δѡ���豸", L"��ʾ", MB_OK);
		return;
	}

	LabRoom* delete_labroom = LinkedList_at(GetResourceManage()->laboratory_list, selectedIndex);
	wchar_t message[256];
	swprintf_s(message, sizeof(message) / sizeof(wchar_t), L"ȷ��ɾ�� %s ��", delete_labroom->name);

	int result = MessageBox(hWnd, message, L"ɾ��ȷ��", MB_YESNO | MB_ICONQUESTION);
	if (result == IDYES)
	{
		LinkedList_delete(GetResourceManage()->laboratory_list, selectedIndex);

		Node* temp = delete_labroom->equipments_list->head;
		while (temp->next)
		{
			temp = temp->next;
			int ee_id = *((int*)temp->data);
			ExperimentalEquipment* ee = id_to_equipment(ee_id);
			ee->room_id = -1;
		}

		temp = delete_labroom->technician_id_list->head;
		while (temp->next)
		{
			temp = temp->next;
			int account_id = *((int*)temp->data);
			Account* account = FindById(account_id);
			account->roomid = -1;
		}

		DestoryLabRoom(delete_labroom);

		HWND hwndListView = GetDlgItem(hWnd, IDC_LISTVIEW);
		ListView_DeleteItem(hwndListView, selectedIndex);

		MessageBox(hWnd, L"�豸��ɾ��", L"ɾ��", MB_OK);
	}
	else
	{
		return;
	}
}

void _ChangeLabRoom(HWND hWnd)
{
	HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
	int selectedIndex = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);

	if (selectedIndex == -1)
	{
		MessageBox(hWnd, L"δѡ��ʵ����", L"��ʾ", MB_OK);
		return;
	}

	int result = MessageBox(hWnd, L"ȷ��Ҫ�޸���", L"�޸�ȷ��", MB_YESNO | MB_ICONQUESTION);
	LabRoom* change_lab = LinkedList_at(GetResourceManage()->laboratory_list, selectedIndex);
	if (result == IDYES)
	{
		HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME_CHANGE);
		if (GetWindowTextLength(hEdit) != 0)
		{
			wchar_t name[50];
			GetDlgItemText(hWnd, IDC_EDIT_NAME_CHANGE, name, sizeof(name) / sizeof(wchar_t));
			ChangeLabName(change_lab, name);
		}

		wchar_t idBuffer[16];
		swprintf_s(idBuffer, 16, L"%d", change_lab->id);
		ListView_SetItemText(hListView, selectedIndex, 0, idBuffer);

		ListView_SetItemText(hListView, selectedIndex, 1, change_lab->name);

		MessageBox(hWnd, L"�豸���޸�", L"�޸�", MB_OK);
	}
	else
	{
		return;
	}
}

bool DeleteLabRoom()
{
    printf("������Ҫɾ����ʵ���ұ��\n");
    int room_id;
    scanf_s("%d", &room_id);
	Node* temp = GetResourceManage()->laboratory_list->head;
	while (temp->next)
	{
		LabRoom* labroom = (LabRoom*)temp->next->data;
		if (room_id == labroom->id)
		{
            Node* temp2 = temp->next;
			temp->next = temp->next->next;
            free(temp2);
			printf("ɾ���ɹ�\n");
			system("pause");
            return True;
		}
		temp = temp->next;
	}
	printf("��ɾ����ʵ���Ҳ�����\n");
	return False;
}

bool ChangeLabRoom()
{
	printf("������Ҫ�޸ĵ�ʵ���ұ��\n");
	int room_id;
	scanf_s("%d", &room_id);
	LabRoom* labroom = RoomId_to_LabRoom(room_id);
	if (labroom == NULL)
	{
		printf("��ʵ���Ҳ�����\n");
		return False;
	}
	while (1)
	{
		system("cls");

		printf("---            ѡ��Ҫ���еĲ���         ---\n");
		printf("---            1. �޸�ʵ��������               ---\n");
		printf("---            2. �ڸ�ʵ��������豸           ---\n");
		printf("---            3. �ڸ�ʵ����ɾ���豸           ---\n");
		printf("---            4. �ڸ�ʵ�������ʵ��Ա         ---\n");
		printf("---            5. �ڸ�ʵ����ɾ��ʵ��Ա         ---\n");
		printf("---            0. ȡ��               ---\n");
		printf("--- ѡ��->");

		int option = 0;
		scanf_s("%d", &option);
		getchar();
		switch (option)
		{
		case 1:
		{
			printf("�������µ�ʵ��������(������Ŀɻس�����)\n");
			char newname[LABROOM_LENGTH] = "";
			fgets(newname, LABROOM_LENGTH, stdin);
			ChangeLabName(labroom, newname);
			break;
		}
		case 2:
		{
			printf("������Ҫ�ڸ�ʵ������ӵ��豸id(������Ŀɻس�����)\n");
			int eqid = 0;
			scanf_s("%d", &eqid);
			getchar();
			AddEquipment(labroom, eqid);
			break;
		}
		case 3:
		{
			printf("������Ҫ�ڸ�ʵ����ɾ�����豸id(������Ŀɻس�����)\n");
			int eqid = 0;
			scanf_s("%d", &eqid);
			getchar();
			DeleteEquipment(labroom, eqid);
			break;
		}
		case 4:
		{
			printf("������Ҫ�ڸ�ʵ������ӵ�ʵ��Աid(������Ŀɻس�����)\n");
			int techid = 0;
			scanf_s("%d", &techid);
			getchar();
			AddTechnician(labroom, techid);
			break;
		}
		case 5:
		{
			printf("������Ҫ�ڸ�ʵ����ɾ����ʵ��Աid(������Ŀɻس�����)\n");
			int techid = 0;
			scanf_s("%d", &techid);
			getchar();
			DeleteTechnician(labroom, techid);
			break;
		}
		case 0:
			return True;
		}
		system("pause");
	}
	
}

bool ChangeLabName(LabRoom* lab_room, wchar_t* newname)
{
	wcscpy_s(lab_room->name, LABROOM_LENGTH, newname);
	return True;
}

bool AddEquipment(LabRoom* lab_room, int eqid)
{
	if (eqid == 0)
		return False;
	LinkedList* temp=EFindById(GetResourceManage()->equipment_list, eqid);
	if (temp->head->next == NULL)
	{
		printf("���豸������\n");
		return False;
	}
	ExperimentalEquipment* eq = (ExperimentalEquipment*)temp->head->next->data;
	LinkedList_pushback(lab_room->equipments_list, &(eq->id));
	printf("��ӳɹ�\n");
	return True;
}

bool DeleteEquipment(LabRoom* lab_room, int eqid)
{
	if (eqid == 0)
		return False;
	Node* temp = lab_room->equipments_list->head;
	while (temp->next)
	{
		int* id = (int*)temp->next->data;
		if (*id == eqid)
		{
			temp->next = temp->next->next;
			return True;
		}
		temp = temp->next;
	}
	printf("��ɾ�����豸������\n");
	return False;
}

bool AddTechnician(LabRoom* lab_room, int techid)
{
	if (techid == 0)
		return False;
	Account* tech = FindById(techid);
	if (tech == NULL)
	{
		printf("��ʵ��Ա������\n");
		return False;
	}
	LinkedList_pushback(lab_room->technician_id_list, &tech->id);
	if (tech->roomid != -1)
	{
		LabRoom* old_labroom = RoomId_to_LabRoom(tech->roomid);
		if (old_labroom != NULL)
		{
			DeleteTechnician(old_labroom, techid);
		}
	}
	tech->roomid = lab_room->id;
	return True;
}

bool DeleteTechnician(LabRoom* lab_room, int techid)
{
	if (techid == 0)
		return False;
	Node* temp = lab_room->technician_id_list->head;
	while (temp->next)
	{
		int* tech = (int*)temp->next->data;
		if (*tech == techid)
		{
			FindById(techid)->roomid = -1;
			temp->next = temp->next->next;
			return True;
		}
		temp = temp->next;
	}
	printf("��ɾ����ʵ��Ա������\n");
	return False;
}

LabRoom* RoomId_to_LabRoom(int room_id)
{
    Node* temp = GetResourceManage()->laboratory_list->head;

    while (temp->next)
    {
        temp = temp->next;
        LabRoom* labroom = (LabRoom*)temp->data;
        if (room_id == labroom->id)
            return labroom;
    }
    return NULL;
}

