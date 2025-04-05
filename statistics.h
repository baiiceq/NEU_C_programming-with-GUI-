#pragma once
#include "account.h"
#include "category.h"
#include "lab_room.h"
#include "experimental_equipment.h"
#include "sort.h"
#include "string.h"
#include <math.h>


    // ͳ�ƹ���ID����
#define ID_BTN_STATISTICS 1010

// ͳ��������ؼ�ID
#define IDC_BTN_PRESET_STATS 1110
#define IDC_BTN_EQUIPMENT_STATS 1111
#define IDC_BTN_ACCOUNT_STATS 1112
#define IDC_BTN_STATS_BACK 1113

// �豸ͳ�ƿؼ�ID
#define IDC_CHECK_BY_CATEGORY 1115
#define IDC_CHECK_BY_LABROOM 1116
#define IDC_CHECK_BY_PRICE 1117
#define IDC_CHECK_BY_DATE 1118
#define IDC_CATEGORY_COMBO 1119
#define IDC_LABROOM_COMBO 1120
#define IDC_EDIT_MIN_PRICE 1121
#define IDC_EDIT_MAX_PRICE 1122
#define IDC_EDIT_START_DATE 1123
#define IDC_EDIT_END_DATE 1124
#define IDC_BTN_EQUIPMENT_STATS_EXECUTE 1125
#define IDC_BTN_EQUIPMENT_STATS_BACK 1126
#define IDC_EQUIPMENT_STATS_RESULT_LIST 1127
#define IDC_CHECK_BY_STATUS 1128
#define IDC_STATUS_COMBO 1129 

// �˻�ͳ�ƿؼ�ID
#define IDC_CHECK_BY_ACCOUNT_TYPE 1130
#define IDC_CHECK_BY_ACCOUNT_LABROOM 1131
#define IDC_ACCOUNT_TYPE_COMBO 1132
#define IDC_ACCOUNT_LABROOM_COMBO 1133
#define IDC_BTN_ACCOUNT_STATS_EXECUTE 1134
#define IDC_BTN_ACCOUNT_STATS_BACK 1135
#define IDC_ACCOUNT_STATS_RESULT_LIST 1136

// ����ͼ��صĿؼ�ID
#define IDC_PIE_CHART_AREA 1501
#define IDC_BTN_SWITCH_CHART 1502
#define MAX_PIE_ITEMS 30  // ����ͼ�������

// ȫ�ִ��ھ��
static HWND hwndStatisticsMain = NULL;          // ͳ��������
static HWND hwndPresetStats = NULL;             // Ԥ��ͳ�ƴ���
static HWND hwndEquipmentStats = NULL;          // �豸ͳ�ƴ���
static HWND hwndAccountStats = NULL;            // �˻�ͳ�ƴ���

// �豸ͳ�ƴ��ڿؼ����
static HWND hCheckByCategory = NULL;
static HWND hCategoryCombo = NULL;
static HWND hCheckByLabroom = NULL;
static HWND hLabroomCombo = NULL;
static HWND hCheckByPrice = NULL;
static HWND hEditMinPrice = NULL;
static HWND hEditMaxPrice = NULL;
static HWND hCheckByDate = NULL;
static HWND hEditStartDate = NULL;
static HWND hEditEndDate = NULL;
static HWND hEquipmentResultList = NULL;
static HWND hCheckByStatus = NULL; 
static HWND hStatusCombo = NULL; // �豸״̬������


// �˻�ͳ�ƴ��ڿؼ����
static HWND hCheckByAccountType = NULL;
static HWND hAccountTypeCombo = NULL;
static HWND hCheckByAccountLabroom = NULL;
static HWND hAccountLabroomCombo = NULL;
static HWND hAccountResultList = NULL;

// �ⲿ���õ�ȫ�ֱ���
extern HWND hwndSystemMaintenance;
extern EquipmentManagement* em;

typedef struct _EquipmentsCount
{
    int categoryId;
    int countByCategory;
    int room_id;
    int countByRoom;
    int countByDate;
    int countByPrice;
    int countByState;
    int min_price;
    int max_price;
	EquipmentState state;
    wchar_t startDate[DATE_LENGTH];
    wchar_t endDate[DATE_LENGTH];
    int count;
} EquipmentsCount;

typedef struct _AccountCount
{
    AccountType type;
    int countByType;
    int room_id;
    int countByRoom;
    int count;
} AccountCount;

// ����豸���������
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

// ���ʵ����������
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

// �豸ͳ�ƹ���
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
		bool is_state_conform = Count->countByState < 0 ||
			(Count->countByState >= 0 && Count->state == ee->state);

        if (is_category_conform && is_roomid_conform && is_date_conform && is_price_conform&&is_state_conform)
        {
            Count->count++;
        }
        node = node->next;
    }
}

// ִ���豸ͳ��
void ExecuteEquipmentStatistics(HWND hWnd, HWND hListView)
{
    EquipmentsCount count;
    memset(&count, 0, sizeof(EquipmentsCount));

    count.countByCategory = -1;
    count.countByRoom = -1;
    count.countByPrice = -1;
    count.countByDate = -1;
	count.countByState = -1;
    count.count = 0;

    ResourceManager* rm = GetResourceManage();
    if (!rm) return;

    // ���豸����ͳ��
    if (SendMessage(hCheckByCategory, BM_GETCHECK, 0, 0) == BST_CHECKED)
    {
        count.countByCategory = 1;

        //��ȡ�豸���id
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

    // ��ʵ����ͳ��
    if (SendMessage(hCheckByLabroom, BM_GETCHECK, 0, 0) == BST_CHECKED)
    {
        count.countByRoom = 1;


        int selectedIndex = SendMessage(hLabroomCombo, CB_GETCURSEL, 0, 0);
        if (selectedIndex != CB_ERR)
        {
            wchar_t buffer[100];
            SendMessage(hLabroomCombo, CB_GETLBTEXT, selectedIndex, (LPARAM)buffer);
            count.room_id = _wtoi(buffer);
        }
    }

    // ���۸�ͳ��
    if (SendMessage(hCheckByPrice, BM_GETCHECK, 0, 0) == BST_CHECKED)
    {
        count.countByPrice = 1;
        wchar_t minPrice[32], maxPrice[32];
        GetWindowText(hEditMinPrice, minPrice, 32);
        GetWindowText(hEditMaxPrice, maxPrice, 32);
        count.min_price = _wtoi(minPrice);
        count.max_price = _wtoi(maxPrice);
    }

    // ������ͳ��
    if (SendMessage(hCheckByDate, BM_GETCHECK, 0, 0) == BST_CHECKED)
    {
        count.countByDate = 1;
        GetWindowText(hEditStartDate, count.startDate, DATE_LENGTH);
        GetWindowText(hEditEndDate, count.endDate, DATE_LENGTH);
    }

    if (SendMessageW(hCheckByStatus, BM_GETCHECK, 0, 0) == BST_CHECKED)
    {
		count.countByState = 1;
		int selectedIndex = SendMessage(hStatusCombo, CB_GETCURSEL, 0, 0);
		if (selectedIndex != CB_ERR)
		{
			switch (selectedIndex)
			{
			case 0: count.state = Using; break;
			case 1: count.state = Idle; break;
			case 2: count.state = Lost; break;
			case 3: count.state = Damaged; break;
			case 4: count.state = Scrapped; break;
			case 5: count.state = Repairing; break;
			default: count.state = Using; break;
			}
		}
    }
    // ִ��ͳ��
    CountEquipment(rm->equipment_list, &count);

    // ������ʾͳ�ƽ��
    int itemIndex = 0;

    // ���ͳ������
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
            lvi.pszText = L"�豸����";
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
            lvi.pszText = L"����ʵ����";
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
        lvi.pszText = L"�۸�Χ";
        ListView_InsertItem(hListView, &lvi);

        swprintf(buffer, 256, L"%d Ԫ - %d Ԫ", count.min_price, count.max_price);
        ListView_SetItemText(hListView, itemIndex, 1, buffer);
        itemIndex++;
    }

    if (count.countByDate >= 0)
    {
        lvi.mask = LVIF_TEXT;
        lvi.iItem = itemIndex;
        lvi.iSubItem = 0;
        lvi.pszText = L"���ڷ�Χ";
        ListView_InsertItem(hListView, &lvi);

        wchar_t startDate[DATE_LENGTH], endDate[DATE_LENGTH];
        mbstowcs_s(NULL, startDate, DATE_LENGTH, count.startDate, _TRUNCATE);
        mbstowcs_s(NULL, endDate, DATE_LENGTH, count.endDate, _TRUNCATE);

        swprintf(buffer, 256, L"%s - %s", startDate, endDate);
        ListView_SetItemText(hListView, itemIndex, 1, buffer);
        itemIndex++;
    }

    if (count.countByState >= 0)
    {
		lvi.mask = LVIF_TEXT;
		lvi.iItem = itemIndex;
		lvi.iSubItem = 0;
		lvi.pszText = L"�豸״̬";
		ListView_InsertItem(hListView, &lvi);
		const wchar_t* stateStr = L"";
        switch (count.state)
        {
        case Using: stateStr = L"����ʹ��"; break;
        case Idle: stateStr = L"����"; break;
        case Lost: stateStr = L"��ʧ"; break;
        case Damaged: stateStr = L"��"; break;
        case Scrapped: stateStr = L"����"; break;
        default: stateStr = L"����ά��"; break;
        }
		ListView_SetItemText(hListView, itemIndex, 1, stateStr);
		itemIndex++;
    }

    // ���ͳ�ƽ��
    lvi.mask = LVIF_TEXT;
    lvi.iItem = itemIndex;
    lvi.iSubItem = 0;
    lvi.pszText = L"ͳ�ƽ��";
    ListView_InsertItem(hListView, &lvi);

    swprintf(buffer, 256, L"���� %d ���豸", count.count);
    ListView_SetItemText(hListView, itemIndex, 1, buffer);
}

// �˻�ͳ�ƹ���
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

// ִ���˻�ͳ��
void ExecuteAccountStatistics(HWND hWnd, HWND hListView)
{
    AccountCount count;
    memset(&count, 0, sizeof(AccountCount));

    count.countByType = -1;
    count.countByRoom = -1;
    count.count = 0;

    ResourceManager* rm = GetResourceManage();
    if (!rm) return;

    // ���˻�����ͳ��
    if (SendMessage(hCheckByAccountType, BM_GETCHECK, 0, 0) == BST_CHECKED)
    {
        count.countByType = 1;
        int selectedIndex = SendMessage(hAccountTypeCombo, CB_GETCURSEL, 0, 0);
        if (selectedIndex != CB_ERR)
        {
            // ����������ѡ�������˻�����
            switch (selectedIndex)
            {
            case 0: count.type = Admin; break;
            case 1: count.type = Experimenter; break;
            case 2: count.type = User; break;
            default: count.type = Unknow; break;
            }
        }
    }

    // ��ʵ����ͳ��
    if (SendMessage(hCheckByAccountLabroom, BM_GETCHECK, 0, 0) == BST_CHECKED)
    {
        count.countByRoom = 1;
        int selectedIndex = SendMessage(hAccountLabroomCombo, CB_GETCURSEL, 0, 0);
        if (selectedIndex != CB_ERR)
        {
            wchar_t buffer[100];
            SendMessage(hAccountLabroomCombo, CB_GETLBTEXT, selectedIndex, (LPARAM)buffer);
            count.room_id = _wtoi(buffer);
        }
    }

    // ִ��ͳ��
    CountAccount(rm->account_list, &count);

    // ��ʾͳ�ƽ��
    int itemIndex = 0;

    // ���ͳ������
    LVITEM lvi;
    wchar_t buffer[256];

    if (count.countByType >= 0)
    {
        lvi.mask = LVIF_TEXT;
        lvi.iItem = itemIndex;
        lvi.iSubItem = 0;
        lvi.pszText = L"�˻�����";
        ListView_InsertItem(hListView, &lvi);

        const wchar_t* typeStr = L"";
        switch (count.type)
        {
        case Admin: typeStr = L"����Ա"; break;
        case Experimenter: typeStr = L"ʵ��Ա"; break;
        case User: typeStr = L"һ���û�"; break;
        default: typeStr = L"δ֪"; break;
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
            lvi.pszText = L"����ʵ����";
            ListView_InsertItem(hListView, &lvi);

            ListView_SetItemText(hListView, itemIndex, 1, labroom->name);
            itemIndex++;
        }
    }

    // ���ͳ�ƽ��
    lvi.mask = LVIF_TEXT;
    lvi.iItem = itemIndex;
    lvi.iSubItem = 0;
    lvi.pszText = L"ͳ�ƽ��";
    ListView_InsertItem(hListView, &lvi);

    swprintf(buffer, 256, L"���� %d ���˻�", count.count);
    ListView_SetItemText(hListView, itemIndex, 1, buffer);
}

// ����ͼ���ݽṹ
typedef struct _PieChartItem {
    wchar_t label[50];         //С���ı�ǩ
	int value;                 //С����ֵ
	COLORREF color;            //С������ɫ
} PieChartItem;

// ����ͼ��������
typedef struct _PieChartData {
    wchar_t title[100];                //����ͼ����
    PieChartItem items[MAX_PIE_ITEMS]; //����ͼ��С��
	int itemCount;                     //С����
    int total;                         //ͳ��������
} PieChartData;

// ȫ�ֱ���
static PieChartData labRoomPieData;
static PieChartData categoryPieData;
static BOOL showLabRoomChart = TRUE;  // TRUE��ʾʵ����ͳ��ͼ��FALSE��ʾ���ͳ��ͼ
static HWND hSwitchButton = NULL;     // �л���ť���

// ��ʼ������ͼ����
void InitPieChartData(PieChartData* data, const wchar_t* title) 
{
    wcscpy_s(data->title, 100, title);
    data->itemCount = 0;
    data->total = 0;
}

// ���С��
void AddPieChartItem(PieChartData* data, const wchar_t* label, int value, COLORREF color)
{
    if (data->itemCount >= MAX_PIE_ITEMS) return;

    wcscpy_s(data->items[data->itemCount].label, 50, label);
    data->items[data->itemCount].value = value;
    data->items[data->itemCount].color = color;
    data->itemCount++;
    data->total += value;
}

// �ռ�ʵ�����豸����
void CollectLabRoomEquipmentStats() 
{
    ResourceManager* rm = GetResourceManage();
    if (!rm || !rm->laboratory_list || !rm->equipment_list) return;

    InitPieChartData(&labRoomPieData, L"��ʵ�����豸����ͳ��");
    // ������ݵ�����ͼ
    Node* temp = rm->laboratory_list->head->next;
    for (int i = 0; temp; i++,temp=temp->next)
    {
        EquipmentsCount a;
		a.countByRoom = 1;
		LabRoom* labRoom = (LabRoom*)temp->data;
		a.room_id = labRoom->id;
		a.countByCategory = -1;
		a.countByDate = -1;
		a.countByPrice = -1;
		a.count = 0;
		CountEquipment(rm->equipment_list, &a);
        if (a.count> 0) 
        {
            // ������ɫ - ʹ�ò�ͬ��RGBֵ
            COLORREF color = RGB(
                50 + (i * 67) % 200,
                50 + ((i + 3) * 111) % 200,
                50 + ((i + 7) * 157) % 200
            );
            AddPieChartItem(&labRoomPieData, labRoom->name, a.count, color);
        }
    }
}

// �ռ��豸�������ͳ������
void CollectCategoryEquipmentStats() {
    ResourceManager* rm = GetResourceManage();
    if (!rm || !rm->category_list || !rm->equipment_list) return;

    InitPieChartData(&categoryPieData, L"���豸�������ͳ��");
    // ������ݵ�����ͼ
	Node* category = rm->category_list->head->next;
    for (int i = 0; category; i++,category=category->next)
    {
		Category* cat = (Category*)category->data;
        EquipmentsCount a;
		a.countByCategory = 1;
		a.categoryId = cat->id;
		a.countByRoom = -1;
		a.countByDate = -1;
		a.countByPrice = -1;
		a.count = 0;
		CountEquipment(rm->equipment_list, &a);
        if (a.count > 0) {
            // ������ɫ - ʹ�ò�ͬ��RGBֵ
            COLORREF color = RGB(
                50 + (i * 61) % 200,
                50 + ((i + 5) * 101) % 200,
                50 + ((i + 9) * 173) % 200
            );
            AddPieChartItem(&categoryPieData, cat->name, a.count, color);
        }
    }
}

// ��������ͼ
void DrawPieChart(HDC hdc, RECT rect, const PieChartData* pieData) {
    if (pieData->itemCount == 0 || pieData->total == 0) {
        SetTextColor(hdc, RGB(0, 0, 0));
        SetBkMode(hdc, TRANSPARENT);
        DrawText(hdc, L"û��ͳ������", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        return;
    }

    int centerX = (rect.left + rect.right) / 2;
    int centerY = (rect.top + rect.bottom) / 2;
    // �����ռ��ͼ���ͱ���
    int radius = min((rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2) - 70; 

    // �ռ䲻��
    if (radius < 10) return; 

    // ����
    RECT titleRect = { rect.left, rect.top, rect.right, rect.top + 30 };
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, pieData->title, -1, &titleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // ��������
    int startAngle = 0;
    int endAngle;

    for (int i = 0; i < pieData->itemCount; i++) {
        if (pieData->items[i].value == 0) continue; 

        endAngle = startAngle + (pieData->items[i].value * 360) / pieData->total;

        HBRUSH hBrush = CreateSolidBrush(pieData->items[i].color);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        int x1 = centerX - (int)(radius * sin(startAngle * 3.14159 / 180));
        int x2 = centerY - (int)(radius * cos(startAngle * 3.14159 / 180));
		int x3 = centerX - (int)(radius * sin(endAngle * 3.14159 / 180));
		int x4 = centerY - (int)(radius * cos(endAngle * 3.14159 / 180));
        // ��������
        Pie(hdc,
            centerX - radius, centerY - radius,
            centerX + radius, centerY + radius,
            x1, x2, x3, x4);

        SelectObject(hdc, hOldBrush);
        DeleteObject(hBrush);

        startAngle = endAngle;
    }
    {
    // ����ͼ��
    int legendX = rect.left + 50;
    int legendY = centerY + radius + 20;
    int legendBoxSize = 15;
    int legendSpacing = 25;
    int legendsPerRow = 2;  // ÿ����ʾ��ͼ����
    int currentLegendInRow = 0;
    int originalLegendX = legendX;

    for (int i = 0; i < pieData->itemCount; i++) {
        if (pieData->items[i].value == 0) continue; // ���������ݵ���

        HBRUSH hBrush = CreateSolidBrush(pieData->items[i].color);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        // ����ͼ��ɫ��
        Rectangle(hdc,
            legendX, legendY,
            legendX + legendBoxSize, legendY + legendBoxSize);

        SelectObject(hdc, hOldBrush);
        DeleteObject(hBrush);

        // ����ͼ���ı�
        wchar_t legendText[100];
        swprintf(legendText, 100, L"%s: %d (%.1f%%)",
            pieData->items[i].label,
            pieData->items[i].value,
            (float)pieData->items[i].value * 100.0f / pieData->total);

        RECT legendTextRect = {
            legendX + legendBoxSize + 5,
            legendY,
            legendX + legendBoxSize + 255,
            legendY + legendBoxSize
        };

        DrawText(hdc, legendText, -1, &legendTextRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        // ����ͼ��λ��
        currentLegendInRow++;
        if (currentLegendInRow >= legendsPerRow) {
            currentLegendInRow = 0;
            legendX = originalLegendX;
            legendY += legendSpacing;
        }
        else {
            legendX += 300;  // ��һ�е�ˮƽƫ��
        }
    }
    }
}

// ˢ�»�ͼ��
void RefreshPresetStatsWindow(HWND hWnd) {
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    InvalidateRect(hWnd, &clientRect, TRUE);
}

// Ԥ��ͳ�ƴ��ڹ���
LRESULT CALLBACK PresetStatsWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HFONT hFont = NULL;

    switch (msg) {
    case WM_CREATE:
    {
        // ��������
        CreateWindow(L"STATIC", L"Ԥ��ͳ��", WS_VISIBLE | WS_CHILD | SS_CENTER,
            300, 10, 200, 30, hWnd, NULL, NULL, NULL);

        // �����л�ͼ��ť
        hSwitchButton = CreateWindow(L"BUTTON", L"�л������ͳ��ͼ", WS_VISIBLE | WS_CHILD,
            300, 460, 200, 30, hWnd, (HMENU)IDC_BTN_SWITCH_CHART, NULL, NULL);

        // �������ذ�ť
        CreateWindow(L"BUTTON", L"����", WS_VISIBLE | WS_CHILD,
            350, 500, 100, 30, hWnd, (HMENU)IDC_BTN_STATS_BACK, NULL, NULL);

        // �ռ�ͳ������
        CollectLabRoomEquipmentStats();
        CollectCategoryEquipmentStats();

        // �������õ�����
        hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"΢���ź�");

        break;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // ��������
        if (hFont) {
            SelectObject(hdc, hFont);
        }

        // ��������ͼ��������
        RECT chartRect;
        chartRect.left = 50;
        chartRect.top = 50;
        chartRect.right = 750;
        chartRect.bottom = 450;

        // ���Ʊ���
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
        FillRect(hdc, &chartRect, hBrush);
        DeleteObject(hBrush);

        // ��������ͼ
        if (showLabRoomChart) {
            DrawPieChart(hdc, chartRect, &labRoomPieData);
        }
        else {
            DrawPieChart(hdc, chartRect, &categoryPieData);
        }

        EndPaint(hWnd, &ps);
        break;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_BTN_SWITCH_CHART:
            // �л�ͼ������
            showLabRoomChart = !showLabRoomChart;

            // ���°�ť�ı�
            if (showLabRoomChart) {
                SetWindowText(hSwitchButton, L"�л������ͳ��ͼ");
            }
            else {
                SetWindowText(hSwitchButton, L"�л���ʵ����ͳ��ͼ");
            }

            // �ػ洰��
            RefreshPresetStatsWindow(hWnd);
            break;

        case IDC_BTN_STATS_BACK:
            // ������ͳ�ƽ���
            ShowWindow(hwndPresetStats, SW_HIDE);
            ShowWindow(hwndStatisticsMain, SW_SHOW);
            break;
        }
        break;

    case WM_CLOSE:
        ShowWindow(hwndPresetStats, SW_HIDE);
        ShowWindow(hwndStatisticsMain, SW_SHOW);
        break;

    case WM_DESTROY:
        if (hFont) {
            DeleteObject(hFont);
            hFont = NULL;
        }
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}


// �豸ͳ�ƴ��ڹ���
LRESULT CALLBACK EquipmentStatsWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        // ��������
        CreateWindow(L"STATIC", L"�豸ͳ��", WS_VISIBLE | WS_CHILD | SS_CENTER,
            300, 10, 200, 30, hWnd, NULL, NULL, NULL);

        // �������豸����ͳ��ѡ��
        hCheckByCategory = CreateWindow(L"BUTTON", L"���豸����ͳ��", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            70, 60, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_CATEGORY, NULL, NULL);

        CreateWindow(L"STATIC", L"�豸����:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
            220, 60, 80, 25, hWnd, NULL, NULL, NULL);

        hCategoryCombo = CreateWindow(L"COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
            310, 60, 120, 200, hWnd, (HMENU)IDC_CATEGORY_COMBO, NULL, NULL);

        // ����豸����������
        FillCategoryComboBox(hCategoryCombo);

        // ������ʵ����ͳ��ѡ��
        hCheckByLabroom = CreateWindow(L"BUTTON", L"��ʵ����ͳ��", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            70, 100, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_LABROOM, NULL, NULL);

        CreateWindow(L"STATIC", L"ʵ����:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
            220, 100, 80, 25, hWnd, NULL, NULL, NULL);

        hLabroomCombo = CreateWindow(L"COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
            310, 100, 120, 200, hWnd, (HMENU)IDC_LABROOM_COMBO, NULL, NULL);

        // ���ʵ����������
        FillLabroomComboBox(hLabroomCombo);

        // �������۸�ͳ��ѡ��
        hCheckByPrice = CreateWindow(L"BUTTON", L"���۸�ͳ��", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            70, 140, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_PRICE, NULL, NULL);

        CreateWindow(L"STATIC", L"��ͼ۸�:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
            220, 140, 80, 25, hWnd, NULL, NULL, NULL);

        hEditMinPrice = CreateWindow(L"EDIT", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
            310, 140, 80, 25, hWnd, (HMENU)IDC_EDIT_MIN_PRICE, NULL, NULL);

        CreateWindow(L"STATIC", L"��߼۸�:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
            400, 140, 80, 25, hWnd, NULL, NULL, NULL);

        hEditMaxPrice = CreateWindow(L"EDIT", L"10000", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
            490, 140, 80, 25, hWnd, (HMENU)IDC_EDIT_MAX_PRICE, NULL, NULL);

        // ����������ͳ��ѡ��
        hCheckByDate = CreateWindow(L"BUTTON", L"������ͳ��", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            70, 180, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_DATE, NULL, NULL);

        CreateWindow(L"STATIC", L"��ʼ����:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
            220, 180, 80, 25, hWnd, NULL, NULL, NULL);

        hEditStartDate = CreateWindow(L"EDIT", L"20200101", WS_VISIBLE | WS_CHILD | WS_BORDER,
            310, 180, 80, 25, hWnd, (HMENU)IDC_EDIT_START_DATE, NULL, NULL);

        CreateWindow(L"STATIC", L"��������:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
            400, 180, 80, 25, hWnd, NULL, NULL, NULL);

        hEditEndDate = CreateWindow(L"EDIT", L"20240101", WS_VISIBLE | WS_CHILD | WS_BORDER,
            490, 180, 80, 25, hWnd, (HMENU)IDC_EDIT_END_DATE, NULL, NULL);

		//�������豸״̬ͳ��ѡ��
		hCheckByStatus = CreateWindow(L"BUTTON", L"���豸״̬ͳ��", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			70, 220, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_STATUS, NULL, NULL);
		CreateWindow(L"STATIC", L"�豸״̬:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
			220, 220, 80, 25, hWnd, NULL, NULL, NULL);
		hStatusCombo = CreateWindow(L"COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
			310, 220, 120, 200, hWnd, (HMENU)IDC_STATUS_COMBO, NULL, NULL);
		// ����豸״̬������
		SendMessage(hStatusCombo, CB_ADDSTRING, 0, (LPARAM)L"Using");
		SendMessage(hStatusCombo, CB_ADDSTRING, 0, (LPARAM)L"Idle");
		SendMessage(hStatusCombo, CB_ADDSTRING, 0, (LPARAM)L"Lost");
		SendMessage(hStatusCombo, CB_ADDSTRING, 0, (LPARAM)L"Damaged");
		SendMessage(hStatusCombo, CB_ADDSTRING, 0, (LPARAM)L"Scrapped");
		SendMessage(hStatusCombo, CB_ADDSTRING, 0, (LPARAM)L"Repairing");
		SendMessage(hStatusCombo, CB_SETCURSEL, 0, 0);


        // ����ͳ�ƽ���б���ͼ
        hEquipmentResultList = CreateWindowEx(0, WC_LISTVIEW, NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT,
            50, 260, 700, 210, hWnd, (HMENU)IDC_EQUIPMENT_STATS_RESULT_LIST, NULL, NULL);

        // �����
        LVCOLUMN lvc;
        lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

        lvc.pszText = L"ͳ����Ŀ";
        lvc.cx = 200;
        lvc.iSubItem = 0;
        ListView_InsertColumn(hEquipmentResultList, 0, &lvc);

        lvc.pszText = L"ͳ�ƽ��";
        lvc.cx = 470;
        lvc.iSubItem = 1;
        ListView_InsertColumn(hEquipmentResultList, 1, &lvc);

        // ����������ť
        CreateWindow(L"BUTTON", L"ִ��ͳ��", WS_VISIBLE | WS_CHILD,
            250, 490, 100, 30, hWnd, (HMENU)IDC_BTN_EQUIPMENT_STATS_EXECUTE, NULL, NULL);

        CreateWindow(L"BUTTON", L"����", WS_VISIBLE | WS_CHILD,
            450, 490, 100, 30, hWnd, (HMENU)IDC_BTN_EQUIPMENT_STATS_BACK, NULL, NULL);

        break;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BTN_EQUIPMENT_STATS_EXECUTE:
            // ��ս���б�
            ListView_DeleteAllItems(hEquipmentResultList);

            // ִ���豸ͳ��
            ExecuteEquipmentStatistics(hWnd, hEquipmentResultList);
            break;

        case IDC_BTN_EQUIPMENT_STATS_BACK:
            // ������ͳ�ƽ���
            ShowWindow(hwndEquipmentStats, SW_HIDE);
            ShowWindow(hwndStatisticsMain, SW_SHOW);
            break;
        }
        break;

    case WM_CLOSE:
        ShowWindow(hwndEquipmentStats, SW_HIDE);
        ShowWindow(hwndStatisticsMain, SW_SHOW);
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

// �˻�ͳ�ƴ��ڹ���
LRESULT CALLBACK AccountStatsWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        // ��������
        CreateWindow(L"STATIC", L"�˻�ͳ��", WS_VISIBLE | WS_CHILD | SS_CENTER,
            300, 10, 200, 30, hWnd, NULL, NULL, NULL);

        // �������˻�����ͳ��ѡ��
        hCheckByAccountType = CreateWindow(L"BUTTON", L"���˻�����ͳ��", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            70, 60, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_ACCOUNT_TYPE, NULL, NULL);

        CreateWindow(L"STATIC", L"�˻�����:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
            220, 60, 80, 25, hWnd, NULL, NULL, NULL);

        hAccountTypeCombo = CreateWindow(L"COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
            310, 60, 150, 200, hWnd, (HMENU)IDC_ACCOUNT_TYPE_COMBO, NULL, NULL);

        // ����˻�����������
        SendMessage(hAccountTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"����Ա");
        SendMessage(hAccountTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"ʵ��Ա");
        SendMessage(hAccountTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"һ���û�");
        SendMessage(hAccountTypeCombo, CB_SETCURSEL, 0, 0);

        // ������ʵ����ͳ��ѡ������ʵ��Ա��
        hCheckByAccountLabroom = CreateWindow(L"BUTTON", L"��ʵ����ͳ��", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            70, 100, 150, 25, hWnd, (HMENU)IDC_CHECK_BY_ACCOUNT_LABROOM, NULL, NULL);

        CreateWindow(L"STATIC", L"ʵ����:", WS_VISIBLE | WS_CHILD | SS_RIGHT,
            220, 100, 80, 25, hWnd, NULL, NULL, NULL);

        hAccountLabroomCombo = CreateWindow(L"COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
            310, 100, 120, 200, hWnd, (HMENU)IDC_ACCOUNT_LABROOM_COMBO, NULL, NULL);

        // ���ʵ����������
        FillLabroomComboBox(hAccountLabroomCombo);

        // ����ͳ�ƽ���б���ͼ
        hAccountResultList = CreateWindowEx(0, WC_LISTVIEW, NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT,
            50, 150, 700, 300, hWnd, (HMENU)IDC_ACCOUNT_STATS_RESULT_LIST, NULL, NULL);

        // �����
        LVCOLUMN lvc;
        lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

        lvc.pszText = L"ͳ����Ŀ";
        lvc.cx = 200;
        lvc.iSubItem = 0;
        ListView_InsertColumn(hAccountResultList, 0, &lvc);

        lvc.pszText = L"ͳ�ƽ��";
        lvc.cx = 470;
        lvc.iSubItem = 1;
        ListView_InsertColumn(hAccountResultList, 1, &lvc);

        // ����������ť
        CreateWindow(L"BUTTON", L"ִ��ͳ��", WS_VISIBLE | WS_CHILD,
            250, 490, 100, 30, hWnd, (HMENU)IDC_BTN_ACCOUNT_STATS_EXECUTE, NULL, NULL);

        CreateWindow(L"BUTTON", L"����", WS_VISIBLE | WS_CHILD,
            450, 490, 100, 30, hWnd, (HMENU)IDC_BTN_ACCOUNT_STATS_BACK, NULL, NULL);

        break;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BTN_ACCOUNT_STATS_EXECUTE:
            // ��ս���б�
            ListView_DeleteAllItems(hAccountResultList);

            // ִ���˻�ͳ��
            ExecuteAccountStatistics(hWnd, hAccountResultList);
            break;

        case IDC_BTN_ACCOUNT_STATS_BACK:
            // ������ͳ�ƽ���
            ShowWindow(hwndAccountStats, SW_HIDE);
            ShowWindow(hwndStatisticsMain, SW_SHOW);
            break;
        }
        break;

    case WM_CLOSE:
        ShowWindow(hwndAccountStats, SW_HIDE);
        ShowWindow(hwndStatisticsMain, SW_SHOW);
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

// ͳ�������洰�ڹ���
LRESULT CALLBACK StatisticsMainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        // ��������
        CreateWindow(L"STATIC", L"ͳ�ƹ���", WS_VISIBLE | WS_CHILD | SS_CENTER,
            150, 10, 100, 30, hWnd, NULL, NULL, NULL);

        // �����ӹ��ܰ�ť - ��ť��С������λ��
        CreateWindow(L"BUTTON", L"Ԥ��ͳ��", WS_VISIBLE | WS_CHILD,
            125, 60, 150, 40, hWnd, (HMENU)IDC_BTN_PRESET_STATS, NULL, NULL);

        CreateWindow(L"BUTTON", L"�豸ͳ��", WS_VISIBLE | WS_CHILD,
            125, 110, 150, 40, hWnd, (HMENU)IDC_BTN_EQUIPMENT_STATS, NULL, NULL);

        CreateWindow(L"BUTTON", L"�˻�ͳ��", WS_VISIBLE | WS_CHILD,
            125, 160, 150, 40, hWnd, (HMENU)IDC_BTN_ACCOUNT_STATS, NULL, NULL);

        // �������ذ�ť
        CreateWindow(L"BUTTON", L"����", WS_VISIBLE | WS_CHILD,
            125, 220, 150, 40, hWnd, (HMENU)IDC_BTN_STATS_BACK, NULL, NULL);

        // �����Ӵ���
        WNDCLASS wc = { 0 };

        // Ԥ��ͳ�ƴ���
        wc.lpfnWndProc = PresetStatsWndProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = L"PresetStatsWindow";
        RegisterClass(&wc);

        // �豸ͳ�ƴ���
        wc.lpfnWndProc = EquipmentStatsWndProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = L"EquipmentStatsWindow";
        RegisterClass(&wc);

        // �˻�ͳ�ƴ���
        wc.lpfnWndProc = AccountStatsWndProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = L"AccountStatsWindow";
        RegisterClass(&wc);

        // ����Ԥ��ͳ�ƴ���
        hwndPresetStats = CreateWindow(L"PresetStatsWindow", L"Ԥ��ͳ��",
            WS_OVERLAPPED | WS_SYSMENU, 100, 50, 800, 600, hWnd, NULL, GetModuleHandle(NULL), NULL);

        // �����豸ͳ�ƴ���
        hwndEquipmentStats = CreateWindow(L"EquipmentStatsWindow", L"�豸ͳ��",
            WS_OVERLAPPED | WS_SYSMENU, 100, 50, 800, 600, hWnd, NULL, GetModuleHandle(NULL), NULL);

        // �����˻�ͳ�ƴ���
        hwndAccountStats = CreateWindow(L"AccountStatsWindow", L"�˻�ͳ��",
            WS_OVERLAPPED | WS_SYSMENU, 100, 50, 800, 600, hWnd, NULL, GetModuleHandle(NULL), NULL);

        break;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BTN_PRESET_STATS:
            // ��ʾԤ��ͳ�ƴ���
            ShowWindow(hwndStatisticsMain, SW_HIDE);
            ShowWindow(hwndPresetStats, SW_SHOW);
            break;

        case IDC_BTN_EQUIPMENT_STATS:
            // ��ʾ�豸ͳ�ƴ���
            ShowWindow(hwndStatisticsMain, SW_HIDE);
            ShowWindow(hwndEquipmentStats, SW_SHOW);
            break;

        case IDC_BTN_ACCOUNT_STATS:
            // ��ʾ�˻�ͳ�ƴ���
            ShowWindow(hwndStatisticsMain, SW_HIDE);
            ShowWindow(hwndAccountStats, SW_SHOW);
            break;

        case IDC_BTN_STATS_BACK:
            // ������һ���˵�
            DestroyWindow(hwndPresetStats);
            DestroyWindow(hwndEquipmentStats);
            DestroyWindow(hwndAccountStats);
            DestroyWindow(hwndStatisticsMain);
            break;
        }
        break;

    case WM_CLOSE:
        DestroyWindow(hwndPresetStats);
        DestroyWindow(hwndEquipmentStats);
        DestroyWindow(hwndAccountStats);
        DestroyWindow(hwndStatisticsMain);
        break;

    case WM_DESTROY:
        hwndPresetStats = NULL;
        hwndEquipmentStats = NULL;
        hwndAccountStats = NULL;
        hwndStatisticsMain = NULL;
        ShowWindow(hwndSystemMaintenance, SW_SHOW);
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// ��ʾͳ�ƹ���������
void ShowStatisticsWindow(HWND hWnd)
{
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = StatisticsMainWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"StatisticsMainWindow";

    RegisterClass(&wc);

    hwndStatisticsMain = CreateWindow(L"StatisticsMainWindow", L"ͳ�ƹ���",
        WS_OVERLAPPED | WS_SYSMENU, 100, 50,450 , 350, hWnd, NULL, GetModuleHandle(NULL), NULL);

    ShowWindow(hwndStatisticsMain, SW_SHOW);
    UpdateWindow(hwndStatisticsMain);
}

