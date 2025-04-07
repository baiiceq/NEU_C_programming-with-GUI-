#include "service.h"
#include <windows.h>
#include <commctrl.h>
#include <time.h>

Service* CCreateService(int equipment_id, char* equipment_name, int user_id, wchar_t* data, Service_type type, wchar_t* reason)
{
	Service* service = (Service*)malloc(sizeof(Service));
	if (service == NULL)
	{
		printf("�ڴ����ʧ��\n");
		exit(-1);
	}
	service->service_id = GetNewId(ServiceID);
	service->equipment_id = equipment_id;
	wcscpy_s(service->equipment_name, EQUIPMENT_LENGTH, equipment_name);
	service->user_id = user_id;
	wcscpy_s(service->data, DATE_LENGTH, data);
	service->type = type;
	wcscpy_s(service->reason, NOTE_LENGTH, reason);
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
	printf("�������������(0:��ʧ�Ǽ� 1:�𻵵Ǽ� 2:ά�޵Ǽ� 3:ά����� 4:���ϵǼ�)->");
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
	case 5:
		service->type = UsingRegister;
		ee->state = Using;
		break;
	case 6:
		service->type = UsingFinish;
		ee->state = Idle;
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
	printf("�����ɹ�\n");
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
static HWND hwndServiceManagement;

LRESULT CALLBACK ServiceWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HWND hServiceIdEdit, hServiceTypeCombo, hEquipmentNameEdit, 
        hOperatorId, hServiceDate, hServiceDescriptionEdit;
    static HWND hServiceListView;

    switch (msg)
    {
	case WM_CREATE:
		// �豸���������Ͳ�ѯ��ť
		CreateWindow(L"STATIC", L"�豸����:", WS_VISIBLE | WS_CHILD, 20, 20, 80, 25, hWnd, NULL, NULL, NULL);
		hEquipmentNameEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 20, 200, 25, hWnd, NULL, NULL, NULL);
		CreateWindow(L"BUTTON", L"��ѯ", WS_VISIBLE | WS_CHILD, 320, 20, 80, 25, hWnd, (HMENU)IDC_BUTTON_FIND_SERVICE, GetModuleHandle(NULL), NULL);

		// �б���ͼ
		hServiceListView = CreateWindowEx(0, WC_LISTVIEW, NULL,
			WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_SINGLESEL,
			20, 60, 760, 300, hWnd, NULL, GetModuleHandle(NULL), NULL);

		LVCOLUMNW lvc;
		lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

		lvc.pszText = L"�豸����ID";
		lvc.cx = 80;
		lvc.iSubItem = 0;
		ListView_InsertColumn(hServiceListView, 0, &lvc);

		lvc.pszText = L"��������";
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

		lvc.pszText = L"��������";
		lvc.cx = 100;
		lvc.iSubItem = 5;
		ListView_InsertColumn(hServiceListView, 5, &lvc);

		lvc.pszText = L"��������";
		lvc.cx = 200;
		lvc.iSubItem = 6;
		ListView_InsertColumn(hServiceListView, 6, &lvc);

		CreateWindow(L"STATIC", L"��������:", WS_VISIBLE | WS_CHILD, 20, 380, 80, 25, hWnd, NULL, NULL, NULL);
		hServiceTypeCombo = CreateWindow(L"COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
			100, 380, 120, 200, hWnd, NULL, NULL, NULL);
		SendMessage(hServiceTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"LostRegister");
		SendMessage(hServiceTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"DamagedRegister");
		SendMessage(hServiceTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"ServiceRegister");
		SendMessage(hServiceTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"ServiceFinish");
		SendMessage(hServiceTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"ScrapRegister");
		SendMessage(hServiceTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"UsingRegister");
		SendMessage(hServiceTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"UsingFinish");
		SendMessage(hServiceTypeCombo, CB_SETCURSEL, 0, 0); // ����Ĭ��ѡ����


		CreateWindow(L"STATIC", L"������ԱID:", WS_VISIBLE | WS_CHILD, 240, 380, 80, 25, hWnd, NULL, NULL, NULL);
		hOperatorId = CreateWindow(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 320, 380, 200, 25, hWnd, NULL, NULL, NULL);
		wchar_t operatorIdBuffer[16];
		swprintf_s(operatorIdBuffer, 16, L"%d", em->current_account->id);
		SetWindowText(hOperatorId, operatorIdBuffer);

		CreateWindow(L"STATIC", L"��������:", WS_VISIBLE | WS_CHILD, 20, 420, 80, 25, hWnd, NULL, NULL, NULL);
		hServiceDate = CreateWindowEx(0, DATETIMEPICK_CLASS, NULL,
			WS_BORDER | WS_CHILD | WS_VISIBLE | DTS_SHORTDATEFORMAT,
			100, 420, 130, 25, hWnd, NULL, GetModuleHandle(NULL), NULL);

		CreateWindow(L"STATIC", L"��������:", WS_VISIBLE | WS_CHILD, 240, 420, 80, 25, hWnd, NULL, NULL, NULL);
		hServiceDescriptionEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 320, 420, 200, 25, hWnd, NULL, NULL, NULL);

		CreateWindow(L"BUTTON", L"���", WS_VISIBLE | WS_CHILD,
			540, 380, 120, 40, hWnd, (HMENU)IDC_BUTTON_ADD_SERVICE, GetModuleHandle(NULL), NULL);

		// ����豸�����¼�б�����
		FillServiceList(hServiceListView);
		break;
    case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_ADD_SERVICE:
		{
			if (GetWindowTextLength(hEquipmentNameEdit) == 0)
			{
				MessageBox(hWnd, L"�������豸����", L"����", MB_OK | MB_ICONERROR);
				break;
			}
			if (GetWindowTextLength(hServiceDescriptionEdit) == 0)
			{
				MessageBox(hWnd, L"�������������", L"����", MB_OK | MB_ICONERROR);
				break;
			}
			//�豸����
			wchar_t equipmentName[EQUIPMENT_LENGTH];
			GetWindowText(hEquipmentNameEdit, equipmentName, EQUIPMENT_LENGTH);

			ResourceManager* rm = GetResourceManage();
			LinkedList* temp = EFindByName(rm->equipment_list, equipmentName);
			if (temp->size == 0)
			{
				MessageBox(hWnd, L"�豸������", L"����", MB_OK | MB_ICONERROR);
				break;
			}
			ExperimentalEquipment* ee = (ExperimentalEquipment*)temp->head->next->data;

			wchar_t serviceDescription[NOTE_LENGTH];
			GetWindowText(hServiceDescriptionEdit, serviceDescription, NOTE_LENGTH);

			int selectedType = SendMessage(hServiceTypeCombo, CB_GETCURSEL, 0, 0);
			Service_type type = (Service_type)selectedType;

			SYSTEMTIME st;
			GetLocalTime(&st);
			wchar_t date[DATE_LENGTH];
			swprintf_s(date, DATE_LENGTH, L"%04d-%02d-%02d", st.wYear, st.wMonth, st.wDay);

			if (type == ScrapRegister)
			{
				wchar_t syear[5] = L"";
				wchar_t* end;
				for (int i = 0;i < 4; i++)
				{
					syear[i] = ee->purchase_date[i];
				}
				syear[4] = L'\0';
				long year = wcstol(syear, &end, 10);
				if (st.wYear - year < ee->category->disposal_years)
				{
					MessageBox(hWnd, L"�豸ʹ�����޲���", L"����", MB_OK | MB_ICONERROR);
					break;
				}
			}

			wchar_t addBuffter[300];
			swprintf_s(addBuffter, 300, L"�豸ID:%d\n�豸����:%ls\n������ԱID:%d\n��������:%ls\n",
				ee->id, equipmentName, em->current_account->id, date);
			switch (type)
			{
			case LostRegister:
				swprintf_s(addBuffter, 300, L"%ls��������:%ls\n", addBuffter, L"LostRegister");
				break;
			case DamagedRegister:
				swprintf_s(addBuffter, 300, L"%ls��������:%ls\n", addBuffter, L"DamagedRegister");
				break;
			case ServiceRegister:
				swprintf_s(addBuffter, 300, L"%ls��������:%ls\n", addBuffter, L"ServiceRegister");
				break;
			case ServiceFinish:
				swprintf_s(addBuffter, 300, L"%ls��������:%ls\n", addBuffter, L"ServiceFinish");
				break;
			case ScrapRegister:
				swprintf_s(addBuffter, 300, L"%ls��������:%ls\n", addBuffter, L"ScrapRegister");
				break;
			case UsingRegister:
				swprintf_s(addBuffter, 300, L"%ls��������:%ls\n", addBuffter, L"UsingRegister");
				break;
			case UsingFinish:
				swprintf_s(addBuffter, 300, L"%ls��������:%ls\n", addBuffter, L"UsingFinish");
				break;
			}
			swprintf_s(addBuffter, 300, L"%ls��������:%ls", addBuffter, serviceDescription);
			if (!MessageBox(hWnd, addBuffter, L"��Ϣȷ��", MB_OK | MB_ICONINFORMATION))
				break;
			Service* service = CCreateService(ee->id, equipmentName, em->current_account->id, date, type, serviceDescription);
			LinkedList_pushback(rm->service_list, service);
			switch (type)
			{
			case LostRegister:ee->state = Lost;
				break;
			case DamagedRegister:ee->state = Damaged;
				break;
			case ServiceRegister:ee->state = Repairing;
				break;
			case ServiceFinish:ee->state = Idle;
				break;
			case ScrapRegister:ee->state = Scrapped;
				break;
			case UsingRegister:ee->state = Using;
				break;
			case UsingFinish:ee->state = Idle;
				break;
			}
			SendMessage(hWnd, WM_CREATE, 0, 0);
		}
		break;
		case IDC_BUTTON_FIND_SERVICE:
		{
			if (GetWindowTextLength(hEquipmentNameEdit) == 0)
			{
				MessageBox(hWnd, L"�������豸����", L"����", MB_OK | MB_ICONERROR);
				break;
			}

			wchar_t equipmentName[EQUIPMENT_LENGTH];
			GetWindowText(hEquipmentNameEdit, equipmentName, EQUIPMENT_LENGTH);

			ResourceManager* rm = GetResourceManage();
			LinkedList* temp = EFindByName(rm->equipment_list, equipmentName);
			if (temp->size == 0)
			{
				MessageBox(hWnd, L"�豸������", L"����", MB_OK | MB_ICONERROR);
				break;
			}

			ListView_DeleteAllItems(hServiceListView);

			Node* node = rm->service_list->head->next;
			int index = 0;
			int count = 0;
			while (node)
			{
				Service* service = (Service*)node->data;
				if (wcscmp(service->equipment_name, equipmentName) == 0)
				{
					count++;
					LVITEM lvItem;
					ZeroMemory(&lvItem, sizeof(LVITEM));
					lvItem.mask = LVIF_TEXT;
					lvItem.iItem = index;
					lvItem.iSubItem = 0;

					// �豸����ID
					wchar_t idBuffer[16];
					swprintf_s(idBuffer, 16, L"%d", service->service_id);
					lvItem.pszText = idBuffer;
					ListView_InsertItem(hServiceListView, &lvItem);

					// ��������
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
					case ScrapRegister:(type, 50, L"ScrapRegister");
						break;
					case UsingRegister: wcscpy_s(type, 50, L"UsingRegister");
						break;
					default: wcscpy_s(type, 50, L"UsingFinish");
						break;
					}
					ListView_SetItemText(hServiceListView, index, 1, type);

					// �豸ID
					wchar_t equipmentIdBuffer[16];
					swprintf_s(equipmentIdBuffer, 16, L"%d", service->equipment_id);
					ListView_SetItemText(hServiceListView, index, 2, equipmentIdBuffer);

					// �豸����
					ListView_SetItemText(hServiceListView, index, 3, service->equipment_name);

					// ������ԱID
					wchar_t operatorIdBuffer[16];
					swprintf_s(operatorIdBuffer, 16, L"%d", service->user_id);
					ListView_SetItemText(hServiceListView, index, 4, operatorIdBuffer);

					// ��������
					ListView_SetItemText(hServiceListView, index, 5, service->data);

					// ��������
					ListView_SetItemText(hServiceListView, index, 6, service->reason);

					index++;
				}
				node = node->next;
			}
			if(count==0)
				MessageBox(hWnd, L"û���ҵ�����豸�����¼", L"��ʾ", MB_OK | MB_ICONINFORMATION);
		}
		break;
		}
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

    hwndServiceManagement = CreateWindow(L"ServiceManagementWindow", L"�豸����",
        WS_OVERLAPPED | WS_SYSMENU, 200, 100, 800, 550, hWnd, NULL, GetModuleHandle(NULL), NULL);

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

        // �豸����ID
        wchar_t idBuffer[16];
        swprintf_s(idBuffer, 16, L"%d", service->service_id);
        lvItem.pszText = idBuffer;
        ListView_InsertItem(hListView, &lvItem);

        // ��������
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
		case ScrapRegister: wcscpy_s(type, 50, L"ScrapRegister");
			break;
		case UsingRegister: wcscpy_s(type, 50, L"UsingRegister");
			break;
		default: wcscpy_s(type, 50, L"UsingFinish");
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

        // ��������
        ListView_SetItemText(hListView, i, 5, service->data);

        // ��������
        ListView_SetItemText(hListView, i, 6, service->reason);
    }
}
