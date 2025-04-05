#pragma once
#include "experimental_equipment.h"
#include "category.h"
#include "lab_room.h"
#include "account.h"
#include "configs.h"

#include <Windows.h>
#include <CommCtrl.h>

#define IDC_TAB 3000
#define IDC_EDIT_NAME 3001
#define IDC_EDIT_MIN_PRICE 3002
#define IDC_EDIT_MAX_PRICE 3003
#define IDC_EDIT_ROOM_ID 3004
#define IDC_MIN_DATE 3005
#define IDC_MAX_DATE 3006
#define IDC_EDIT_ID 3007
#define IDC_COMBOX_CATEGORY 3008
#define IDC_LISTVIEW 3009
#define IDC_STATIC 3010
#define IDC_BUTTON_QUERY 3011
#define IDC_BUTTON_EXIT 3012

extern HWND hwndAdminManagement;

HWND hwndfuncQuery;
HWND hMainTab;

typedef bool (*QueryCondition)(void* data, void* param);

void Query(LinkedList* list, QueryCondition condition, void* param, void (*PrintResult)(void*))
{
    if (list == NULL || condition == NULL || PrintResult == NULL) {
        printf("��ѯʧ��: ������Ч��\n");
        return;
    }

    Node* node = list->head;
    bool found = False;
    while (node->next != NULL)
    {
        node = node->next;
        if (condition(node->data, param))
        {
            PrintResult(node->data);
            found = True;
        }
        
    }

    system("pause");

    if (!found) 
    {
        printf("δ�ҵ����������ļ�¼��\n\n");
        system("pause");
    }
}

typedef struct _EquipmentQuery
{
    int id;                            // ID
    int category_id;                   // ���
    char purchase_date[DATE_LENGTH];   // ��������
    int min_price;                     // ��С�۸�
    int max_price;                     // ���۸�
    char name[EQUIPMENT_LENGTH];       // ����
    int room_id;                       // ��������

} EquipmentQuery;

bool QueryEquipmenCondition(void* data, void* param)
{
    ExperimentalEquipment* eq = (ExperimentalEquipment*)data;
    EquipmentQuery* query = (EquipmentQuery*)param;

    // �ж������� query->category ���ǿգ���ƥ�䣩
    if (query->category_id != -1 && eq->category->id != query->category_id)
        return False;

    if (query->id != -1 && query->id != eq->id)
        return False;

    // �жϹ�������
    if (strlen(query->purchase_date) > 0 && strcmp(eq->purchase_date, query->purchase_date) != 0)
        return False;

    // �жϼ۸�Χ
    if ((query->min_price >= 0 && eq->price < query->min_price) ||
        (query->max_price >= 0 && eq->price > query->max_price))
        return False;

    // �ж��豸���ƣ�ģ��ƥ�䣩
    if (strlen(query->name) > 0 && strstr(eq->name, query->name) == NULL)
        return False;

    // �ж���������
    if (query->room_id != -1 && eq->room_id != query->room_id)
        return False;
    

    return True;  // ��������������
}

void PrintEquipment(void* data)
{
    ExperimentalEquipment* eq = (ExperimentalEquipment*)data;
    printf("ID: %d, ����: %s, ���: %s, �۸�: %d, ���������: %d\n",
        eq->id, eq->name, eq->category->name, eq->price, eq->room_id);
}

void QueryEquipmentMenu()
{
    char name[EQUIPMENT_LENGTH] = "";    // �豸����
    int category_id = -1;                // �豸���ID��-1 ����ɸѡ
    int room_id = -1;                    // �豸����ʵ����ID��-1 ����ɸѡ
    double min_price = -1;               // �豸��ͼ۸�-1 ����ɸѡ
    double max_price = -1;               // �豸��߼۸�-1 ����ɸѡ
    char date[DATE_LENGTH] = "";         // �豸�������� 
    int id = -1;                         // �豸id

    system("cls");
    printf("---          ��ѯʵ���豸��Ϣ          ---\n\n");

    // ѯ���豸���ƣ���ѡ��
    printf("--- �����豸���ƣ����س�������-> ");
    while (getchar() != '\n');
    fgets(name, EQUIPMENT_LENGTH, stdin);
    name[strcspn(name, "\n")] = 0; // ȥ�����з�'
    fflush(stdin);
    system("cls");

    // ѯ���豸 ID����ѡ��
    printf("--- �����豸 ID�����س�������-> ");
    char input[20];
    fgets(input, 20, stdin);
    if (strlen(input) > 1)
    {
        id = atoi(input);
    }
    fflush(stdin);
    system("cls");

    // ѯ����� ID����ѡ��
    printf("--- �����豸��� ID�����س�������-> ");
    fgets(input, 20, stdin);
    if (strlen(input) > 1) 
    {
        category_id = atoi(input);
    }
    fflush(stdin);
    system("cls");

    // ѯ���豸����ʱ�䣨��ѡ��
    printf("--- �����豸����ʱ�䣨���س�������-> ");
    fgets(date, DATE_LENGTH, stdin);
    date[strcspn(date, "\n")] = 0; // ȥ�����з�
    fflush(stdin);
    system("cls");

    // ѯ��ʵ���� ID����ѡ��
    printf("--- ����ʵ���� ID�����س�������-> ");
    fgets(input, 20, stdin);
    if (strlen(input) > 1) 
    {
        room_id = atoi(input);
    }
    fflush(stdin);
    system("cls");

    // ѯ�ʼ۸����䣨��ѡ��
    printf("--- ������ͼ۸񣨰��س�������-> ");
    fgets(input, 20, stdin);
    if (strlen(input) > 1) 
    {
        min_price = atof(input);
    }

    printf("--- ������߼۸񣨰��س�������-> ");
    fgets(input, 20, stdin);
    if (strlen(input) > 1) 
    {
        max_price = atof(input);
    }

    LinkedList* list = GetResourceManage()->equipment_list;
    EquipmentQuery query;

    strcpy_s(query.name, EQUIPMENT_LENGTH, name);
    query.category_id = category_id;
    query.room_id = room_id;
    query.min_price = min_price;
    query.max_price = max_price;
    query.id = id;
    strcpy_s(query.purchase_date, DATE_LENGTH, date);
    
    Query(list, QueryEquipmenCondition, &query, PrintEquipment);
}

typedef struct _AccountQuery
{
    AccountType account_type;            // �˻����
    char user_name[USER_NMAE_LENGTH];    // �˻�����
    int id;                              //�˻�ID
    int room_id;                          //����ʵ����id
} AccountQuery;

bool QueryAccountCondition(void* data, void* param)
{
    Account* account = (Account*)data;
    AccountQuery* query = (AccountQuery*)param;

    // �ж������� query->category ���ǿգ���ƥ�䣩
     if (query->account_type != Unknow && account->account_type != query->account_type)
        return False;

    if (query->id != -1 && query->id != account->id)
        return False;


    // �ж��豸���ƣ�ģ��ƥ�䣩
    if (strlen(query->user_name) > 0 && strstr(account->user_name, query->user_name) == NULL)
        return False;

    // �ж���������
    if (query->room_id != -1 && account->roomid != query->room_id)
        return False;


    return True;  // ��������������
}

void PrintAccount(void* data)
{
    Account* account = (Account*)data;
    char type[10] = "";
    switch (account->account_type)
    {
    case Admin:
        strcpy_s(type, 10, "����Ա");
        break;
    case Experimenter:
        strcpy_s(type, 10, "ʵ��Ա");
        break;
    case User:
        strcpy_s(type, 10, "һ���û�");
        break;
    default:
        break;
    }

    printf("ID: %d, �˻�����: %s, �˻�����: %s, ���������: %d\n",
        account->id, type, account->user_name, account->roomid);
}

void QueryAccountMenu()
{
    char name[USER_NMAE_LENGTH] = "";              // �˻�����
    AccountType account_type = Unknow;             // ���ID
    int room_id = -1;                              // �˻�����ʵ����ID
    int id = -1;                                   // �˻�id

    system("cls");
    printf("---          ��ѯ�˻���Ϣ          ---\n\n");

    // ѯ�����ƣ���ѡ��
    printf("--- �����˻��û��������س�������-> ");
    while (getchar() != '\n');
    fgets(name, USER_NMAE_LENGTH, stdin);
    name[strcspn(name, "\n")] = 0; // ȥ�����з�'
    fflush(stdin);
    system("cls");

    // ѯ�� ID����ѡ��
    printf("--- �����˻� ID�����س�������-> ");
    char input[20];
    fgets(input, 20, stdin);
    if (strlen(input) > 1)
    {
        id = atoi(input);
    }
    fflush(stdin);
    system("cls");

    // ѯ����𣨿�ѡ��
    printf("--- 1.����Ա\n");
    printf("--- 2.ʵ��Ա\n");
    printf("--- 3.һ���û�\n");
    printf("--- �����˻���𣨰��س�������-> ");
    fgets(input, 20, stdin);
    if (strlen(input) > 1)
    {
        int temp = atoi(input);
        if (temp >= 4 || temp <= 0)
        {
            printf("�Ƿ�ָ��\n");
            system("pause");
            return;
        }
        account_type = (AccountType)(temp - 1);
    }
    fflush(stdin);
    system("cls");


    // ѯ��ʵ���� ID����ѡ��
    printf("--- ����ʵ���� ID�����س�������-> ");
    fgets(input, 20, stdin);
    if (strlen(input) > 1)
    {
        room_id = atoi(input);
    }
    fflush(stdin);
    system("cls");


    LinkedList* list = GetResourceManage()->account_list;
    AccountQuery query;

    strcpy_s(query.user_name, USER_NMAE_LENGTH, name);
    query.room_id = room_id;
    query.id = id;
    query.account_type = account_type;

    Query(list, QueryAccountCondition, &query, PrintAccount);
}

typedef struct _LabroomQuery
{
    char name[LABROOM_LENGTH];           // ʵ��������
    int id;                              // ʵ����ID
} LabroomQuery;

bool QueryLabroomCondition(void* data, void* param)
{
    LabRoom* Labroom = (LabRoom*)data;
    LabroomQuery* query = (LabroomQuery*)param;

    if (query->id != -1 && query->id != Labroom->id)
        return False;

    // �ж����ƣ�ģ��ƥ�䣩
    if (strlen(query->name) > 0 && strstr(Labroom->name, query->name) == NULL)
        return False;

    return True;  // ��������������
}

void PrintLabroom(void* data)
{
    LabRoom* Labroom = (LabRoom*)data;
 
    printf("ID: %d, ʵ��������: %s\n",
        Labroom->id, Labroom->name);
}

void QueryLabroomMenu()
{
    char name[LABROOM_LENGTH] = "";                // ʵ��������
    int id = -1;                                   // ʵ����id

    system("cls");
    printf("---          ��ѯʵ������Ϣ          ---\n\n");

    // ѯ�����ƣ���ѡ��
    printf("--- ����ʵ�������֣����س�������-> ");
    while (getchar() != '\n');
    fgets(name, LABROOM_LENGTH, stdin);
    name[strcspn(name, "\n")] = 0; // ȥ�����з�'
    fflush(stdin);
    system("cls");

    // ѯ�� ID����ѡ��
    printf("--- ����ʵ���� ID�����س�������-> ");
    char input[20];
    fgets(input, 20, stdin);
    if (strlen(input) > 1)
    {
        id = atoi(input);
    }
    fflush(stdin);
    system("cls");

    LinkedList* list = GetResourceManage()->laboratory_list;
    LabroomQuery query;

    strcpy_s(query.name, LABROOM_LENGTH, name);
    query.id = id;

    Query(list, QueryLabroomCondition, &query, PrintLabroom);
}

typedef struct _CategoryQuery
{
    char name[LABROOM_LENGTH];           // ʵ��������
    int id;                              // ʵ����ID
} CategoryQuery;

bool QueryCategoryCondition(void* data, void* param)
{
    Category* category = (Category*)data;
    CategoryQuery* query = (CategoryQuery*)param;

    if (query->id != -1 && query->id != category->id)
        return False;

    // �ж����ƣ�ģ��ƥ�䣩
    if (strlen(query->name) > 0 && strstr(category->name, query->name) == NULL)
        return False;

    return True;  // ��������������
}

void PrintCategory(void* data)
{
    Category* category = (Category*)data;

    printf("ID: %d, �������: %s, ��������: %d\n",
        category->id, category->name,category->disposal_years);
}

void QueryCategoryMenu()
{
    char name[CATEGORY_LENGTH] = "";                // �������
    int id = -1;                                    // ���id

    system("cls");
    printf("---          ��ѯ�����Ϣ          ---\n\n");

    // ѯ�����ƣ���ѡ��
    printf("--- ����������֣����س�������-> ");
    while (getchar() != '\n');
    fgets(name, LABROOM_LENGTH, stdin);
    name[strcspn(name, "\n")] = 0; // ȥ�����з�'
    fflush(stdin);
    system("cls");

    // ѯ�� ID����ѡ��
    printf("--- ������� ID�����س�������-> ");
    char input[20];
    fgets(input, 20, stdin);
    if (strlen(input) > 1)
    {
        id = atoi(input);
    }
    fflush(stdin);
    system("cls");

    LinkedList* list = GetResourceManage()->category_list;
    CategoryQuery query;

    strcpy_s(query.name, LABROOM_LENGTH, name);
    query.id = id;

    Query(list, QueryCategoryCondition, &query, PrintCategory);
}

void CreateButtonsQuery(HWND hWnd, int tabIndex)
{
    HWND h;
    int idc = IDC_EDIT_NAME;
    for (int i = idc; i <= 3011; i++)
    {
        while ((h = GetDlgItem(hWnd, i)) != NULL)
            DestroyWindow(h);
    }

    ResourceManager* rm = GetResourceManage();

    switch (tabIndex)
    {
    case 0:
    {
        CreateWindow(L"BUTTON", L"��ѯ", WS_VISIBLE | WS_CHILD,
            600, 80, 120, 40, hWnd, (HMENU)IDC_BUTTON_QUERY, GetModuleHandle(NULL), NULL);


        CreateWindow(L"STATIC", L"�豸123", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 60, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 60, 130, 25, hWnd, (HMENU)IDC_EDIT_NAME, NULL, NULL);

        CreateWindow(L"STATIC", L"�豸�۸�", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 100, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 100, 130, 25, hWnd, (HMENU)IDC_EDIT_PRICE, NULL, NULL);

        CreateWindow(L"STATIC", L"����ʵ����ID", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 140, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 140, 130, 25, hWnd, (HMENU)IDC_EDIT_ROOM_ID, NULL, NULL);

        CreateWindow(L"STATIC", L"����ʱ��", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 300, 60, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindowEx(0, DATETIMEPICK_CLASS, NULL,
            WS_BORDER | WS_CHILD | WS_VISIBLE | DTS_SHORTDATEFORMAT,
            400, 60, 130, 25, hWnd, (HMENU)IDC_DATEPICKER_DATE, GetModuleHandle(NULL), NULL);

        HWND hListView = CreateWindowExW(0, WC_LISTVIEW, NULL,
            WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS,
            30, 200, 280, 260, hWnd, (HMENU)IDC_LISTVIEW, GetModuleHandle(NULL), NULL);

        break;
    }
    }
}

void CreateTabControlQuery(HWND hWnd)
{
    hMainTab = CreateWindow(WC_TABCONTROL, L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
        20, 20, 750, 460, hWnd, (HMENU)IDC_TAB, GetModuleHandle(NULL), NULL);

    TCITEM tie;
    tie.mask = TCIF_TEXT;

    wchar_t* tabNames[] = { L"�豸", L"����", L"ʵ��Ա", L"���" };
    for (int i = 0; i < 4; i++)
    {
        tie.pszText = tabNames[i];
        TabCtrl_InsertItem(hMainTab, i, &tie);
    }

    CreateButtonsQuery(hWnd, 0);
}

LRESULT CALLBACK QueryWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        CreateTabControlQuery(hWnd);
        break;

    case WM_NOTIFY:
        if (((LPNMHDR)lParam)->idFrom == IDC_TAB && ((LPNMHDR)lParam)->code == TCN_SELCHANGE)
        {
            int tabIndex = TabCtrl_GetCurSel(hMainTab);
            CreateButtonsQuery(hWnd, tabIndex);
        }
        break;

    case WM_COMMAND:
    {
        int tabIndex = TabCtrl_GetCurSel(hMainTab);
        switch (LOWORD(wParam))
        {
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;

        case WM_DESTROY:
            hwndfuncQuery = NULL;
            ShowWindow(hwndAdminManagement, SW_SHOW);
            break;
        }
    }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void ShowQueryWindow(HWND hWnd)
{
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = QueryWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"QueryWindow";

    RegisterClass(&wc);

    hwndfuncQuery = CreateWindow(L"QueryWindow", L"��Ϣ��ѯ",
        WS_OVERLAPPED | WS_SYSMENU, 200, 100, 800, 600, hWnd, NULL, GetModuleHandle(NULL), NULL);

    ShowWindow(hwndfuncQuery, SW_SHOW);
    UpdateWindow(hwndfuncQuery);
}