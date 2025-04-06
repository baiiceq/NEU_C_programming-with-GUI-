#include "service.h"
#include <windows.h>
#include <commctrl.h>
#include <time.h>

Service* CCreateService(int equipment_id, char* equipment_name, int user_id, char* data, Service_type type, char* reason)
{
	Service* service = (Service*)malloc(sizeof(Service));
	if (service == NULL)
	{
		printf("�ڴ����ʧ��\n");
		exit(-1);
	}
	service->service_id = GetNewId(ServiceID);
	service->equipment_id = equipment_id;
	strcpy_s(service->equipment_name, EQUIPMENT_LENGTH, equipment_name);
	strcpy_s(service->user_id, USER_NMAE_LENGTH, user_id);
	strcpy_s(service->data, DATE_LENGTH, data);
	service->type = type;
	strcpy_s(service->reason, NOTE_LENGTH, reason);
	return service;
}

void DestoryService(Service* service)
{
	if (service)
		free(service);
}

void AddService(Account* account)
{
	ResourceManager* rm = GetResourceManage();
	Service* service = (Service*)malloc(sizeof(Service));
	if (service == NULL)
	{
		printf("�ڴ����ʧ��\n");
		return;
	}
	printf("�������豸ID->");
	scanf_s("%d", &service->equipment_id);
	getchar();
	ExperimentalEquipment* ee = (ExperimentalEquipment*)EFindById(rm->equipment_list, service->equipment_id)->head->next->data;
	printf("ID��Ӧ���豸��Ϊ%s\n", ee->name);
	strcpy_s(service->equipment_name, EQUIPMENT_LENGTH, ee->name);
	//��ȡʱ��
	time_t now;
	time(&now);
	char formattedTime[30];
	struct tm timeinfo;
	localtime_s(&timeinfo, &now);
	strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%d_%H-%M-%S", &timeinfo);
	formattedTime[10] = 0;
	strcpy_s(service->data, DATE_LENGTH, formattedTime);
	printf("������ά������(0:��ʧ�Ǽ� 1:�𻵵Ǽ� 2:ά�޵Ǽ� 3:ά����� 4:���ϵǼ�)->");
	int type;
	scanf_s("%d", &type);
	getchar();
	switch (type)
	{
	case 0:
		service->type = LostRegister;
		ee->state = Lost;
		break;
	case 1:
		service->type = DamagedRegister;
		ee->state = Damaged;
		break;
	case 2:
		service->type = ServiceRegister;
		ee->state = Repairing;
		break;
	case 3:
		service->type = ServiceFinish;
		ee->state = Idle;
		break;
	case 4:
		service->type = ScrapRegister;
		ee->state = Scrapped;
		break;
	default:
		printf("�Ƿ�ָ��\n");
		free(service);
		return;
	}
	printf("���б�ע������->");
	fgets(service->reason, NOTE_LENGTH, stdin);
	if (strcmp(service->reason, "\n") == 0)
		strcpy_s(service->reason, sizeof(service->reason), "nothing");
	else
		service->reason[strlen(service->reason) - 1] = '\0';
	service->user_id = account->id;
	service->service_id = GetNewId(ServiceID);
	LinkedList_pushback(rm->service_list, service);
	printf("ά�޳ɹ�\n");
}

void DDeleteService(Service* service)
{
	ResourceManager* rm = GetResourceManage();
	Node* temp = rm->service_list->head;
	while (temp->next)
	{
		Service* temp_service = (Service*)temp->next->data;
		if (temp_service == service)
		{
			temp->next = temp->next->next;
			DestoryService(service);
			rm->service_list->size--;
			break;
		}
		temp = temp->next;
	}
}



extern EquipmentManagement* em;
HWND hwndServiceManagement;

LRESULT CALLBACK ServiceWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HWND hServiceIdEdit, hServiceTypeEdit, hEquipmentIdEdit, hEquipmentNameEdit, hOperatorIdEdit, hServiceDateEdit, hServiceDescriptionEdit;
    static HWND hServiceListView;

    switch (msg)
    {
    case WM_CREATE:
        // ���������
        CreateWindow(L"STATIC", L"ά��ID:", WS_VISIBLE | WS_CHILD, 20, 20, 80, 25, hWnd, NULL, NULL, NULL);
        hServiceIdEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 20, 200, 25, hWnd, NULL, NULL, NULL);

        CreateWindow(L"STATIC", L"ά������:", WS_VISIBLE | WS_CHILD, 20, 60, 80, 25, hWnd, NULL, NULL, NULL);
        hServiceTypeEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 60, 200, 25, hWnd, NULL, NULL, NULL);

        CreateWindow(L"STATIC", L"�豸ID:", WS_VISIBLE | WS_CHILD, 20, 100, 80, 25, hWnd, NULL, NULL, NULL);
        hEquipmentIdEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 100, 200, 25, hWnd, NULL, NULL, NULL);

        CreateWindow(L"STATIC", L"�豸����:", WS_VISIBLE | WS_CHILD, 20, 140, 80, 25, hWnd, NULL, NULL, NULL);
        hEquipmentNameEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 140, 200, 25, hWnd, NULL, NULL, NULL);

        CreateWindow(L"STATIC", L"������ԱID:", WS_VISIBLE | WS_CHILD, 20, 180, 80, 25, hWnd, NULL, NULL, NULL);
        hOperatorIdEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 180, 200, 25, hWnd, NULL, NULL, NULL);

        CreateWindow(L"STATIC", L"ά������:", WS_VISIBLE | WS_CHILD, 20, 220, 80, 25, hWnd, NULL, NULL, NULL);
        hServiceDateEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 220, 200, 25, hWnd, NULL, NULL, NULL);

        CreateWindow(L"STATIC", L"ά������:", WS_VISIBLE | WS_CHILD, 20, 260, 80, 25, hWnd, NULL, NULL, NULL);
        hServiceDescriptionEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 260, 200, 25, hWnd, NULL, NULL, NULL);

        // ����ά�޼�¼�б���ͼ
        hServiceListView = CreateWindowEx(0, WC_LISTVIEW, NULL,
            WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_SINGLESEL,
            20, 300, 760, 300, hWnd, NULL, GetModuleHandle(NULL), NULL);

        // �����
        LVCOLUMNW lvc;
        lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

        lvc.pszText = L"ά��ID";
        lvc.cx = 80;
        lvc.iSubItem = 0;
        ListView_InsertColumn(hServiceListView, 0, &lvc);

        lvc.pszText = L"ά������";
        lvc.cx = 100;
        lvc.iSubItem = 1;
        ListView_InsertColumn(hServiceListView, 1, &lvc);

        lvc.pszText = L"�豸ID";
        lvc.cx = 80;
        lvc.iSubItem = 2;
        ListView_InsertColumn(hServiceListView, 2, &lvc);

        lvc.pszText = L"�豸����";
        lvc.cx = 150;
        lvc.iSubItem = 3;
        ListView_InsertColumn(hServiceListView, 3, &lvc);

        lvc.pszText = L"������ԱID";
        lvc.cx = 100;
        lvc.iSubItem = 4;
        ListView_InsertColumn(hServiceListView, 4, &lvc);

        lvc.pszText = L"ά������";
        lvc.cx = 100;
        lvc.iSubItem = 5;
        ListView_InsertColumn(hServiceListView, 5, &lvc);

        lvc.pszText = L"ά������";
        lvc.cx = 200;
        lvc.iSubItem = 6;
        ListView_InsertColumn(hServiceListView, 6, &lvc);

        // ���ά�޼�¼�б�����
        FillServiceList(hServiceListView);
        break;

    case WM_COMMAND:
        // ����ť����¼�
        break;

    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        hwndServiceManagement = NULL;
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void ShowServiceWindow(HWND hWnd)
{
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = ServiceWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"ServiceManagementWindow";

    RegisterClass(&wc);

    hwndServiceManagement = CreateWindow(L"ServiceManagementWindow", L"ά�޹���",
        WS_OVERLAPPED | WS_SYSMENU, 200, 100, 800, 650, hWnd, NULL, GetModuleHandle(NULL), NULL);

    ShowWindow(hwndServiceManagement, SW_SHOW);
    UpdateWindow(hwndServiceManagement);
}

void FillServiceList(HWND hListView)
{
    ListView_DeleteAllItems(hListView);

    ResourceManager* rm = GetResourceManage();
    if (!rm || !rm->service_list) return;

    Node* temp = rm->service_list->head;
    size_t count = rm->service_list->size;

    for (size_t i = 0; i < count; i++)
    {
        temp = temp->next;
        if (!temp) break;

        Service* service = (Service*)temp->data;
        if (!service) continue;

        LVITEM lvItem;
        ZeroMemory(&lvItem, sizeof(LVITEM));
        lvItem.mask = LVIF_TEXT;
        lvItem.iItem = i;
        lvItem.iSubItem = 0;

        // ά��ID
        wchar_t idBuffer[16];
        swprintf_s(idBuffer, 16, L"%d", service->service_id);
        lvItem.pszText = idBuffer;
        ListView_InsertItem(hListView, &lvItem);

        // ά������
		wchar_t type[50];
        switch (service->type)
        {
		case LostRegister: wcscpy_s(type, 50, L"LostRegister"); 
            break;
		case DamagedRegister: wcscpy_s(type, 50, L"DamagedRegister");
			break;
		case ServiceRegister: wcscpy_s(type, 50, L"ServiceRegister");
			break;
		case ServiceFinish: wcscpy_s(type, 50, L"ServiceFinish");
			break;
        default: wcscpy_s(type, 50, L"ScrapRegister");
			break;
        }
        ListView_SetItemText(hListView, i, 1,type);

        // �豸ID
        wchar_t equipmentIdBuffer[16];
        swprintf_s(equipmentIdBuffer, 16, L"%d", service->equipment_id);
        ListView_SetItemText(hListView, i, 2, equipmentIdBuffer);

        // �豸����
        ListView_SetItemText(hListView, i, 3, service->equipment_name);

        // ������ԱID
        wchar_t operatorIdBuffer[16];
        swprintf_s(operatorIdBuffer, 16, L"%d", service->user_id);
        ListView_SetItemText(hListView, i, 4, operatorIdBuffer);

        // ά������
        ListView_SetItemText(hListView, i, 5, service->data);

        // ά������
        ListView_SetItemText(hListView, i, 6, service->reason);
    }
}
