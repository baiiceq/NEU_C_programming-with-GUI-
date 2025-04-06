#pragma once
#include "query.h"

extern HWND hwndAdminManagement;

HWND hwndfuncSort;
HWND hMainTab;

#define IDC_BUTTON_SORT 4000
#define IDC_CHECK_BOX_NAME_1 4001
#define IDC_CHECK_BOX_NAME_2 4002
#define IDC_CHECK_BOX_ID_1 4003
#define IDC_CHECK_BOX_ID_2 4004
#define IDC_CHECK_BOX_PRICE_1 4005
#define IDC_CHECK_BOX_PRICE_2 4006
#define IDC_CHECK_BOX_DATE_1 4007
#define IDC_CHECK_BOX_DATE_2 4008
#define IDC_CHECK_BOX_CATEGORYID_1 4009
#define IDC_CHECK_BOX_CATEGORYID_2 4010
#define IDC_CHECK_BOX_ROOMID_1 4011
#define IDC_CHECK_BOX_ROOMID_2 4012
#define IDC_STATIC 4013
#define IDC_LISTVIEW 4014


typedef struct _SortAccount
{
    int sortById;    
    int sortByType; 
    int sortByUsername;     
} SortAccount;

int CompareAccount(const void* a, const void* b, void* arg)
{
    Account* accountA = (Account*)a;
    Account* accountB = (Account*)b;

    SortAccount* sort_account = (SortAccount*)arg;

    if (sort_account->sortById != 0)
    {
        int cmp = accountA->id - accountB->id;
        if (cmp != 0) return sort_account->sortById == 1 ? cmp : -cmp;
    }

    if (sort_account->sortByType != 0)
    {
        int cmp = accountA->account_type - accountB->account_type;
        if (cmp != 0) return sort_account->sortByType == 1 ? -cmp : cmp;
    }

    if (sort_account->sortByUsername != 0)
    {
        int cmp = strcmp(accountA->user_name, accountB->user_name);
        if (cmp != 0) return sort_account->sortByUsername == 1 ? cmp : -cmp;
    }

    return 0;
}

void SortAccountMenu()
{
    SortAccount sort_account;
    system("cls");
    printf("---        账户排序设置        ---\n\n");

    while (getchar() != '\n');

    printf("--- 输入账户ID排序方式（1 升序，-1 降序，回车跳过）-> ");
    char input[10];
    fgets(input, sizeof(input), stdin);
    sort_account.sortById = (strlen(input) > 1) ? atoi(input) : 0;

    printf("--- 输入账户类型排序方式（1 升序，-1 降序，回车跳过）-> ");
    fgets(input, sizeof(input), stdin);
    sort_account.sortByType = (strlen(input) > 1) ? atoi(input) : 0;

    printf("--- 输入用户名排序方式（1 升序，-1 降序，回车跳过）-> ");
    fgets(input, sizeof(input), stdin);
    sort_account.sortByUsername = (strlen(input) > 1) ? atoi(input) : 0;

    LinkedList* list = GetResourceManage()->account_list;

    LinkedList_sort(list, CompareAccount, &sort_account);

    LinkedList_print(list, PrintAccount);
}

void _SortAccount(HWND hWnd)
{
    SortAccount sort_account = { 0 };

    HWND hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_ID_1);
    HWND hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_ID_2);
    LRESULT state1 = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
    LRESULT state2 = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);

    if (state1 == BST_CHECKED)
    {
        sort_account.sortById = 1;
    }
    else if (state2 == BST_CHECKED)
    {
        sort_account.sortById = -1;
    }

    hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_NAME_1);
    hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_NAME_2);
    state1 = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
    state2 = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);

    if (state1 == BST_CHECKED)
    {
        sort_account.sortByUsername = 1;
    }
    else if (state2 == BST_CHECKED)
    {
        sort_account.sortByUsername = -1;
    }

    hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_CATEGORYID_1);
    hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_CATEGORYID_2);
    state1 = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
    state2 = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);

    if (state1 == BST_CHECKED)
    {
        sort_account.sortByType = 1;
    }
    else if (state2 == BST_CHECKED)
    {
        sort_account.sortByType = -1;
    }

    LinkedList* list = GetResourceManage()->account_list;

    LinkedList_sort(list, CompareAccount, &sort_account);

    HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
    ListView_DeleteAllItems(hListView);

    Node* temp = list->head;
    while (temp->next)
    {
        temp = temp->next;
        AddAccount(hWnd, temp->data, IDC_LISTVIEW);
    }

    MessageBox(hWnd, L"排序成功", L"提示", MB_OK);
}

typedef struct _SortCategory
{
    int sortById;
    int sortByDisposalYears;
    int sortByName;
} SortCategory;

int CompareCategory(const void* a, const void* b, void* arg)
{
    Category* categoryA = (Category*)a;
    Category* categoryB = (Category*)b;

    SortCategory* sort_category = (SortCategory*)arg;

    if (sort_category->sortById != 0)
    {
        int cmp = categoryA->id - categoryB->id;
        if (cmp != 0) return sort_category->sortById == 1 ? cmp : -cmp;
    }

    if (sort_category->sortByDisposalYears != 0)
    {
        int cmp = categoryA->disposal_years - categoryB->disposal_years;
        if (cmp != 0) return sort_category->sortByDisposalYears == 1 ? cmp : -cmp;
    }

    if (sort_category->sortByName != 0)
    {
        int cmp = wcscmp(categoryA->name, categoryB->name);
        if (cmp != 0) return sort_category->sortByName == 1 ? cmp : -cmp;
    }

    return 0;
}

void SortCategoryMenu()
{
    SortCategory sort_category;
    system("cls");
    printf("---        设备类型排序设置        ---\n\n");

    while (getchar() != '\n');

    printf("--- 输入设备类型ID排序方式（1 升序，-1 降序，回车跳过）-> ");
    char input[10];
    fgets(input, sizeof(input), stdin);
    sort_category.sortById = (strlen(input) > 1) ? atoi(input) : 0;

    printf("--- 输入设备类型报废年限排序方式（1 升序，-1 降序，回车跳过）-> ");
    fgets(input, sizeof(input), stdin);
    sort_category.sortByDisposalYears = (strlen(input) > 1) ? atoi(input) : 0;

    printf("--- 输入设备类型名称排序方式（1 升序，-1 降序，回车跳过）-> ");
    fgets(input, sizeof(input), stdin);
    sort_category.sortByName = (strlen(input) > 1) ? atoi(input) : 0;

    LinkedList* list = GetResourceManage()->category_list;

    LinkedList_sort(list, CompareCategory, &sort_category);

    LinkedList_print(list, PrintCategory);

    sort_category.sortByDisposalYears = 0;
    sort_category.sortById = 1;
    sort_category.sortByName = 0;
    LinkedList_sort(list, CompareCategory, &sort_category);
}

void _SortCategory(HWND hWnd)
{
    SortCategory sort_category = { 0 };

    HWND hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_ID_1);
    HWND hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_ID_2);
    LRESULT state1 = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
    LRESULT state2 = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);

    if (state1 == BST_CHECKED)
    {
        sort_category.sortById = 1;
    }
    else if (state2 == BST_CHECKED)
    {
        sort_category.sortById = -1;
    }

    hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_NAME_1);
    hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_NAME_2);
    state1 = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
    state2 = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);

    if (state1 == BST_CHECKED)
    {
        sort_category.sortByName = 1;
    }
    else if (state2 == BST_CHECKED)
    {
        sort_category.sortByName = -1;
    }

    hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_PRICE_1);
    hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_PRICE_2);
    state1 = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
    state2 = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);

    if (state1 == BST_CHECKED)
    {
        sort_category.sortByDisposalYears = 1;
    }
    else if (state2 == BST_CHECKED)
    {
        sort_category.sortByDisposalYears = -1;
    }

    LinkedList* list = GetResourceManage()->category_list;

    LinkedList_sort(list, CompareCategory, &sort_category);

    HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
    ListView_DeleteAllItems(hListView);

    Node* temp = list->head;
    while (temp->next)
    {
        temp = temp->next;
        Addcategory(hWnd, temp->data, IDC_LISTVIEW);
    }

    MessageBox(hWnd, L"排序成功", L"提示", MB_OK);
}

typedef struct _SortLabroom
{
    int sortById;
    int sortByName;
} SortLabroom;

int CompareLabroom(const void* a, const void* b, void* arg)
{
    LabRoom* labroomA = (LabRoom*)a;
    LabRoom* labroomB = (LabRoom*)b;

    SortLabroom* sort_labroom = (SortLabroom*)arg;

    if (sort_labroom->sortById != 0)
    {
        int cmp = labroomA->id - labroomB->id;
        if (cmp != 0) return sort_labroom->sortById == 1 ? cmp : -cmp;
    }

    if (sort_labroom->sortByName != 0)
    {
        int cmp = wcscmp(labroomA->name, labroomB->name);
        if (cmp != 0) return sort_labroom->sortByName == 1 ? cmp : -cmp;
    }

    return 0;
}

void SortLabroomMenu()
{
    SortLabroom sort_labroom;
    system("cls");
    printf("---        实验室排序设置        ---\n\n");

    while (getchar() != '\n');

    printf("--- 输入实验室ID排序方式（1 升序，-1 降序，回车跳过）-> ");
    char input[10];
    fgets(input, sizeof(input), stdin);
    sort_labroom.sortById = (strlen(input) > 1) ? atoi(input) : 0;

    printf("--- 输入实验室名称排序方式（1 升序，-1 降序，回车跳过）-> ");
    fgets(input, sizeof(input), stdin);
    sort_labroom.sortByName = (strlen(input) > 1) ? atoi(input) : 0;

    LinkedList* list = GetResourceManage()->laboratory_list;

    LinkedList_sort(list, CompareLabroom, &sort_labroom);

    LinkedList_print(list, PrintLabroom);
}

void _SortLabroom(HWND hWnd)
{
    SortLabroom sort_labroom = { 0 };

    HWND hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_ID_1);
    HWND hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_ID_2);
    LRESULT state1 = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
    LRESULT state2 = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);

    if (state1 == BST_CHECKED)
    {
        sort_labroom.sortById = 1;
    }
    else if (state2 == BST_CHECKED)
    {
        sort_labroom.sortById = -1;
    }

    hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_NAME_1);
    hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_NAME_2);
    state1 = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
    state2 = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);

    if (state1 == BST_CHECKED)
    {
        sort_labroom.sortByName = 1;
    }
    else if (state2 == BST_CHECKED)
    {
        sort_labroom.sortByName = -1;
    }

    LinkedList* list = GetResourceManage()->laboratory_list;

    LinkedList_sort(list, CompareLabroom, &sort_labroom);

    HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
    ListView_DeleteAllItems(hListView);

    Node* temp = list->head;
    while (temp->next)
    {
        temp = temp->next;
        AddLabroom(hWnd, temp->data, IDC_LISTVIEW);
    }

    MessageBox(hWnd, L"排序成功", L"提示", MB_OK);
}

typedef struct _SortEquiment
{
    int sortById;
    int sortByName;
    int sortByPrice;
    int sortByDate;
    int sortByCategoryId;
    int sortByLabroomId;
    
} SortEquipment;

int CompareEquiment(const void* a, const void* b, void* arg)
{
    ExperimentalEquipment* equimenA = (ExperimentalEquipment*)a;
    ExperimentalEquipment* equimenB = (ExperimentalEquipment*)b;

    SortEquipment* sort_equiment = (SortEquipment*)arg;

    if (sort_equiment->sortById != 0)
    {
        int cmp = equimenA->id - equimenB->id;
        if (cmp != 0) return sort_equiment->sortById == 1 ? cmp : -cmp;
    }

    if (sort_equiment->sortByName != 0)
    {
        int cmp = wcscmp(equimenA->name, equimenB->name);
        if (cmp != 0) return sort_equiment->sortByName == 1 ? cmp : -cmp;
    }

    if (sort_equiment->sortByPrice != 0)
    {
        int cmp = equimenA->price - equimenB->price;
        if (cmp != 0) return sort_equiment->sortByPrice == 1 ? cmp : -cmp;
    }

    if (sort_equiment->sortByDate != 0)
    {
        int cmp = wcscmp(equimenA->purchase_date, equimenB->purchase_date);
        if (cmp != 0) return sort_equiment->sortByDate == 1 ? cmp : -cmp;
    }

    if (sort_equiment->sortByCategoryId != 0)
    {
        int cmp = equimenA->category->id - equimenB->category->id;
        if (cmp != 0) return sort_equiment->sortByCategoryId == 1 ? cmp : -cmp;
    }

    if (sort_equiment->sortByLabroomId != 0)
    {
        int cmp = equimenA->room_id - equimenB->room_id;
        if (cmp != 0) return sort_equiment->sortByLabroomId == 1 ? cmp : -cmp;
    }

    return 0;
}

void SortEquipmentMenu()
{
    SortEquipment sort_equipment;
    system("cls");
    printf("---        实验设备排序设置        ---\n\n");

    while (getchar() != '\n');

    printf("--- 输入实验设备ID排序方式（1 升序，-1 降序，回车跳过）-> ");
    char input[10];
    fgets(input, sizeof(input), stdin);
    sort_equipment.sortById = (strlen(input) > 1) ? atoi(input) : 0;

    printf("--- 输入实验设备名称排序方式（1 升序，-1 降序，回车跳过）-> ");
    fgets(input, sizeof(input), stdin);
    sort_equipment.sortByName = (strlen(input) > 1) ? atoi(input) : 0;

    printf("--- 输入实验设备价格排序方式（1 升序，-1 降序，回车跳过）-> ");
    fgets(input, sizeof(input), stdin);
    sort_equipment.sortByPrice = (strlen(input) > 1) ? atoi(input) : 0;

    printf("--- 输入实验设备购买日期排序方式（1 升序，-1 降序，回车跳过）-> ");
    fgets(input, sizeof(input), stdin);
    sort_equipment.sortByDate = (strlen(input) > 1) ? atoi(input) : 0;

    printf("--- 输入实验设备类型ID排序方式（1 升序，-1 降序，回车跳过）-> ");
    fgets(input, sizeof(input), stdin);
    sort_equipment.sortByCategoryId = (strlen(input) > 1) ? atoi(input) : 0;

    printf("--- 输入实验设备所属实验室ID排序方式（1 升序，-1 降序，回车跳过）-> ");
    fgets(input, sizeof(input), stdin);
    sort_equipment.sortByLabroomId = (strlen(input) > 1) ? atoi(input) : 0;

    LinkedList* list = GetResourceManage()->equipment_list;

    LinkedList_sort(list, CompareEquiment, &sort_equipment);

    LinkedList_print(list, PrintEquipment);
}

void _SortEquipment(HWND hWnd)
{
    SortEquipment sort_equipment = { 0 };
    
    HWND hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_ID_1);
    HWND hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_ID_2);
    LRESULT state1 = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
    LRESULT state2 = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);

    if (state1 == BST_CHECKED) 
    {
        sort_equipment.sortById = 1;
    }
    else if (state2 == BST_CHECKED)
    {
        sort_equipment.sortById = -1;
    }

    hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_NAME_1);
    hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_NAME_2);
    state1 = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
    state2 = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);

    if (state1 == BST_CHECKED)
    {
        sort_equipment.sortByName = 1;
    }
    else if (state2 == BST_CHECKED)
    {
        sort_equipment.sortByName = -1;
    }

    hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_PRICE_1);
    hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_PRICE_2);
    state1 = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
    state2 = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);

    if (state1 == BST_CHECKED)
    {
        sort_equipment.sortByPrice = 1;
    }
    else if (state2 == BST_CHECKED)
    {
        sort_equipment.sortByPrice = -1;
    }

    hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_DATE_1);
    hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_DATE_2);
    state1 = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
    state2 = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);

    if (state1 == BST_CHECKED)
    {
        sort_equipment.sortByDate = 1;
    }
    else if (state2 == BST_CHECKED)
    {
        sort_equipment.sortByDate = -1;
    }

    hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_CATEGORYID_1);
    hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_CATEGORYID_2);
    state1 = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
    state2 = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);

    if (state1 == BST_CHECKED)
    {
        sort_equipment.sortByCategoryId = 1;
    }
    else if (state2 == BST_CHECKED)
    {
        sort_equipment.sortByCategoryId = -1;
    }

    hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_ROOMID_1);
    hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_ROOMID_2);
    state1 = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
    state2 = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);

    if (state1 == BST_CHECKED)
    {
        sort_equipment.sortByLabroomId = 1;
    }
    else if (state2 == BST_CHECKED)
    {
        sort_equipment.sortByLabroomId = -1;
    }

    LinkedList* list = GetResourceManage()->equipment_list;

    LinkedList_sort(list, CompareEquiment, &sort_equipment);

    HWND hListView = GetDlgItem(hWnd, IDC_LISTVIEW);
    ListView_DeleteAllItems(hListView);

    Node* temp = list->head;
    while (temp->next)
    {
        temp = temp->next;
        AddEquiment(hWnd, temp->data, IDC_LISTVIEW);
    }

    MessageBox(hWnd, L"排序成功", L"提示", MB_OK);
}

void CreateButtonsSort(HWND hWnd, int tabIndex)
{
    HWND h;
    int idc = IDC_BUTTON_SORT;
    for (int i = idc; i <= 4018; i++)
    {
        while ((h = GetDlgItem(hWnd, i)) != NULL)
            DestroyWindow(h);
    }

    CreateWindow(L"BUTTON", L"返回", WS_VISIBLE | WS_CHILD,
        600, 495, 120, 40, hWnd, (HMENU)IDC_BUTTON_EXIT, GetModuleHandle(NULL), NULL);

    ResourceManager* rm = GetResourceManage();

    switch (tabIndex)
    {
    case 0:
    {
        CreateWindow(L"BUTTON", L"排序", WS_VISIBLE | WS_CHILD,
            50, 425, 120, 40, hWnd, (HMENU)IDC_BUTTON_SORT, GetModuleHandle(NULL), NULL);

        CreateWindow(L"STATIC", L"ID排序", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 70, 140, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindowW(L"BUTTON", L"升序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 190, 70, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_ID_1, NULL, NULL);
        CreateWindowW(L"BUTTON", L"降序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 260, 70, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_ID_2, NULL, NULL);

        CreateWindow(L"STATIC", L"名字排序", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 130, 140, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindowW(L"BUTTON", L"升序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 190, 130, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_NAME_1, NULL, NULL);
        CreateWindowW(L"BUTTON", L"降序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 260, 130, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_NAME_2, NULL, NULL);

        CreateWindow(L"STATIC", L"价格排序", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 190, 140, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindowW(L"BUTTON", L"升序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 190, 190, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_PRICE_1, NULL, NULL);
        CreateWindowW(L"BUTTON", L"降序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 260, 190, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_PRICE_2, NULL, NULL);

        CreateWindow(L"STATIC", L"所属实验室ID排序", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 250, 140, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindowW(L"BUTTON", L"升序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 190, 250, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_ROOMID_1, NULL, NULL);
        CreateWindowW(L"BUTTON", L"降序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 260, 250, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_ROOMID_2, NULL, NULL);

        CreateWindow(L"STATIC", L"购买时间排序", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 310, 140, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindowW(L"BUTTON", L"升序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 190, 310, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_DATE_1, NULL, NULL);
        CreateWindowW(L"BUTTON", L"降序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 260, 310, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_DATE_2, NULL, NULL);

        CreateWindow(L"STATIC", L"设备种类ID排序", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 370, 140, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindowW(L"BUTTON", L"升序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 190, 370, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_CATEGORYID_1, NULL, NULL);
        CreateWindowW(L"BUTTON", L"降序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 260, 370, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_CATEGORYID_2, NULL, NULL);

        CreateWindow(L"STATIC", L"排序结果", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 490, 50, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);

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

        lvc.pszText = L"状态";
        lvc.cx = 80;
        lvc.iSubItem = 6;
        ListView_InsertColumn(hListView, 6, &lvc);

        break;
    }
    case 1:
    {
        CreateWindow(L"BUTTON", L"排序", WS_VISIBLE | WS_CHILD,
            50, 425, 120, 40, hWnd, (HMENU)IDC_BUTTON_SORT, GetModuleHandle(NULL), NULL);

        CreateWindow(L"STATIC", L"ID排序", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 120, 140, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindowW(L"BUTTON", L"升序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 190, 120, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_ID_1, NULL, NULL);
        CreateWindowW(L"BUTTON", L"降序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 260, 120, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_ID_2, NULL, NULL);

        CreateWindow(L"STATIC", L"名字排序", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 240, 140, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindowW(L"BUTTON", L"升序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 190, 240, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_NAME_1, NULL, NULL);
        CreateWindowW(L"BUTTON", L"降序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 260, 240, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_NAME_2, NULL, NULL);

        CreateWindow(L"STATIC", L"排序结果", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 490, 50, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);

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
        CreateWindow(L"BUTTON", L"排序", WS_VISIBLE | WS_CHILD,
            50, 425, 120, 40, hWnd, (HMENU)IDC_BUTTON_SORT, GetModuleHandle(NULL), NULL);

        CreateWindow(L"STATIC", L"ID排序", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 120, 140, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindowW(L"BUTTON", L"升序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 190, 120, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_ID_1, NULL, NULL);
        CreateWindowW(L"BUTTON", L"降序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 260, 120, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_ID_2, NULL, NULL);

        CreateWindow(L"STATIC", L"名字排序", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 240, 140, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindowW(L"BUTTON", L"升序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 190, 240, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_NAME_1, NULL, NULL);
        CreateWindowW(L"BUTTON", L"降序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 260, 240, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_NAME_2, NULL, NULL);

        CreateWindow(L"STATIC", L"类别排序", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 360, 140, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindowW(L"BUTTON", L"升序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 190, 360, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_CATEGORYID_1, NULL, NULL);
        CreateWindowW(L"BUTTON", L"降序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 260, 360, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_CATEGORYID_2, NULL, NULL);

        CreateWindow(L"STATIC", L"排序结果", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 490, 50, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);

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
        CreateWindow(L"BUTTON", L"排序", WS_VISIBLE | WS_CHILD,
            50, 425, 120, 40, hWnd, (HMENU)IDC_BUTTON_SORT, GetModuleHandle(NULL), NULL);

        CreateWindow(L"STATIC", L"ID排序", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 120, 140, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindowW(L"BUTTON", L"升序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 190, 120, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_ID_1, NULL, NULL);
        CreateWindowW(L"BUTTON", L"降序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 260, 120, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_ID_2, NULL, NULL);

        CreateWindow(L"STATIC", L"名字排序", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 240, 140, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindowW(L"BUTTON", L"升序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 190, 240, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_NAME_1, NULL, NULL);
        CreateWindowW(L"BUTTON", L"降序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 260, 240, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_NAME_2, NULL, NULL);

        CreateWindow(L"STATIC", L"报废年限排序排序", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 360, 140, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindowW(L"BUTTON", L"升序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 190, 360, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_PRICE_1, NULL, NULL);
        CreateWindowW(L"BUTTON", L"降序", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 260, 360, 60, 25, hWnd, (HMENU)IDC_CHECK_BOX_PRICE_2, NULL, NULL);

        CreateWindow(L"STATIC", L"排序结果", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 490, 50, 100, 25, hWnd, (HMENU)IDC_STATIC, NULL, NULL);

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

void CreateTabControlSort(HWND hWnd)
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

    CreateButtonsSort(hWnd, 0);
}

LRESULT CALLBACK SortWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        CreateTabControlSort(hWnd);
        break;

    case WM_NOTIFY:
        if (((LPNMHDR)lParam)->idFrom == IDC_TAB && ((LPNMHDR)lParam)->code == TCN_SELCHANGE)
        {
            int tabIndex = TabCtrl_GetCurSel(hMainTab);
            CreateButtonsSort(hWnd, tabIndex);
        }
        break;

    case WM_COMMAND:
    {
        int tabIndex = TabCtrl_GetCurSel(hMainTab);
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON_SORT:
            switch (tabIndex)
            {
            case 0:
                _SortEquipment(hWnd);
                break;
            case 1:
                _SortLabroom(hWnd);
                break;
            case 2:
                _SortAccount(hWnd);
                break;
            case 3:
                _SortCategory(hWnd);
                break;
            default:
                break;
            }
            break;
        case IDC_CHECK_BOX_CATEGORYID_1:
        {
            HWND hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_CATEGORYID_1);
            HWND hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_CATEGORYID_2);
            LRESULT state = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
            if (state == BST_CHECKED)
            {
                SendMessage(hCheckBox2, BM_SETCHECK, BST_UNCHECKED, 0);
            }
            break;
        }
        case IDC_CHECK_BOX_CATEGORYID_2:
        {
            HWND hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_CATEGORYID_1);
            HWND hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_CATEGORYID_2);
            LRESULT state = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);
            if (state == BST_CHECKED)
            {
                SendMessage(hCheckBox1, BM_SETCHECK, BST_UNCHECKED, 0);
            }
            break;
        }
        case IDC_CHECK_BOX_NAME_1:
        {
            HWND hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_NAME_1);
            HWND hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_NAME_2);
            LRESULT state = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
            if (state == BST_CHECKED)
            {
                SendMessage(hCheckBox2, BM_SETCHECK, BST_UNCHECKED, 0);
            }
            break;
        }
        case IDC_CHECK_BOX_NAME_2:
        {
            HWND hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_NAME_1);
            HWND hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_NAME_2);
            LRESULT state = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);
            if (state == BST_CHECKED)
            {
                SendMessage(hCheckBox1, BM_SETCHECK, BST_UNCHECKED, 0);
            }
            break;
        }
        case IDC_CHECK_BOX_ROOMID_1:
        {
            HWND hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_ROOMID_1);
            HWND hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_ROOMID_2);
            LRESULT state = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
            if (state == BST_CHECKED)
            {
                SendMessage(hCheckBox2, BM_SETCHECK, BST_UNCHECKED, 0);
            }
            break;
        }
        case IDC_CHECK_BOX_ROOMID_2:
        {
            HWND hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_ROOMID_1);
            HWND hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_ROOMID_2);
            LRESULT state = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);
            if (state == BST_CHECKED)
            {
                SendMessage(hCheckBox1, BM_SETCHECK, BST_UNCHECKED, 0);
            }
            break;
        }
        case IDC_CHECK_BOX_PRICE_1:
        {
            HWND hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_PRICE_1);
            HWND hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_PRICE_2);
            LRESULT state = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
            if (state == BST_CHECKED)
            {
                SendMessage(hCheckBox2, BM_SETCHECK, BST_UNCHECKED, 0);
            }
            break;
        }
        case IDC_CHECK_BOX_PRICE_2:
        {
            HWND hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_PRICE_1);
            HWND hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_PRICE_2);
            LRESULT state = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);
            if (state == BST_CHECKED)
            {
                SendMessage(hCheckBox1, BM_SETCHECK, BST_UNCHECKED, 0);
            }
            break;
        }
        case IDC_CHECK_BOX_ID_1:
        {
            HWND hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_ID_1);
            HWND hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_ID_2);
            LRESULT state = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
            if (state == BST_CHECKED)
            {
                SendMessage(hCheckBox2, BM_SETCHECK, BST_UNCHECKED, 0);
            }
            break;
        }
        case IDC_CHECK_BOX_ID_2:
        {
            HWND hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_ID_1);
            HWND hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_ID_2);
            LRESULT state = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);
            if (state == BST_CHECKED)
            {
                SendMessage(hCheckBox1, BM_SETCHECK, BST_UNCHECKED, 0);
            }
            break;
        }
        case IDC_CHECK_BOX_DATE_1:
        {
            HWND hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_DATE_1);
            HWND hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_DATE_2);
            LRESULT state = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
            if (state == BST_CHECKED)
            {
                SendMessage(hCheckBox2, BM_SETCHECK, BST_UNCHECKED, 0);
            }
            break;
        }
        case IDC_CHECK_BOX_DATE_2:
        {
            HWND hCheckBox1 = GetDlgItem(hWnd, IDC_CHECK_BOX_DATE_1);
            HWND hCheckBox2 = GetDlgItem(hWnd, IDC_CHECK_BOX_DATE_2);
            LRESULT state = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);
            if (state == BST_CHECKED)
            {
                SendMessage(hCheckBox1, BM_SETCHECK, BST_UNCHECKED, 0);
            }
            break;
        }
        case IDC_BUTTON_EXIT:
            ShowWindow(hwndfuncSort, SW_HIDE);
            hwndfuncSort = NULL;
            ShowWindow(hwndAdminManagement, SW_SHOW);
            break;
        }
    }
    case WM_DESTROY:
        hwndfuncSort = NULL;
        ShowWindow(hwndAdminManagement, SW_SHOW);
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void ShowSortWindow(HWND hWnd)
{
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = SortWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"SortWindow";

    RegisterClass(&wc);

    hwndfuncSort = CreateWindow(L"SortWindow", L"信息排序",
        WS_OVERLAPPED | WS_SYSMENU, 200, 100, 800, 600, hWnd, NULL, GetModuleHandle(NULL), NULL);

    ShowWindow(hwndfuncSort, SW_SHOW);
    UpdateWindow(hwndfuncSort);
}