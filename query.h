#pragma once
#include "experimental_equipment.h"
#include "category.h"
#include "lab_room.h"
#include "account.h"
#include "configs.h"

#include <Windows.h>
#include <CommCtrl.h>
#include <wchar.h> 

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
#define IDC_CHECKBOX_DATE 3013
#define IDC_CHECKBOX_CATEGORY 3014

extern HWND hwndAdminManagement;

HWND hwndfuncQuery;
HWND hMainTab;

typedef bool (*QueryCondition)(void* data, void* param);

void Query(LinkedList* list, QueryCondition condition, void* param, void (*PrintResult)(void*))
{
    if (list == NULL || condition == NULL || PrintResult == NULL) {
        printf("查询失败: 参数无效！\n");
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
        printf("未找到符合条件的记录。\n\n");
        system("pause");
    }
}

// 用于GUI界面的查询函数
void _Query(LinkedList* list, QueryCondition condition, void* param, void (*AddListView)(HWND,void*), HWND hWnd)
{
    Node* node = list->head;

    while (node->next != NULL)
    {
        node = node->next;
        if (condition(node->data, param))
        {
            AddListView(hWnd, node->data);
        }
    }
}

typedef struct _EquipmentQuery
{
    int id;                               // ID
    int category_id;                      // 类别
    wchar_t max_date[DATE_LENGTH];        // 最晚购入日期
    wchar_t min_date[DATE_LENGTH];        // 最早购入日期
    int min_price;                        // 最小价格
    int max_price;                        // 最大价格
    wchar_t name[EQUIPMENT_LENGTH];       // 名字
    int room_id;                          // 所属房间

} EquipmentQuery;

bool QueryEquipmenCondition(void* data, void* param)
{
    ExperimentalEquipment* eq = (ExperimentalEquipment*)data;
    EquipmentQuery* query = (EquipmentQuery*)param;

    // 判断类别
    if (query->category_id != -1 && eq->category->id != query->category_id)
        return False;

    // 判断购入日期
    if (wcslen(query->min_date) > 0 && wcscmp(eq->purchase_date, query->min_date) < 0)
        return False;

    if (wcslen(query->max_date) > 0 && wcscmp(eq->purchase_date, query->max_date) > 0)
        return False;

    // 判断价格范围
    if ((query->min_price >= 0 && eq->price < query->min_price) ||
        (query->max_price >= 0 && eq->price > query->max_price))
        return False;

    // 判断设备名称（模糊匹配）
    if (wcslen(query->name) > 0 && wcsstr(eq->name, query->name) == NULL)
        return False;

    // 判断所属房间
    if (query->room_id != -1 && eq->room_id != query->room_id)
        return False;
    

    return True;  // 所有条件都符合
}

void PrintEquipment(void* data)
{
    ExperimentalEquipment* eq = (ExperimentalEquipment*)data;
    printf("ID: %d, 名称: %s, 类别: %s, 价格: %d, 所属房间号: %d\n",
        eq->id, eq->name, eq->category->name, eq->price, eq->room_id);
}

void AddEquiment(HWND hWnd, void* data)
{
    ExperimentalEquipment* ee = (ExperimentalEquipment*)data;

    HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
    int index = ListView_GetItemCount(hListView);

    LVITEM lvItem;
    lvItem.mask = LVIF_TEXT;
    lvItem.iItem = index;
    lvItem.iSubItem = 0;

    wchar_t idBuffer[16];
    swprintf_s(idBuffer, 16, L"%d", ee->id);
    lvItem.pszText = idBuffer;
    ListView_InsertItem(hListView, &lvItem);

    ListView_SetItemText(hListView, index, 1, ee->name);

    ListView_SetItemText(hListView, index, 2, ee->category->name);

    wchar_t roomBuffer[16];
    swprintf_s(roomBuffer, 16, L"%d", ee->room_id);
    ListView_SetItemText(hListView, index, 3, roomBuffer);

    wchar_t priceBuffer[16];
    swprintf_s(priceBuffer, 16, L"%d", ee->price);
    ListView_SetItemText(hListView, index, 4, priceBuffer);

    ListView_SetItemText(hListView, index, 5, ee->purchase_date)
}

void _QueryEquipment(HWND hWnd)
{
    EquipmentQuery query;
    query.room_id = -1;
    query.id = -1;
    query.max_price = -1;
    query.min_price = -1;
    query.category_id = -1; 
    wcscpy_s(query.min_date, DATE_LENGTH, L"");
    wcscpy_s(query.max_date, DATE_LENGTH, L"");
    wcscpy_s(query.name, EQUIPMENT_LENGTH, L"");
    
    HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_ROOM_ID);
    if (GetWindowTextLength(hEdit) != 0)
    {
        int room_id = GetInputNumber(hWnd, IDC_EDIT_ROOM_ID);
        query.room_id = room_id;
    }
    hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME);
    if (GetWindowTextLength(hEdit) != 0)
    {
        GetDlgItemText(hWnd, IDC_EDIT_NAME, query.name, 50);
    }
    hEdit = GetDlgItem(hWnd, IDC_EDIT_MIN_PRICE);
    if (GetWindowTextLength(hEdit) != 0)
    {
        int min_price = GetInputNumber(hWnd, IDC_EDIT_MIN_PRICE);
        query.min_price = min_price;
    }
    hEdit = GetDlgItem(hWnd, IDC_EDIT_MAX_PRICE);
    if (GetWindowTextLength(hEdit) != 0)
    {
        int max_price = GetInputNumber(hWnd, IDC_EDIT_MAX_PRICE);
        query.max_price = max_price;
    }
    HWND hCheckBox = GetDlgItem(hWnd, IDC_CHECKBOX_DATE);
    if (SendMessage(hCheckBox, BM_GETCHECK, 0, 0) == BST_CHECKED)
    {
        SYSTEMTIME st;
        HWND hDatePicker = GetDlgItem(hWnd, IDC_MIN_DATE);
        SendMessage(hDatePicker, DTM_GETSYSTEMTIME, 0, (LPARAM)&st);
        swprintf(query.min_date, 11, L"%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
        hDatePicker = GetDlgItem(hWnd, IDC_MAX_DATE);
        SendMessage(hDatePicker, DTM_GETSYSTEMTIME, 0, (LPARAM)&st);
        swprintf(query.max_date, 11, L"%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
    }

    hCheckBox = GetDlgItem(hWnd, IDC_CHECKBOX_CATEGORY);
    if (SendMessage(hCheckBox, BM_GETCHECK, 0, 0) == BST_CHECKED)
    {
        HWND hComboBox = GetDlgItem(hWnd, IDC_COMBOX_CATEGORY);
        int index = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
        Category* category = LinkedList_at(GetResourceManage()->category_list, index);
        query.category_id = category->id;
    }
    HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
    ListView_DeleteAllItems(hListView);

    LinkedList* list = GetResourceManage()->equipment_list;
    _Query(list, QueryEquipmenCondition, &query, AddEquiment,hWnd);

    MessageBox(hWnd, L"查询成功", L"提示", MB_OK);
}

void QueryEquipmentMenu()
{
    char name[EQUIPMENT_LENGTH] = "";    // 设备名称
    int category_id = -1;                // 设备类别ID，-1 代表不筛选
    int room_id = -1;                    // 设备所在实验室ID，-1 代表不筛选
    double min_price = -1;               // 设备最低价格，-1 代表不筛选
    double max_price = -1;               // 设备最高价格，-1 代表不筛选
    char date[DATE_LENGTH] = "";         // 设备购买日期 
    int id = -1;                         // 设备id

    system("cls");
    printf("---          查询实验设备信息          ---\n\n");

    // 询问设备名称（可选）
    printf("--- 输入设备名称（按回车跳过）-> ");
    while (getchar() != '\n');
    fgets(name, EQUIPMENT_LENGTH, stdin);
    name[strcspn(name, "\n")] = 0; // 去掉换行符'
    fflush(stdin);
    system("cls");

    // 询问设备 ID（可选）
    printf("--- 输入设备 ID（按回车跳过）-> ");
    char input[20];
    fgets(input, 20, stdin);
    if (strlen(input) > 1)
    {
        id = atoi(input);
    }
    fflush(stdin);
    system("cls");

    // 询问类别 ID（可选）
    printf("--- 输入设备类别 ID（按回车跳过）-> ");
    fgets(input, 20, stdin);
    if (strlen(input) > 1) 
    {
        category_id = atoi(input);
    }
    fflush(stdin);
    system("cls");

    // 询问设备购买时间（可选）
    printf("--- 输入设备购买时间（按回车跳过）-> ");
    fgets(date, DATE_LENGTH, stdin);
    date[strcspn(date, "\n")] = 0; // 去掉换行符
    fflush(stdin);
    system("cls");

    // 询问实验室 ID（可选）
    printf("--- 输入实验室 ID（按回车跳过）-> ");
    fgets(input, 20, stdin);
    if (strlen(input) > 1) 
    {
        room_id = atoi(input);
    }
    fflush(stdin);
    system("cls");

    // 询问价格区间（可选）
    printf("--- 输入最低价格（按回车跳过）-> ");
    fgets(input, 20, stdin);
    if (strlen(input) > 1) 
    {
        min_price = atof(input);
    }

    printf("--- 输入最高价格（按回车跳过）-> ");
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
    //strcpy_s(query.purchase_date, DATE_LENGTH, date);
    
    Query(list, QueryEquipmenCondition, &query, PrintEquipment);
}

typedef struct _AccountQuery
{
    AccountType account_type;               // 账户类别
    wchar_t user_name[USER_NMAE_LENGTH];    // 账户名称
    int id;                                 //账户ID
    int room_id;                            //所属实验室id
} AccountQuery;

bool QueryAccountCondition(void* data, void* param)
{
    Account* account = (Account*)data;
    AccountQuery* query = (AccountQuery*)param;

    // 判断类别（如果 query->category 不是空，就匹配）
     if (query->account_type != Unknow && account->account_type != query->account_type)
        return False;

    // 判断设备名称（模糊匹配）
    if (wcslen(query->user_name) > 0 && wcsstr(account->user_name, query->user_name) == NULL)
        return False;

    // 判断所属房间
    if (query->room_id != -1 && account->roomid != query->room_id)
        return False;


    return True;  // 所有条件都符合
}

void PrintAccount(void* data)
{
    Account* account = (Account*)data;
    char type[10] = "";
    switch (account->account_type)
    {
    case Admin:
        strcpy_s(type, 10, "管理员");
        break;
    case Experimenter:
        strcpy_s(type, 10, "实验员");
        break;
    case User:
        strcpy_s(type, 10, "一般用户");
        break;
    default:
        break;
    }

    printf("ID: %d, 账户类型: %s, 账户名称: %s, 所属房间号: %d\n",
        account->id, type, account->user_name, account->roomid);
}

void AddAccount(HWND hWnd, void* data)
{
    Account* account = (Account*)data;

    HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
    int index = ListView_GetItemCount(hListView);

    LVITEM lvItem;
    lvItem.mask = LVIF_TEXT;
    lvItem.iItem = index;
    lvItem.iSubItem = 0;

    wchar_t idBuffer[16];
    swprintf_s(idBuffer, 16, L"%d", account->id);
    lvItem.pszText = idBuffer;
    ListView_InsertItem(hListView, &lvItem);

    ListView_SetItemText(hListView, index, 1, account->user_name);

    wchar_t Buffer[16];
    switch (account->account_type)
    {
    case Admin:
        wcscpy_s(Buffer, 16, L"管理员");
        break;
    case Experimenter:
        wcscpy_s(Buffer, 16, L"实验员");
        break;
    case User:
        wcscpy_s(Buffer, 16, L"一般用户");
        break;
    }
    ListView_SetItemText(hListView, index, 3, Buffer);

    wchar_t roomBuffer[16];
    swprintf_s(roomBuffer, 16, L"%d", account->roomid);
    ListView_SetItemText(hListView, index, 2, roomBuffer);
}

void QueryAccountMenu()
{
    char name[USER_NMAE_LENGTH] = "";              // 账户名称
    AccountType account_type = Unknow;             // 类别ID
    int room_id = -1;                              // 账户所在实验室ID
    int id = -1;                                   // 账户id

    system("cls");
    printf("---          查询账户信息          ---\n\n");

    // 询问名称（可选）
    printf("--- 输入账户用户名（按回车跳过）-> ");
    while (getchar() != '\n');
    fgets(name, USER_NMAE_LENGTH, stdin);
    name[strcspn(name, "\n")] = 0; // 去掉换行符'
    fflush(stdin);
    system("cls");

    // 询问 ID（可选）
    printf("--- 输入账户 ID（按回车跳过）-> ");
    char input[20];
    fgets(input, 20, stdin);
    if (strlen(input) > 1)
    {
        id = atoi(input);
    }
    fflush(stdin);
    system("cls");

    // 询问类别（可选）
    printf("--- 1.管理员\n");
    printf("--- 2.实验员\n");
    printf("--- 3.一般用户\n");
    printf("--- 输入账户类别（按回车跳过）-> ");
    fgets(input, 20, stdin);
    if (strlen(input) > 1)
    {
        int temp = atoi(input);
        if (temp >= 4 || temp <= 0)
        {
            printf("非法指令\n");
            system("pause");
            return;
        }
        account_type = (AccountType)(temp - 1);
    }
    fflush(stdin);
    system("cls");


    // 询问实验室 ID（可选）
    printf("--- 输入实验室 ID（按回车跳过）-> ");
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

void _QueryAccount(HWND hWnd)
{
    AccountQuery query;
    query.id = -1;
    query.room_id = -1;
    query.account_type = Unknow;
    wcscpy_s(query.user_name, DATE_LENGTH, L"");

    HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_ROOM_ID);
    if (GetWindowTextLength(hEdit) != 0)
    {
        int room_id = GetInputNumber(hWnd, IDC_EDIT_ROOM_ID);
        query.room_id = room_id;
    }
    hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME);
    if (GetWindowTextLength(hEdit) != 0)
    {
        GetDlgItemText(hWnd, IDC_EDIT_NAME, query.user_name, 50);
    }
    
    HWND hCheckBox = GetDlgItem(hWnd, IDC_CHECKBOX_CATEGORY);
    if (SendMessage(hCheckBox, BM_GETCHECK, 0, 0) == BST_CHECKED)
    {
        HWND hComboBox = GetDlgItem(hWnd, IDC_COMBOX_CATEGORY);
        int index = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
        switch (index)
        {
        case 0:
            query.account_type = Admin;
            break;
        case 1:
            query.account_type = Experimenter;
            break;
        case 2:
            query.account_type = User;
        }
    }
    HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
    ListView_DeleteAllItems(hListView);

    LinkedList* list = GetResourceManage()->account_list;
    _Query(list, QueryAccountCondition, &query, AddAccount, hWnd);

    MessageBox(hWnd, L"查询成功", L"提示", MB_OK);
}

typedef struct _LabroomQuery
{
    wchar_t name[LABROOM_LENGTH];           // 实验室名称
    int id;                              // 实验室ID
} LabroomQuery;

bool QueryLabroomCondition(void* data, void* param)
{
    LabRoom* Labroom = (LabRoom*)data;
    LabroomQuery* query = (LabroomQuery*)param;

    if (query->id != -1 && query->id != Labroom->id)
        return False;

    // 判断名称（模糊匹配）
    if (wcslen(query->name) > 0 && wcsstr(Labroom->name, query->name) == NULL)
        return False;

    return True;  // 所有条件都符合
}

void PrintLabroom(void* data)
{
    LabRoom* Labroom = (LabRoom*)data;
 
    printf("ID: %d, 实验室名字: %s\n",
        Labroom->id, Labroom->name);
}

void AddLabroom(HWND hWnd, void* data)
{
    LabRoom* labroom = (LabRoom*)data;

    HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
    int index = ListView_GetItemCount(hListView);

    LVITEM lvItem;
    lvItem.mask = LVIF_TEXT;
    lvItem.iItem = index;
    lvItem.iSubItem = 0;

    wchar_t idBuffer[16];
    swprintf_s(idBuffer, 16, L"%d", labroom->id);
    lvItem.pszText = idBuffer;
    ListView_InsertItem(hListView, &lvItem);

    ListView_SetItemText(hListView, index, 1, labroom->name);
}

void QueryLabroomMenu()
{
    char name[LABROOM_LENGTH] = "";                // 实验室名称
    int id = -1;                                   // 实验室id

    system("cls");
    printf("---          查询实验室信息          ---\n\n");

    // 询问名称（可选）
    printf("--- 输入实验室名字（按回车跳过）-> ");
    while (getchar() != '\n');
    fgets(name, LABROOM_LENGTH, stdin);
    name[strcspn(name, "\n")] = 0; // 去掉换行符'
    fflush(stdin);
    system("cls");

    // 询问 ID（可选）
    printf("--- 输入实验室 ID（按回车跳过）-> ");
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

void _QueryLabroom(HWND hWnd)
{
    LabroomQuery query;
    query.id = -1;
    wcscpy_s(query.name, 50, L"");

    HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME);
    if (GetWindowTextLength(hEdit) != 0)
    {
        GetDlgItemText(hWnd, IDC_EDIT_NAME, query.name, 50);
    }
    hEdit = GetDlgItem(hWnd, IDC_EDIT_ROOM_ID);
    if (GetWindowTextLength(hEdit) != 0)
    {
        int room_id = GetInputNumber(hWnd, IDC_EDIT_ROOM_ID);
        query.id = room_id;
    }

    HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
    ListView_DeleteAllItems(hListView);

    LinkedList* list = GetResourceManage()->laboratory_list;
    _Query(list, QueryLabroomCondition, &query, AddLabroom, hWnd);

    MessageBox(hWnd, L"查询成功", L"提示", MB_OK);
}

typedef struct _CategoryQuery
{
    wchar_t name[LABROOM_LENGTH];           // 类别名称
    int min_year;                           // 最小报废年限
    int max_year;                           // 最大报废年限
} CategoryQuery;

bool QueryCategoryCondition(void* data, void* param)
{
    Category* category = (Category*)data;
    CategoryQuery* query = (CategoryQuery*)param;

    // 判断名称（模糊匹配）
    if (strlen(query->name) > 0 && strstr(category->name, query->name) == NULL)
        return False;

    // 判断年限范围
    if ((query->min_year >= 0 && category->disposal_years < query->min_year) ||
        (query->max_year >= 0 && category->disposal_years > query->max_year))
        return False;

    return True;  // 所有条件都符合
}

void PrintCategory(void* data)
{
    Category* category = (Category*)data;

    printf("ID: %d, 类别名字: %s, 报废年限: %d\n",
        category->id, category->name,category->disposal_years);
}

void Addcategory(HWND hWnd, void* data)
{
    Category* category = (Category*)data;

    HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
    int index = ListView_GetItemCount(hListView);

    LVITEM lvItem;
    lvItem.mask = LVIF_TEXT;
    lvItem.iItem = index;
    lvItem.iSubItem = 0;

    wchar_t idBuffer[16];
    swprintf_s(idBuffer, 16, L"%d", category->id);
    lvItem.pszText = idBuffer;
    ListView_InsertItem(hListView, &lvItem);

    ListView_SetItemText(hListView, index, 1, category->name);

    wchar_t Buffer[16];
    swprintf_s(Buffer, 16, L"%d", category->disposal_years);
    ListView_SetItemText(hListView, index, 2, Buffer);
}

void QueryCategoryMenu()
{
    char name[CATEGORY_LENGTH] = "";                // 类别名称
    int id = -1;                                    // 类别id

    system("cls");
    printf("---          查询类别信息          ---\n\n");

    // 询问名称（可选）
    printf("--- 输入类别名字（按回车跳过）-> ");
    while (getchar() != '\n');
    fgets(name, LABROOM_LENGTH, stdin);
    name[strcspn(name, "\n")] = 0; // 去掉换行符'
    fflush(stdin);
    system("cls");

    // 询问 ID（可选）
    printf("--- 输入类别 ID（按回车跳过）-> ");
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

    Query(list, QueryCategoryCondition, &query, PrintCategory);
}

void _QueryCategory(HWND hWnd)
{
    CategoryQuery query;
    query.max_year = -1;
    query.min_year = -1;
    wcscpy_s(query.name, 50, L"");

    HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_MIN_PRICE);
    if (GetWindowTextLength(hEdit) != 0)
    {
        int min_price = GetInputNumber(hWnd, IDC_EDIT_MIN_PRICE);
        query.min_year = min_price;
    }
    hEdit = GetDlgItem(hWnd, IDC_EDIT_MAX_PRICE);
    if (GetWindowTextLength(hEdit) != 0)
    {
        int max_price = GetInputNumber(hWnd, IDC_EDIT_MAX_PRICE);
        query.max_year = max_price;
    }
    hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME);
    if (GetWindowTextLength(hEdit) != 0)
    {
        GetDlgItemText(hWnd, IDC_EDIT_NAME, query.name, 50);
    }

    HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
    ListView_DeleteAllItems(hListView);

    LinkedList* list = GetResourceManage()->category_list;
    _Query(list, QueryCategoryCondition, &query, Addcategory, hWnd);

    MessageBox(hWnd, L"查询成功", L"提示", MB_OK);
}

void CreateButtonsQuery(HWND hWnd, int tabIndex)
{
    HWND h;
    int idc = IDC_EDIT_NAME;
    for (int i = idc; i <= 3014; i++)
    {
        while ((h = GetDlgItem(hWnd, i)) != NULL)
            DestroyWindow(h);
    }

    ResourceManager* rm = GetResourceManage();

    switch (tabIndex)
    {
    case 0:
    {
        CreateWindow(L"BUTTON", L"查询", WS_VISIBLE | WS_CHILD,
            50, 425, 120, 40, hWnd, (HMENU)IDC_BUTTON_QUERY, GetModuleHandle(NULL), NULL);

         
        CreateWindow(L"STATIC", L"设备名字", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 60, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 60, 130, 25, hWnd, (HMENU)IDC_EDIT_NAME, NULL, NULL);

        CreateWindow(L"STATIC", L"设备最低价格", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 100, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 100, 130, 25, hWnd, (HMENU)IDC_EDIT_MIN_PRICE, NULL, NULL);

        CreateWindow(L"STATIC", L"设备最高价格", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 140, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 140, 130, 25, hWnd, (HMENU)IDC_EDIT_MAX_PRICE, NULL, NULL);

        CreateWindow(L"STATIC", L"所属实验室ID", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 180, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 180, 130, 25, hWnd, (HMENU)IDC_EDIT_ROOM_ID, NULL, NULL);

        CreateWindowEx(0, L"BUTTON", L"是否选择时间条件查询",
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            60, 220, 200, 30, hWnd, (HMENU)IDC_CHECKBOX_DATE, GetModuleHandle(NULL), NULL);

        CreateWindow(L"STATIC", L"最早购买时间", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 260, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindowEx(0, DATETIMEPICK_CLASS, NULL,
            WS_BORDER | WS_CHILD | WS_VISIBLE | DTS_SHORTDATEFORMAT,
            160, 260, 130, 25, hWnd, (HMENU)IDC_MIN_DATE, GetModuleHandle(NULL), NULL);

        CreateWindow(L"STATIC", L"最晚购买时间", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 300, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindowEx(0, DATETIMEPICK_CLASS, NULL,
            WS_BORDER | WS_CHILD | WS_VISIBLE | DTS_SHORTDATEFORMAT,
            160, 300, 130, 25, hWnd, (HMENU)IDC_MAX_DATE, GetModuleHandle(NULL), NULL);

        CreateWindowEx(0, L"BUTTON", L"是否选择种类条件查询",
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            60, 340, 200, 30, hWnd, (HMENU)IDC_CHECKBOX_CATEGORY, GetModuleHandle(NULL), NULL);

        CreateWindow(L"STATIC", L"设备种类", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 380, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        HWND hComboBox = CreateWindow(L"COMBOBOX", NULL,
            WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_BORDER,
            160, 380, 130, 300, hWnd, (HMENU)IDC_COMBOX_CATEGORY, GetModuleHandle(NULL), NULL);

        Node* temp = rm->category_list->head;
        size_t count = rm->category_list->size;
        for (size_t i = 0; i < count; i++)
        {
            temp = temp->next;
            Category* category = (Category*)temp->data;
            wchar_t buffer[50];
            swprintf(buffer, 50, L"%d. %s", (int)(i + 1), category->name);
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)buffer);
        }

        CreateWindow(L"STATIC", L"查询结果", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 490, 50, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);

        HWND hListView = CreateWindowExW(0, WC_LISTVIEW, NULL,
            WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS,
            360, 80, 360, 390, hWnd, (HMENU)IDC_LISTVIEW, GetModuleHandle(NULL), NULL);

        ListView_DeleteAllItems(hListView);

        LVCOLUMNW lvc;
        lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

        lvc.pszText = L"设备ID";
        lvc.cx = 60;
        lvc.iSubItem = 0;
        ListView_InsertColumn(hListView, 0, &lvc);

        lvc.pszText = L"设备名称";
        lvc.cx = 90;
        lvc.iSubItem = 1;
        ListView_InsertColumn(hListView, 1, &lvc);

        lvc.pszText = L"设备类型";
        lvc.cx = 80;
        lvc.iSubItem = 2;
        ListView_InsertColumn(hListView, 2, &lvc);

        lvc.pszText = L"所属实验室";
        lvc.cx = 80;
        lvc.iSubItem = 3;
        ListView_InsertColumn(hListView, 3, &lvc);

        lvc.pszText = L"价格";
        lvc.cx = 60;
        lvc.iSubItem = 4;
        ListView_InsertColumn(hListView, 4, &lvc);

        lvc.pszText = L"购买日期";
        lvc.cx = 80;
        lvc.iSubItem = 5;
        ListView_InsertColumn(hListView, 5, &lvc);

        break;
    }
    case 1:
    {
        CreateWindow(L"BUTTON", L"查询", WS_VISIBLE | WS_CHILD,
            50, 425, 120, 40, hWnd, (HMENU)IDC_BUTTON_QUERY, GetModuleHandle(NULL), NULL);

        CreateWindow(L"STATIC", L"实验室名字", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 60, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 60, 130, 25, hWnd, (HMENU)IDC_EDIT_NAME, NULL, NULL);

        CreateWindow(L"STATIC", L"实验室ID", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 120, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 120, 130, 25, hWnd, (HMENU)IDC_EDIT_ROOM_ID, NULL, NULL);

        CreateWindow(L"STATIC", L"查询结果", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 490, 50, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);

        HWND hListView = CreateWindowExW(0, WC_LISTVIEW, NULL,
            WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS,
            360, 80, 360, 390, hWnd, (HMENU)IDC_LISTVIEW, GetModuleHandle(NULL), NULL);

        ListView_DeleteAllItems(hListView);

        LVCOLUMNW lvc;
        lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

        lvc.pszText = L"实验室ID";
        lvc.cx = 100;
        lvc.iSubItem = 0;
        ListView_InsertColumn(hListView, 0, &lvc);

        lvc.pszText = L"实验室名称";
        lvc.cx = 150;
        lvc.iSubItem = 1;
        ListView_InsertColumn(hListView, 1, &lvc);
        break;
    }
    case 2:
    {
        CreateWindow(L"BUTTON", L"查询", WS_VISIBLE | WS_CHILD,
            50, 425, 120, 40, hWnd, (HMENU)IDC_BUTTON_QUERY, GetModuleHandle(NULL), NULL);

        CreateWindow(L"STATIC", L"用户名", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 60, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 60, 130, 25, hWnd, (HMENU)IDC_EDIT_NAME, NULL, NULL);

        CreateWindow(L"STATIC", L"管理实验室ID", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 120, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 120, 130, 25, hWnd, (HMENU)IDC_EDIT_ROOM_ID, NULL, NULL);

        CreateWindow(L"STATIC", L"账户类别", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 220, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        HWND hComboBox = CreateWindow(L"COMBOBOX", NULL,
            WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_BORDER,
            160, 220, 130, 300, hWnd, (HMENU)IDC_COMBOX_CATEGORY, GetModuleHandle(NULL), NULL);

        wchar_t buffer[50];
        swprintf(buffer, 50, L"%d. %s", 1, L"管理员");
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)buffer);
        swprintf(buffer, 50, L"%d. %s", 2, L"实验员");
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)buffer);
        swprintf(buffer, 50, L"%d. %s", 3, L"一般用户");
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)buffer);

        CreateWindowEx(0, L"BUTTON", L"是否选择类别条件查询",
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            60, 180, 200, 30, hWnd, (HMENU)IDC_CHECKBOX_CATEGORY, GetModuleHandle(NULL), NULL);

        CreateWindow(L"STATIC", L"查询结果", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 490, 50, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);

        HWND hListView = CreateWindowExW(0, WC_LISTVIEW, NULL,
            WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS,
            360, 80, 360, 390, hWnd, (HMENU)IDC_LISTVIEW, GetModuleHandle(NULL), NULL);

        ListView_DeleteAllItems(hListView);

        LVCOLUMNW lvc;
        lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

        lvc.pszText = L"账户ID";
        lvc.cx = 100;
        lvc.iSubItem = 0;
        ListView_InsertColumn(hListView, 0, &lvc);

        lvc.pszText = L"账户类别";
        lvc.cx = 100;
        lvc.iSubItem = 1;
        ListView_InsertColumn(hListView, 1, &lvc);

        lvc.pszText = L"账户用户名";
        lvc.cx = 100;
        lvc.iSubItem = 2;
        ListView_InsertColumn(hListView, 2, &lvc);

        lvc.pszText = L"管理实验室ID";
        lvc.cx = 150;
        lvc.iSubItem = 3;
        ListView_InsertColumn(hListView, 3, &lvc);
        break;
    }
    case 3:
    {
        CreateWindow(L"BUTTON", L"查询", WS_VISIBLE | WS_CHILD,
            50, 425, 120, 40, hWnd, (HMENU)IDC_BUTTON_QUERY, GetModuleHandle(NULL), NULL);


        CreateWindow(L"STATIC", L"类别名字", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 60, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 60, 130, 25, hWnd, (HMENU)IDC_EDIT_NAME, NULL, NULL);

        CreateWindow(L"STATIC", L"最低报废年限", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 100, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 100, 130, 25, hWnd, (HMENU)IDC_EDIT_MIN_PRICE, NULL, NULL);

        CreateWindow(L"STATIC", L"最高报废年限", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 140, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 140, 130, 25, hWnd, (HMENU)IDC_EDIT_MAX_PRICE, NULL, NULL);

        CreateWindow(L"STATIC", L"查询结果", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 490, 50, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);

        HWND hListView = CreateWindowExW(0, WC_LISTVIEW, NULL,
            WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS,
            360, 80, 360, 390, hWnd, (HMENU)IDC_LISTVIEW, GetModuleHandle(NULL), NULL);

        ListView_DeleteAllItems(hListView);

        LVCOLUMNW lvc;
        lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

        lvc.pszText = L"类别ID";
        lvc.cx = 100;
        lvc.iSubItem = 0;
        ListView_InsertColumn(hListView, 0, &lvc);

        lvc.pszText = L"类别名称";
        lvc.cx = 100;
        lvc.iSubItem = 1;
        ListView_InsertColumn(hListView, 1, &lvc);

        lvc.pszText = L"报废年限";
        lvc.cx = 150;
        lvc.iSubItem = 2;
        ListView_InsertColumn(hListView, 2, &lvc);
    }
    }
}

void CreateTabControlQuery(HWND hWnd)
{
    hMainTab = CreateWindow(WC_TABCONTROL, L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
        20, 20, 750, 460, hWnd, (HMENU)IDC_TAB, GetModuleHandle(NULL), NULL);

    TCITEM tie;
    tie.mask = TCIF_TEXT;

    wchar_t* tabNames[] = { L"设备", L"房间", L"账户", L"类别" };
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
        case IDC_BUTTON_QUERY:
            switch (tabIndex)
            {
            case 0:
                _QueryEquipment(hWnd);
                break;
            case 1:
                _QueryLabroom(hWnd);
                break;
            case 2:
                _QueryAccount(hWnd);
                break;
            case 3:
                _QueryCategory(hWnd);
                break;
            default:
                break;
            }
            break;
        }
    }
    case WM_DESTROY:
        hwndfuncQuery = NULL;
        ShowWindow(hwndAdminManagement, SW_SHOW);
        break;
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

    hwndfuncQuery = CreateWindow(L"QueryWindow", L"信息查询",
        WS_OVERLAPPED | WS_SYSMENU, 200, 100, 800, 600, hWnd, NULL, GetModuleHandle(NULL), NULL);

    ShowWindow(hwndfuncQuery, SW_SHOW);
    UpdateWindow(hwndfuncQuery);
}