#pragma once
#include "account.h"
#include "category.h"
#include "lab_room.h"
#include "experimental_equipment.h"
#include "sort.h"
#include "string.h"

#define ID_BTN_STATISTICS 1010
#define IDC_RADIO_STATS_EQUIPMENT 1011
#define IDC_RADIO_STATS_ACCOUNT 1012
#define IDC_CATEGORY_COMBO 1013
#define IDC_LABROOM_COMBO 1014
#define IDC_CHECK_BY_CATEGORY 1015
#define IDC_CHECK_BY_LABROOM 1016
#define IDC_CHECK_BY_PRICE 1017
#define IDC_CHECK_BY_DATE 1018
#define IDC_CHECK_BY_ACCOUNT_TYPE 1019
#define IDC_CHECK_BY_ACCOUNT_LABROOM 1020
#define IDC_EDIT_MIN_PRICE 1021
#define IDC_EDIT_MAX_PRICE 1022
#define IDC_EDIT_START_DATE 1023
#define IDC_EDIT_END_DATE 1024
#define IDC_ACCOUNT_TYPE_COMBO 1025
#define IDC_ACCOUNT_LABROOM_COMBO 1026
#define IDC_BTN_STATS_EXECUTE 1027
#define IDC_BTN_STATS_BACK 1028
#define IDC_STATS_RESULT_LIST 1029

typedef struct _EquipmentsCount
{
    int categoryId;
    int countByCategory; 
    int room_id;
    int countByRoom;     
    int countByDate;
    int countByPrice;
    int min_price;
    int max_price;
    wchar_t startDate[DATE_LENGTH];
    wchar_t endDate[DATE_LENGTH];
    int count;
} EquipmentsCount;


void CountEquipment(LinkedList* list, EquipmentsCount* Count) 
{
    if (!list || !Count) return;

    Node* node = list->head->next;
    while (node) 
    {
        ExperimentalEquipment* ee = (ExperimentalEquipment*)node->data;

        bool is_category_conform = Count->countByCategory < 0 || 
            (Count->countByCategory >= 0 && Count->categoryId == ee->category->id);
        bool is_roomid_conform = Count->countByRoom < 0 || 
            (Count->countByRoom >= 0 && Count->room_id == ee->room_id);
        bool is_date_conform = Count->countByDate < 0 || 
            (strcmp(ee->purchase_date, Count->startDate) >= 0 && strcmp(ee->purchase_date, Count->endDate) <= 0);
        bool is_price_conform = Count->countByPrice < 0 ||
            (ee->price >= Count->min_price && ee->price <= Count->max_price);

        if (is_category_conform && is_roomid_conform && is_date_conform && is_price_conform)
        {
            Count->count++;
        }
        node = node->next;
    }
}

static void PrintEquipment2(EquipmentsCount* Count)
{
    ResourceManager* rm = GetResourceManage();

    printf("--- 设备统计结果 ---\n");
    if (Count->countByRoom >= 0)
    {
        LabRoom* labroom = RoomId_to_LabRoom(Count->room_id);
        printf("--- 实验室%s 中:\n", labroom->name);
    }

    if (Count->countByDate >= 0)
    {
        printf("在日期%s到%s之间\n", Count->startDate, Count->endDate);
    }

    if (Count->countByPrice >= 0)
    {
        printf("在价格%d元到%d元之间\n", Count->min_price, Count->max_price);
    }

    if (Count->countByCategory >= 0)
    {
        Category* category = Id_to_Category(Count->categoryId);

        printf("类型为%s\n", category->name);
    }

    printf("共有%d件\n",Count->count);
    system("pause");
        
}

void StatisticsEquipmentMenu()
{
    EquipmentsCount Count;
    Count.countByCategory = -1;
    Count.countByDate = -1;
    Count.countByPrice = -1;
    Count.countByRoom = -1;
    Count.count = 0;

    ResourceManager* rm = GetResourceManage();

    system("cls");

    printf("---        设备统计设置        ---\n\n");

    while (getchar() != '\n');

    // 按设备类型统计
    printf("--- 是否按照设备类型统计（1 确定，回车跳过）-> ");
    char input[10];
    fgets(input, sizeof(input), stdin);
    if (input[0] == '1') 
    {
        printf("--- 当前统计类型有\n");
        size_t category_count = rm->category_list->size;

        printf("---      选择你要统计的类型        ---\n\n");

        Node* temp = rm->category_list->head->next;
        for (size_t i = 0; i < category_count; i++)
        {
            Category* category = (Category*)temp->data;
            printf("--- No%d. %s\n", (int)i + 1, category->name);
            temp = temp->next;
        }

        printf("--- 输入想要统计的设备类型编号: ");
        fgets(input, sizeof(input), stdin);
        int categoryIndex = atoi(input);
        if (categoryIndex > 0 && categoryIndex <= (int)category_count) 
        {
            Count.countByCategory = 1;
            Count.categoryId = categoryIndex;
        }
    }

    // 按实验室统计
    printf("--- 是否按照设备所属实验室统计（1 确定，回车跳过）-> ");
    fgets(input, sizeof(input), stdin);
    if (input[0] == '1') 
    {
        printf("--- 请输入实验室ID: ");
        fgets(input, sizeof(input), stdin);
        Count.room_id = atoi(input);

        LabRoom* labroom = RoomId_to_LabRoom(Count.room_id);
        if (labroom == NULL)
        {
            printf("该实验室不存在\n");
            system("pause");
            return;
        }
        Count.countByRoom = 1;
    }

    // 按价格统计
    printf("--- 是否按照设备价格统计（1 确定，回车跳过）-> ");
    fgets(input, sizeof(input), stdin);
    if (input[0] == '1') 
    {
        printf("--- 请输入最低价格: ");
        fgets(input, sizeof(input), stdin);
        Count.min_price = atof(input);

        printf("--- 请输入最高价格: ");
        fgets(input, sizeof(input), stdin);
        Count.max_price = atof(input);

        Count.countByPrice = 1;
    }

    printf("--- 是否按照设备日期统计（1 确定，回车跳过）-> ");
    fgets(input, sizeof(input), stdin);
    if (input[0] == '1') 
    {
        printf("--- 请输入最早时间（YYYYMMDD）: ");
        fgets(Count.startDate, sizeof(Count.startDate), stdin);
        Count.startDate[strcspn(Count.startDate, "\n")] = '\0';  

        printf("--- 请输入最晚时间（YYYYMMDD）: ");
        fgets(Count.endDate, sizeof(Count.endDate), stdin);
        Count.endDate[strcspn(Count.endDate, "\n")] = '\0';

        Count.countByDate = 1;
    }

    CountEquipment(rm->equipment_list, &Count);

    PrintEquipment2(&Count);
}

typedef struct _AccountCount
{
    AccountType type;
    int countByType;
    int room_id;
    int countByRoom;
    int count;
} AccountCount;

void CountAccount(LinkedList* list, AccountCount* Count)
{
    if (!list || !Count) return;

    Node* node = list->head->next;
    while (node)
    {
        Account* account = (Account*)node->data;

        bool is_category_conform = Count->countByType < 0 ||
            (Count->countByType >= 0 && Count->type == account->account_type);
        bool is_roomid_conform = Count->countByRoom < 0 ||
            (Count->countByRoom >= 0 && Count->room_id == account->roomid);

        if (is_category_conform && is_roomid_conform)
        {
            Count->count++;
        }
        node = node->next;
    }
}

static void PrintAccount2(AccountCount* Count)
{
    ResourceManager* rm = GetResourceManage();

    printf("--- 设备统计结果 ---\n");
    if (Count->countByRoom >= 0)
    {
        LabRoom* labroom = RoomId_to_LabRoom(Count->room_id);
        printf("--- 实验室%s 中:\n", labroom->name);
    }

    if (Count->countByType >= 0)
    {
        char type[10] = "";
        switch (Count->type)
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
        }
        printf("--- 账户类型为%s :\n", type);
    }

    printf("共有%d个\n", Count->count);
    system("pause");

}

void StatisticsAccountMenu()
{
    AccountCount Count;
    Count.countByType = -1;
    Count.type = Unknow;
    Count.countByRoom = -1;
    Count.count = 0;

    ResourceManager* rm = GetResourceManage();

    system("cls");

    printf("---        设备统计设置        ---\n\n");

    while (getchar() != '\n');

    // 按账户类型统计
    printf("--- 是否按照账户类型统计（1 确定，回车跳过）-> ");
    char input[10];
    fgets(input, sizeof(input), stdin);
    if (input[0] == '1')
    {
        printf("--- 账户类型有\n");

        printf("---      选择你要统计的类型        ---\n\n");

        printf("--- No1. 管理员\n");
        printf("--- No2. 实验员\n");
        printf("--- No3. 一般用户\n");

        printf("--- 输入想要统计的账户类型: ");
        fgets(input, sizeof(input), stdin);
        int accountIndex = atoi(input);
        if (accountIndex > 0 && accountIndex <= 3)
        {
            Count.countByType = 1;
            Count.type = (AccountType)(accountIndex - 1);
        }

        if (Count.type == Experimenter)
        {
            printf("--- 是否按照实验员实验室统计（1 确定，回车跳过）-> ");
            fgets(input, sizeof(input), stdin);
            if (input[0] == '1')
            {
                printf("--- 请输入实验室ID: ");
                fgets(input, sizeof(input), stdin);
                Count.room_id = atoi(input);

                LabRoom* labroom = RoomId_to_LabRoom(Count.room_id);
                if (labroom == NULL)
                {
                    printf("该实验室不存在\n");
                    system("pause");
                    return;
                }
                Count.countByRoom = 1;
            }
        }
    }

    CountAccount(rm->equipment_list, &Count);

    PrintAccount2(&Count);
}

extern HWND hwndSystemMaintenance;
extern EquipmentManagement* em;

HWND hwndStatistics = NULL;
HWND hwndEquipmentGroup = NULL;
HWND hwndAccountGroup = NULL;


// 填充设备类别下拉框
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

// 填充实验室下拉框
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

// 执行设备统计
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

	// 按设备类型统计
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

	// 按实验室统计
	if (SendMessage(hCheckByLabroom, BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		count.countByRoom = 1;
		int selectedIndex = SendMessage(hLabroomCombo, CB_GETCURSEL, 0, 0);
		if (selectedIndex != CB_ERR)
		{
			wchar_t buffer[100];
			SendMessage(hLabroomCombo, CB_GETLBTEXT, selectedIndex, (LPARAM)buffer);
			// 解析出实验室ID
			count.room_id = _wtoi(buffer);
		}
	}

	// 按价格统计
	if (SendMessage(hCheckByPrice, BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		count.countByPrice = 1;
		wchar_t minPrice[32], maxPrice[32];
		GetWindowText(hEditMinPrice, minPrice, 32);
		GetWindowText(hEditMaxPrice, maxPrice, 32);
		count.min_price = _wtoi(minPrice);
		count.max_price = _wtoi(maxPrice);
	}

	// 按日期统计
	if (SendMessage(hCheckByDate, BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		count.countByDate = 1;
		wchar_t startDate[DATE_LENGTH], endDate[DATE_LENGTH];
		GetWindowText(hEditStartDate, startDate, DATE_LENGTH);
		GetWindowText(hEditEndDate, endDate, DATE_LENGTH);

		// 转换日期字符串
		char mbStartDate[DATE_LENGTH], mbEndDate[DATE_LENGTH];
		wcstombs_s(NULL, mbStartDate, DATE_LENGTH, startDate, _TRUNCATE);
		wcstombs_s(NULL, mbEndDate, DATE_LENGTH, endDate, _TRUNCATE);

		strcpy_s(count.startDate, DATE_LENGTH, mbStartDate);
		strcpy_s(count.endDate, DATE_LENGTH, mbEndDate);
	}

	// 执行统计
	CountEquipment(rm->equipment_list, &count);

	// 显示统计结果
	int itemIndex = 0;

	// 添加统计条件
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
			lvi.pszText = L"设备类型";
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
			lvi.pszText = L"所属实验室";
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
		lvi.pszText = L"价格范围";
		ListView_InsertItem(hListView, &lvi);

		swprintf(buffer, 256, L"%d 元 - %d 元", count.min_price, count.max_price);
		ListView_SetItemText(hListView, itemIndex, 1, buffer);
		itemIndex++;
	}

	if (count.countByDate >= 0)
	{
		lvi.mask = LVIF_TEXT;
		lvi.iItem = itemIndex;
		lvi.iSubItem = 0;
		lvi.pszText = L"日期范围";
		ListView_InsertItem(hListView, &lvi);

		wchar_t startDate[DATE_LENGTH], endDate[DATE_LENGTH];
		mbstowcs_s(NULL, startDate, DATE_LENGTH, count.startDate, _TRUNCATE);
		mbstowcs_s(NULL, endDate, DATE_LENGTH, count.endDate, _TRUNCATE);

		swprintf(buffer, 256, L"%s - %s", startDate, endDate);
		ListView_SetItemText(hListView, itemIndex, 1, buffer);
		itemIndex++;
	}

	// 添加统计结果
	lvi.mask = LVIF_TEXT;
	lvi.iItem = itemIndex;
	lvi.iSubItem = 0;
	lvi.pszText = L"统计结果";
	ListView_InsertItem(hListView, &lvi);

	swprintf(buffer, 256, L"共有 %d 件设备", count.count);
	ListView_SetItemText(hListView, itemIndex, 1, buffer);
}

// 执行账户统计
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

	// 按账户类型统计
	if (SendMessage(hCheckByAccountType, BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		count.countByType = 1;
		int selectedIndex = SendMessage(hAccountTypeCombo, CB_GETCURSEL, 0, 0);
		if (selectedIndex != CB_ERR)
		{
			// 根据下拉框选择设置账户类型
			switch (selectedIndex)
			{
			case 0: count.type = Admin; break;
			case 1: count.type = Experimenter; break;
			case 2: count.type = User; break;
			default: count.type = Unknow; break;
			}
		}
	}

	// 按实验室统计（仅针对实验员）
	if (count.type == Experimenter &&
		SendMessage(hCheckByAccountLabroom, BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		count.countByRoom = 1;
		int selectedIndex = SendMessage(hAccountLabroomCombo, CB_GETCURSEL, 0, 0);
		if (selectedIndex != CB_ERR)
		{
			wchar_t buffer[100];
			SendMessage(hAccountLabroomCombo, CB_GETLBTEXT, selectedIndex, (LPARAM)buffer);
			// 解析出实验室ID
			count.room_id = _wtoi(buffer);
		}
	}

	// 执行统计
	CountAccount(rm->account_list, &count);

	// 显示统计结果
	int itemIndex = 0;

	// 添加统计条件
	LVITEM lvi;
	wchar_t buffer[256];

	if (count.countByType >= 0)
	{
		lvi.mask = LVIF_TEXT;
		lvi.iItem = itemIndex;
		lvi.iSubItem = 0;
		lvi.pszText = L"账户类型";
		ListView_InsertItem(hListView, &lvi);

		const wchar_t* typeStr = L"";
		switch (count.type)
		{
		case Admin: typeStr = L"管理员"; break;
		case Experimenter: typeStr = L"实验员"; break;
		case User: typeStr = L"一般用户"; break;
		default: typeStr = L"未知"; break;
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
			lvi.pszText = L"所属实验室";
			ListView_InsertItem(hListView, &lvi);

			ListView_SetItemText(hListView, itemIndex, 1, labroom->name);
			itemIndex++;
		}
	}

	// 添加统计结果
	lvi.mask = LVIF_TEXT;
	lvi.iItem = itemIndex;
	lvi.iSubItem = 0;
	lvi.pszText = L"统计结果";
	ListView_InsertItem(hListView, &lvi);

	swprintf(buffer, 256, L"共有 %d 个账户", count.count);
	ListView_SetItemText(hListView, itemIndex, 1, buffer);
}

// 统计功能窗口过程
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
		// 创建标题
		CreateWindow(L"STATIC", L"统计功能", WS_VISIBLE | WS_CHILD | SS_CENTER,
			300, 10, 200, 30, hWnd, NULL, NULL, NULL);

		// 创建分类选择单选按钮
		hRadioEquipment = CreateWindow(L"BUTTON", L"设备统计", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP,
			50, 50, 100, 30, hWnd, (HMENU)IDC_RADIO_STATS_EQUIPMENT, NULL, NULL);
		hRadioAccount = CreateWindow(L"BUTTON", L"账户统计", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
			200, 50, 100, 30, hWnd, (HMENU)IDC_RADIO_STATS_ACCOUNT, NULL, NULL);

		// 默认选择设备统计
		SendMessage(hRadioEquipment, BM_SETCHECK, BST_CHECKED, 0);

		// 创建设备统计分组框
		hwndEquipmentGroup = CreateWindow(L"BUTTON", L"设备统计选项", WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
			50, 90, 700, 200, hWnd, NULL, NULL, NULL);

		// 创建按设备类型统计选项
		hCheckByCategory = CreateWindow(L"BUTTON", L"按设备类型统计", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			70, 120, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_CATEGORY, NULL, NULL);

		CreateWindow(L"STATIC", L"设备类型:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
			220, 120, 80, 25, hWnd, NULL, NULL, NULL);

		hCategoryCombo = CreateWindow(L"COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
			310, 120, 120, 200, hWnd, (HMENU)IDC_CATEGORY_COMBO, NULL, NULL);

		// 填充设备类型下拉框
		FillCategoryComboBox(hCategoryCombo);

		// 创建按实验室统计选项
		hCheckByLabroom = CreateWindow(L"BUTTON", L"按实验室统计", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			70, 150, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_LABROOM, NULL, NULL);

		CreateWindow(L"STATIC", L"实验室:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
			220, 150, 80, 25, hWnd, NULL, NULL, NULL);

		hLabroomCombo = CreateWindow(L"COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
			310, 150, 120, 200, hWnd, (HMENU)IDC_LABROOM_COMBO, NULL, NULL);

		// 填充实验室下拉框
		FillLabroomComboBox(hLabroomCombo);

		// 创建按价格统计选项
		hCheckByPrice = CreateWindow(L"BUTTON", L"按价格统计", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			70, 180, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_PRICE, NULL, NULL);

		CreateWindow(L"STATIC", L"最低价格:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
			220, 180, 80, 25, hWnd, NULL, NULL, NULL);

		hEditMinPrice = CreateWindow(L"EDIT", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
			310, 180, 80, 25, hWnd, (HMENU)IDC_EDIT_MIN_PRICE, NULL, NULL);

		CreateWindow(L"STATIC", L"最高价格:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
			400, 180, 80, 25, hWnd, NULL, NULL, NULL);

		hEditMaxPrice = CreateWindow(L"EDIT", L"10000", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
			490, 180, 80, 25, hWnd, (HMENU)IDC_EDIT_MAX_PRICE, NULL, NULL);

		// 创建按日期统计选项
		hCheckByDate = CreateWindow(L"BUTTON", L"按日期统计", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			70, 210, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_DATE, NULL, NULL);

		CreateWindow(L"STATIC", L"开始日期:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
			220, 210, 80, 25, hWnd, NULL, NULL, NULL);

		hEditStartDate = CreateWindow(L"EDIT", L"20200101", WS_VISIBLE | WS_CHILD | WS_BORDER,
			310, 210, 80, 25, hWnd, (HMENU)IDC_EDIT_START_DATE, NULL, NULL);

		CreateWindow(L"STATIC", L"结束日期:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
			400, 210, 80, 25, hWnd, NULL, NULL, NULL);

		hEditEndDate = CreateWindow(L"EDIT", L"20240101", WS_VISIBLE | WS_CHILD | WS_BORDER,
			490, 210, 80, 25, hWnd, (HMENU)IDC_EDIT_END_DATE, NULL, NULL);

		// 创建账户统计分组框（初始隐藏）
		hwndAccountGroup = CreateWindow(L"BUTTON", L"账户统计选项", WS_CHILD | BS_GROUPBOX,
			50, 90, 700, 200, hWnd, NULL, NULL, NULL);

		// 创建按账户类型统计选项
		hCheckByAccountType = CreateWindow(L"BUTTON", L"按账户类型统计", WS_CHILD | BS_AUTOCHECKBOX,
			70, 120, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_ACCOUNT_TYPE, NULL, NULL);

		CreateWindow(L"STATIC", L"账户类型:", WS_CHILD | SS_RIGHT,
			220, 120, 80, 25, hWnd, NULL, NULL, NULL);

		hAccountTypeCombo = CreateWindow(L"COMBOBOX", NULL, WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
			310, 120, 150, 200, hWnd, (HMENU)IDC_ACCOUNT_TYPE_COMBO, NULL, NULL);

		// 填充账户类型下拉框
		SendMessage(hAccountTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"管理员");
		SendMessage(hAccountTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"实验员");
		SendMessage(hAccountTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"一般用户");
		SendMessage(hAccountTypeCombo, CB_SETCURSEL, 0, 0);

		// 创建按实验室统计选项（仅针对实验员）
		hCheckByAccountLabroom = CreateWindow(L"BUTTON", L"按实验室统计（仅实验员）", WS_CHILD | BS_AUTOCHECKBOX,
			70, 150, 200, 25, hWnd, (HMENU)IDC_CHECK_BY_ACCOUNT_LABROOM, NULL, NULL);

		CreateWindow(L"STATIC", L"实验室:", WS_CHILD | SS_RIGHT,
			280, 150, 60, 25, hWnd, NULL, NULL, NULL);

		hAccountLabroomCombo = CreateWindow(L"COMBOBOX", NULL, WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
			350, 150, 120, 200, hWnd, (HMENU)IDC_ACCOUNT_LABROOM_COMBO, NULL, NULL);

		// 填充实验室下拉框（与设备统计共用相同数据）
		FillLabroomComboBox(hAccountLabroomCombo);

		// 创建统计结果列表视图
		hResultListView = CreateWindowEx(0, WC_LISTVIEW, NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT,
			50, 300, 700, 200, hWnd, (HMENU)IDC_STATS_RESULT_LIST, NULL, NULL);

		// 添加列
		LVCOLUMN lvc;
		lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

		lvc.pszText = L"统计项目";
		lvc.cx = 200;
		lvc.iSubItem = 0;
		ListView_InsertColumn(hResultListView, 0, &lvc);

		lvc.pszText = L"统计结果";
		lvc.cx = 470;
		lvc.iSubItem = 1;
		ListView_InsertColumn(hResultListView, 1, &lvc);

		// 创建执行统计按钮
		CreateWindow(L"BUTTON", L"执行统计", WS_VISIBLE | WS_CHILD,
			250, 520, 100, 30, hWnd, (HMENU)IDC_BTN_STATS_EXECUTE, NULL, NULL);

		// 创建返回按钮
		CreateWindow(L"BUTTON", L"返回", WS_VISIBLE | WS_CHILD,
			450, 520, 100, 30, hWnd, (HMENU)IDC_BTN_STATS_BACK, NULL, NULL);

		break;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RADIO_STATS_EQUIPMENT:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				// 显示设备统计选项，隐藏账户统计选项
				ShowWindow(hwndEquipmentGroup, SW_SHOW);
				ShowWindow(hwndAccountGroup, SW_HIDE);

				// 显示设备统计的所有子控件
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

				// 隐藏账户统计的所有子控件
				ShowWindow(hCheckByAccountType, SW_HIDE);
				ShowWindow(hAccountTypeCombo, SW_HIDE);
				ShowWindow(hCheckByAccountLabroom, SW_HIDE);
				ShowWindow(hAccountLabroomCombo, SW_HIDE);
			}
			break;

		case IDC_RADIO_STATS_ACCOUNT:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				// 显示账户统计选项，隐藏设备统计选项
				ShowWindow(hwndEquipmentGroup, SW_HIDE);
				ShowWindow(hwndAccountGroup, SW_SHOW);

				// 隐藏设备统计的所有子控件
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

				// 显示账户统计的所有子控件
				ShowWindow(hCheckByAccountType, SW_SHOW);
				ShowWindow(hAccountTypeCombo, SW_SHOW);
				ShowWindow(hCheckByAccountLabroom, SW_SHOW);
				ShowWindow(hAccountLabroomCombo, SW_SHOW);
			}
			break;

		case IDC_BTN_STATS_EXECUTE:
			// 清空结果列表
			ListView_DeleteAllItems(hResultListView);

			// 判断是设备统计还是账户统计
			if (SendMessage(hRadioEquipment, BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				// 设备统计
				ExecuteEquipmentStatistics(hWnd, hResultListView, hCheckByCategory, hCategoryCombo,
					hCheckByLabroom, hLabroomCombo, hCheckByPrice,
					hEditMinPrice, hEditMaxPrice, hCheckByDate,
					hEditStartDate, hEditEndDate);
			}
			else
			{
				// 账户统计
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


// 显示统计功能窗口
void ShowStatisticsWindow(HWND hWnd)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = StatisticsWndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"StatisticsWindow";

	RegisterClass(&wc);

	hwndStatistics = CreateWindow(L"StatisticsWindow", L"统计功能",
		WS_OVERLAPPED | WS_SYSMENU, 100, 50, 800, 600, hWnd, NULL, GetModuleHandle(NULL), NULL);

	ShowWindow(hwndStatistics, SW_SHOW);
	UpdateWindow(hwndStatistics);
}

