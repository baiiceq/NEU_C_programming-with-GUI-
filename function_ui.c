#include "function_ui.h"
#include <wchar.h>
#include <commctrl.h>

extern HWND hwndAdminManagement;

HWND hwndfunc1, hwndfunc2;
HWND hMainTab;

int GetInputNumber(HWND hwnd, int editControlID)
{
    wchar_t text[256];
    GetWindowText(GetDlgItem(hwnd, editControlID), text, 256);

    // ���ı�ת��Ϊ����
    int number = _wtoi(text);

    return number;
}

void ShowNumber(HWND hwnd, int number)
{
    wchar_t buffer[256];
    swprintf(buffer, 256, L"����: %d", number); 

    MessageBox(hwnd, buffer, L"��ʾ", MB_OK);
}

void CreateButtons1(HWND hWnd, int tabIndex)
{
    HWND hButton;
    while ((hButton = GetDlgItem(hWnd, IDC_BUTTON_ADD)) != NULL)
        DestroyWindow(hButton);
    while ((hButton = GetDlgItem(hWnd, IDC_BUTTON_DELETE)) != NULL)
        DestroyWindow(hButton);
    while ((hButton = GetDlgItem(hWnd, IDC_BUTTON_MODIFY)) != NULL)
        DestroyWindow(hButton);

    HWND hEdit;
    while ((hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME)) != NULL)
        DestroyWindow(hEdit);
    while ((hEdit = GetDlgItem(hWnd, IDC_EDIT_PRICE)) != NULL)
        DestroyWindow(hEdit);
    while ((hEdit = GetDlgItem(hWnd, IDC_EDIT_ROOM_ID)) != NULL)
        DestroyWindow(hEdit);
    while ((hEdit = GetDlgItem(hWnd, IDC_DATEPICKER_DATE)) != NULL)
        DestroyWindow(hEdit);
    while ((hEdit = GetDlgItem(hWnd, IDC_COMBOX_CATEGORY)) != NULL)
        DestroyWindow(hEdit);
    while ((hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME_CHANGE)) != NULL)
        DestroyWindow(hEdit);
    while ((hEdit = GetDlgItem(hWnd, IDC_EDIT_PRICE_CHANGE)) != NULL)
        DestroyWindow(hEdit);
    while ((hEdit = GetDlgItem(hWnd, IDC_EDIT_ROOM_ID_CHANGE)) != NULL)
        DestroyWindow(hEdit);
    while ((hEdit = GetDlgItem(hWnd, IDC_DATEPICKER_DATE_CHANGE)) != NULL)
        DestroyWindow(hEdit);
    while ((hEdit = GetDlgItem(hWnd, IDC_COMBOX_CATEGORY_CHANGE)) != NULL)
        DestroyWindow(hEdit);
    while ((hEdit = GetDlgItem(hWnd, IDC_CHECKBOX_1)) != NULL)
        DestroyWindow(hEdit);
    while ((hEdit = GetDlgItem(hWnd, IDC_CHECKBOX_2)) != NULL)
        DestroyWindow(hEdit);
    while ((hEdit = GetDlgItem(hWnd, IDC_LISTVIEW)) != NULL)
        DestroyWindow(hEdit);
    while ((hEdit = GetDlgItem(hWnd, IDC_STATIC_1)) != NULL)
        DestroyWindow(hEdit);
    while ((hEdit = GetDlgItem(hWnd, IDC_BUTTON_EXIT)) != NULL)
        DestroyWindow(hEdit);
    while ((hEdit = GetDlgItem(hWnd, IDC_CHECKBOX_STATE)) != NULL)
        DestroyWindow(hEdit);
    while ((hEdit = GetDlgItem(hWnd, IDC_COMBOX_STATE)) != NULL)
        DestroyWindow(hEdit);
    while ((hEdit = GetDlgItem(hWnd, IDC_EDIT_PASSWORD_CHANGE)) != NULL)
        DestroyWindow(hEdit);
    while ((hEdit = GetDlgItem(hWnd, IDC_EDIT_PASSWORD)) != NULL)
        DestroyWindow(hEdit); 
    while ((hEdit = GetDlgItem(hWnd, IDC_EDIT_COUNT)) != NULL)
        DestroyWindow(hEdit);
    while ((hEdit = GetDlgItem(hWnd, IDC_BUTTON_COUNT)) != NULL)
        DestroyWindow(hEdit);

    CreateWindow(L"BUTTON", L"����", WS_VISIBLE | WS_CHILD,
        600, 495, 120, 40, hWnd, (HMENU)IDC_BUTTON_EXIT, GetModuleHandle(NULL), NULL);

    ResourceManager* rm = GetResourceManage();


    switch (tabIndex)
    {
    case 0:
    {
        CreateWindow(L"BUTTON", L"���", WS_VISIBLE | WS_CHILD,
            600, 150, 120, 40, hWnd, (HMENU)IDC_BUTTON_ADD, GetModuleHandle(NULL), NULL);
        CreateWindow(L"BUTTON", L"ɾ��", WS_VISIBLE | WS_CHILD,
            600, 240, 120, 40, hWnd, (HMENU)IDC_BUTTON_DELETE, GetModuleHandle(NULL), NULL);
        CreateWindow(L"BUTTON", L"�޸�", WS_VISIBLE | WS_CHILD,
            600, 400, 120, 40, hWnd, (HMENU)IDC_BUTTON_MODIFY, GetModuleHandle(NULL), NULL);

        CreateWindow(L"STATIC", L"ѡ����ӵ�����", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 580, 60, 160, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);

        HWND hEdit = CreateWindowEx(
            0, L"EDIT", L"0",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
            580, 100, 150, 25, hWnd, (HMENU)IDC_EDIT_COUNT, NULL, NULL);

        HWND hSpin = CreateWindowEx(
            0, UPDOWN_CLASS, NULL,
            WS_CHILD | WS_VISIBLE | UDS_ALIGNRIGHT | UDS_SETBUDDYINT | UDS_ARROWKEYS,
            0, 0, 0, 0, hWnd, (HMENU)IDC_BUTTON_COUNT, NULL, NULL);

        SendMessage(hSpin, UDM_SETBUDDY, (WPARAM)hEdit, 0);
        SetWindowText(hEdit, L"1");
        SendMessage(hSpin, UDM_SETRANGE, 0, MAKELPARAM(100, 1));

        CreateWindow(L"STATIC", L"�豸����", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 60, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 60, 130, 25, hWnd, (HMENU)IDC_EDIT_NAME, NULL, NULL);

        CreateWindow(L"STATIC", L"�豸�۸�", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 100, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 100, 130, 25, hWnd, (HMENU)IDC_EDIT_PRICE, NULL, NULL);

        CreateWindow(L"STATIC", L"����ʵ����ID", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 140, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 140, 130, 25, hWnd, (HMENU)IDC_EDIT_ROOM_ID, NULL, NULL);

        CreateWindow(L"STATIC", L"����ʱ��", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 300, 60, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindowEx(0, DATETIMEPICK_CLASS, NULL,
            WS_BORDER | WS_CHILD | WS_VISIBLE | DTS_SHORTDATEFORMAT,
            400, 60, 130, 25, hWnd, (HMENU)IDC_DATEPICKER_DATE, GetModuleHandle(NULL), NULL);

        CreateWindow(L"STATIC", L"�豸����", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 300, 100, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        HWND hComboBox = CreateWindow(L"COMBOBOX", NULL,
            WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_BORDER,
            400, 100, 130, 300, hWnd, (HMENU)IDC_COMBOX_CATEGORY, GetModuleHandle(NULL), NULL);

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

        CreateWindow(L"STATIC", L"�޸�����", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 390, 140, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);

        CreateWindow(L"STATIC", L"������", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 300, 180, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 400, 180, 130, 25, hWnd, (HMENU)IDC_EDIT_NAME_CHANGE, NULL, NULL);

        CreateWindow(L"STATIC", L"�¼۸�", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 300, 220, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 400, 220, 130, 25, hWnd, (HMENU)IDC_EDIT_PRICE_CHANGE, NULL, NULL);

        CreateWindow(L"STATIC", L"��ʵ����", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 300, 260, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 400, 260, 130, 25, hWnd, (HMENU)IDC_EDIT_ROOM_ID_CHANGE, NULL, NULL);

        CreateWindow(L"STATIC", L"��ʱ��", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 300, 300, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindowEx(0, DATETIMEPICK_CLASS, NULL,
            WS_BORDER | WS_CHILD | WS_VISIBLE | DTS_SHORTDATEFORMAT,
            400, 300, 130, 25, hWnd, (HMENU)IDC_DATEPICKER_DATE_CHANGE, GetModuleHandle(NULL), NULL);

        CreateWindow(L"STATIC", L"������", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 300, 340, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        hComboBox = CreateWindow(L"COMBOBOX", NULL,
            WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_BORDER,
            400, 340, 130, 300, hWnd, (HMENU)IDC_COMBOX_CATEGORY_CHANGE, GetModuleHandle(NULL), NULL);

        temp = rm->category_list->head;
        for (size_t i = 0; i < count; i++)
        {
            temp = temp->next;
            Category* category = (Category*)temp->data;
            wchar_t buffer[50];
            swprintf(buffer, 50, L"%d. %s", (int)(i + 1), category->name);
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)buffer);
        }
        SendMessage(hComboBox, CB_SETCURSEL, 0, 0);

        CreateWindow(L"STATIC", L"��״̬", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 300, 380, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        hComboBox = CreateWindow(L"COMBOBOX", NULL,
            WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_BORDER,
            400, 380, 130, 300, hWnd, (HMENU)IDC_COMBOX_STATE, GetModuleHandle(NULL), NULL);

        for (int i = 0; i < 6; i++)
        {
            wchar_t buffer[16];
            state_to_string((EquipmentState)i, buffer);
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)buffer);
        }
        SendMessage(hComboBox, CB_SETCURSEL, 0, 0);

        CreateWindowEx(0, L"BUTTON", L"�Ƿ��޸�ʱ��",
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            320, 415, 120, 30, hWnd, (HMENU)IDC_CHECKBOX_1, GetModuleHandle(NULL), NULL);
        CreateWindowEx(0, L"BUTTON", L"�Ƿ��޸�����",
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            440, 415, 120, 30, hWnd, (HMENU)IDC_CHECKBOX_2, GetModuleHandle(NULL), NULL);
        CreateWindowEx(0, L"BUTTON", L"�Ƿ��޸�״̬",
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            320, 450, 120, 30, hWnd, (HMENU)IDC_CHECKBOX_STATE, GetModuleHandle(NULL), NULL);

        HWND hListView = CreateWindowExW(0, WC_LISTVIEW, NULL,
            WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS,
            30, 200, 280, 260, hWnd, (HMENU)IDC_LISTVIEW, GetModuleHandle(NULL), NULL);

        ListView_DeleteAllItems(hListView);

        LVCOLUMNW lvc;
        lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

        lvc.pszText = L"�豸ID";
        lvc.cx = 60;
        lvc.iSubItem = 0;
        ListView_InsertColumn(hListView, 0, &lvc);

        lvc.pszText = L"�豸����";
        lvc.cx = 90;
        lvc.iSubItem = 1;
        ListView_InsertColumn(hListView, 1, &lvc);

        lvc.pszText = L"�豸����";
        lvc.cx = 80;
        lvc.iSubItem = 2;
        ListView_InsertColumn(hListView, 2, &lvc);

        lvc.pszText = L"����ʵ����";
        lvc.cx = 80;
        lvc.iSubItem = 3;
        ListView_InsertColumn(hListView, 3, &lvc);

        lvc.pszText = L"�۸�";
        lvc.cx = 60;
        lvc.iSubItem = 4;
        ListView_InsertColumn(hListView, 4, &lvc);

        lvc.pszText = L"��������";
        lvc.cx = 80;
        lvc.iSubItem = 5;
        ListView_InsertColumn(hListView, 5, &lvc);

        lvc.pszText = L"�豸״̬";
        lvc.cx = 80;
        lvc.iSubItem = 5;
        ListView_InsertColumn(hListView, 6, &lvc);

        temp = rm->equipment_list->head;
        count = rm->equipment_list->size;
        LVITEM lvItem;
        for (size_t i = 0; i < count; i++)
        {
            temp = temp->next;
            ExperimentalEquipment* ee = (ExperimentalEquipment*)temp->data;

            ZeroMemory(&lvItem, sizeof(LVITEM));
            lvItem.mask = LVIF_TEXT;
            lvItem.iItem = i;
            lvItem.iSubItem = 0;
            wchar_t idBuffer[16];
            swprintf_s(idBuffer, 16, L"%d", ee->id);
            lvItem.pszText = idBuffer;
            ListView_InsertItem(hListView, &lvItem);

            ListView_SetItemText(hListView, i, 1, ee->name);

            ListView_SetItemText(hListView, i, 2, ee->category->name);

            wchar_t roomBuffer[16];
            swprintf_s(roomBuffer, 16, L"%d", ee->room_id);
            ListView_SetItemText(hListView, i, 3, roomBuffer);

            wchar_t priceBuffer[16];
            swprintf_s(priceBuffer, 16, L"%d", ee->price);
            ListView_SetItemText(hListView, i, 4, priceBuffer);

            ListView_SetItemText(hListView, i, 5, ee->purchase_date);

            wchar_t stateBuffer[16];
            state_to_string(ee->state, stateBuffer);
            ListView_SetItemText(hListView, i, 6, stateBuffer);

        }
        break;
    }
    case 1:
    {
        CreateWindow(L"BUTTON", L"���", WS_VISIBLE | WS_CHILD,
            600, 80, 120, 40, hWnd, (HMENU)IDC_BUTTON_ADD, GetModuleHandle(NULL), NULL);
        CreateWindow(L"BUTTON", L"ɾ��", WS_VISIBLE | WS_CHILD,
            600, 240, 120, 40, hWnd, (HMENU)IDC_BUTTON_DELETE, GetModuleHandle(NULL), NULL);
        CreateWindow(L"BUTTON", L"�޸�", WS_VISIBLE | WS_CHILD,
            600, 400, 120, 40, hWnd, (HMENU)IDC_BUTTON_MODIFY, GetModuleHandle(NULL), NULL);

        CreateWindow(L"STATIC", L"��������", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 60, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 60, 130, 25, hWnd, (HMENU)IDC_EDIT_NAME, NULL, NULL);

        CreateWindow(L"STATIC", L"�޸�����", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 390, 180, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);

        CreateWindow(L"STATIC", L"������", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 300, 220, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 400, 220, 130, 25, hWnd, (HMENU)IDC_EDIT_NAME_CHANGE, NULL, NULL);

        HWND hListView = CreateWindowExW(0, WC_LISTVIEW, NULL,
            WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS,
            30, 200, 280, 260, hWnd, (HMENU)IDC_LISTVIEW, GetModuleHandle(NULL), NULL);

        ListView_DeleteAllItems(hListView);

        LVCOLUMNW lvc;
        lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

        lvc.pszText = L"ʵ����ID";
        lvc.cx = 100;
        lvc.iSubItem = 0;
        ListView_InsertColumn(hListView, 0, &lvc);

        lvc.pszText = L"ʵ��������";
        lvc.cx = 150;
        lvc.iSubItem = 1;
        ListView_InsertColumn(hListView, 1, &lvc);

        Node* temp = rm->laboratory_list->head;
        size_t count = rm->laboratory_list->size;
        LVITEM lvItem;
        for (size_t i = 0; i < count; i++)
        {
            temp = temp->next;
            LabRoom* labroom = (LabRoom*)temp->data;

            ZeroMemory(&lvItem, sizeof(LVITEM));
            lvItem.mask = LVIF_TEXT;
            lvItem.iItem = i;
            lvItem.iSubItem = 0;
            wchar_t idBuffer[16];
            swprintf_s(idBuffer, 16, L"%d", labroom->id);
            lvItem.pszText = idBuffer;
            ListView_InsertItem(hListView, &lvItem);

            ListView_SetItemText(hListView, i, 1, labroom->name);
        }
        break;
    }
    case 2:
    {
        CreateWindow(L"BUTTON", L"���", WS_VISIBLE | WS_CHILD,
            600, 80, 120, 40, hWnd, (HMENU)IDC_BUTTON_ADD, GetModuleHandle(NULL), NULL);
        CreateWindow(L"BUTTON", L"ɾ��", WS_VISIBLE | WS_CHILD,
            600, 240, 120, 40, hWnd, (HMENU)IDC_BUTTON_DELETE, GetModuleHandle(NULL), NULL);
        CreateWindow(L"BUTTON", L"�޸�", WS_VISIBLE | WS_CHILD,
            600, 400, 120, 40, hWnd, (HMENU)IDC_BUTTON_MODIFY, GetModuleHandle(NULL), NULL);

        CreateWindow(L"STATIC", L"�˻�����", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 60, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 60, 130, 25, hWnd, (HMENU)IDC_EDIT_NAME, NULL, NULL);
        CreateWindow(L"STATIC", L"�˻�����", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 100, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 100, 130, 25, hWnd, (HMENU)IDC_EDIT_PASSWORD, NULL, NULL);
        CreateWindow(L"STATIC", L"����ʵ����ID", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 140, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 140, 130, 25, hWnd, (HMENU)IDC_EDIT_ROOM_ID, NULL, NULL);

        CreateWindow(L"STATIC", L"�޸�����", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 390, 180, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);

        CreateWindow(L"STATIC", L"������", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 300, 220, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 400, 220, 130, 25, hWnd, (HMENU)IDC_EDIT_NAME_CHANGE, NULL, NULL);

        CreateWindow(L"STATIC", L"������", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 300, 260, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 400, 260, 130, 25, hWnd, (HMENU)IDC_EDIT_PASSWORD_CHANGE, NULL, NULL);

        CreateWindow(L"STATIC", L"��ʵ����", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 300, 300, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 400, 300, 130, 25, hWnd, (HMENU)IDC_EDIT_ROOM_ID_CHANGE, NULL, NULL);

        HWND hListView = CreateWindowExW(0, WC_LISTVIEW, NULL,
            WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS,
            30, 200, 280, 260, hWnd, (HMENU)IDC_LISTVIEW, GetModuleHandle(NULL), NULL);

        ListView_DeleteAllItems(hListView);

        LVCOLUMNW lvc;
        lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

        lvc.pszText = L"ʵ��ԱID";
        lvc.cx = 80;
        lvc.iSubItem = 0;
        ListView_InsertColumn(hListView, 0, &lvc);

        lvc.pszText = L"ʵ��Ա�˻�����";
        lvc.cx = 100;
        lvc.iSubItem = 0;
        ListView_InsertColumn(hListView, 1, &lvc);

        lvc.pszText = L"ʵ��Ա�˻�����";
        lvc.cx = 100;
        lvc.iSubItem = 0;
        ListView_InsertColumn(hListView, 2, &lvc);

        lvc.pszText = L"����ʵ���ҵ�ID";
        lvc.cx = 130;
        lvc.iSubItem = 1;
        ListView_InsertColumn(hListView, 3, &lvc);

        Node* temp = rm->account_list->head;
        size_t count = rm->account_list->size;
        LVITEM lvItem;
        int idx = 0;
        for (size_t i = 0; i < count; i++)
        {
            temp = temp->next;
            Account* account = (Account*)temp->data;

            if (account->account_type != Experimenter)
                continue;

            ZeroMemory(&lvItem, sizeof(LVITEM));
            lvItem.mask = LVIF_TEXT;
            lvItem.iItem = idx;
            lvItem.iSubItem = 0;
            wchar_t idBuffer[16];
            swprintf_s(idBuffer, 16, L"%d", account->id);
            lvItem.pszText = idBuffer;
            ListView_InsertItem(hListView, &lvItem);

            ListView_SetItemText(hListView, idx, 1, account->user_name);
            ListView_SetItemText(hListView, idx, 2, account->user_password);

            wchar_t Buffer[16];
            swprintf_s(Buffer, 16, L"%d", account->roomid);
            ListView_SetItemText(hListView, idx, 3, Buffer);
            idx++;
        }
        break;
    }
    case 3:
    {
        CreateWindow(L"BUTTON", L"���", WS_VISIBLE | WS_CHILD,
            600, 80, 120, 40, hWnd, (HMENU)IDC_BUTTON_ADD, GetModuleHandle(NULL), NULL);
        CreateWindow(L"BUTTON", L"ɾ��", WS_VISIBLE | WS_CHILD,
            600, 240, 120, 40, hWnd, (HMENU)IDC_BUTTON_DELETE, GetModuleHandle(NULL), NULL);
        CreateWindow(L"BUTTON", L"�޸�", WS_VISIBLE | WS_CHILD,
            600, 400, 120, 40, hWnd, (HMENU)IDC_BUTTON_MODIFY, GetModuleHandle(NULL), NULL);

        CreateWindow(L"STATIC", L"��������", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 60, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 60, 130, 25, hWnd, (HMENU)IDC_EDIT_NAME, NULL, NULL);
        CreateWindow(L"STATIC", L"���ͱ�������", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, 100, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 100, 130, 25, hWnd, (HMENU)IDC_EDIT_ROOM_ID, NULL, NULL);

        CreateWindow(L"STATIC", L"�޸�����", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 390, 180, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);

        CreateWindow(L"STATIC", L"������", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 300, 220, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 400, 220, 130, 25, hWnd, (HMENU)IDC_EDIT_NAME_CHANGE, NULL, NULL);
        CreateWindow(L"STATIC", L"�±�������", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 300, 300, 100, 25, hWnd, (HMENU)IDC_STATIC_1, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 400, 300, 130, 25, hWnd, (HMENU)IDC_EDIT_ROOM_ID_CHANGE, NULL, NULL);


        HWND hListView = CreateWindowExW(0, WC_LISTVIEW, NULL,
            WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS,
            30, 200, 280, 260, hWnd, (HMENU)IDC_LISTVIEW, GetModuleHandle(NULL), NULL);

        ListView_DeleteAllItems(hListView);

        LVCOLUMNW lvc;
        lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

        lvc.pszText = L"����ID";
        lvc.cx = 60;
        lvc.iSubItem = 0;
        ListView_InsertColumn(hListView, 0, &lvc);

        lvc.pszText = L"��������";
        lvc.cx = 100;
        lvc.iSubItem = 1;
        ListView_InsertColumn(hListView, 1, &lvc);

        lvc.pszText = L"��������";
        lvc.cx = 80;
        lvc.iSubItem = 2;
        ListView_InsertColumn(hListView, 2, &lvc);

        Node* temp = rm->category_list->head;
        size_t count = rm->category_list->size;
        LVITEM lvItem;
        for (size_t i = 0; i < count; i++)
        {
            temp = temp->next;
            Category* category = (Category*)temp->data;

            ZeroMemory(&lvItem, sizeof(LVITEM));
            lvItem.mask = LVIF_TEXT;
            lvItem.iItem = i;
            lvItem.iSubItem = 0;
            wchar_t idBuffer[16];
            swprintf_s(idBuffer, 16, L"%d", category->id);
            lvItem.pszText = idBuffer;
            ListView_InsertItem(hListView, &lvItem);

            ListView_SetItemText(hListView, i, 1, category->name);

            wchar_t Buffer[16];
            swprintf_s(Buffer, 16, L"%d", category->disposal_years);
            ListView_SetItemText(hListView, i, 2, Buffer);
        }
    }
    }
}

void CreateTabControl(HWND hWnd)
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

    CreateButtons1(hWnd, 0); 
}


LRESULT CALLBACK InfoManagementWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        CreateTabControl(hWnd);
        break;

    case WM_NOTIFY:
        if (((LPNMHDR)lParam)->idFrom == IDC_TAB && ((LPNMHDR)lParam)->code == TCN_SELCHANGE)
        {
            int tabIndex = TabCtrl_GetCurSel(hMainTab);
            CreateButtons1(hWnd, tabIndex);
        }
        break;

    case WM_COMMAND:
    {
        int tabIndex = TabCtrl_GetCurSel(hMainTab);
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON_ADD:
            switch (tabIndex)
            {
            case 0:
            {
                _AddExperimentalEquipment(hWnd);
                HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME); 
                SetWindowTextW(hEdit, L"");
                hEdit = GetDlgItem(hWnd, IDC_EDIT_PRICE);
                SetWindowTextW(hEdit, L"");
                hEdit = GetDlgItem(hWnd, IDC_EDIT_ROOM_ID);
                SetWindowTextW(hEdit, L"");
                break;
            }
            case 1:
            {
                _AddLabRoom(hWnd);
                HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME);
                SetWindowTextW(hEdit, L"");
                break;
            }
            case 2:
            {
                _AddAccount(hWnd);
                HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME);
                SetWindowTextW(hEdit, L"");
                hEdit = GetDlgItem(hWnd, IDC_EDIT_PASSWORD);
                SetWindowTextW(hEdit, L"");
                hEdit = GetDlgItem(hWnd, IDC_EDIT_ROOM_ID);
                SetWindowTextW(hEdit, L"");
                break;
            }
            case 3:
            {
                _AddCategory(hWnd);
                HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME);
                SetWindowTextW(hEdit, L"");
                hEdit = GetDlgItem(hWnd, IDC_EDIT_ROOM_ID);
                SetWindowTextW(hEdit, L"");
                break;
            }
            }
            break;
        case IDC_BUTTON_DELETE:
        {
            switch (tabIndex)
            {
            case 0:
            {
                _DeleteExperimentalEquipment(hWnd);
                break;
            }
            case 1:
            {
                _DeleteLabRoom(hWnd);
                break;
            }
            case 2:
            {
                _DeleteAccount(hWnd);
                break;
            }
            case 3:
            {
                _DeleteCategory(hWnd);
                break;
            }
            }
            
            break;
        }
        case IDC_BUTTON_MODIFY:
            switch (tabIndex)
            {
            case 0:
            {
                _ChangeExperimentalEquipment(hWnd);
                HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME_CHANGE);
                SetWindowTextW(hEdit, L"");
                hEdit = GetDlgItem(hWnd, IDC_EDIT_PRICE_CHANGE);
                SetWindowTextW(hEdit, L"");
                hEdit = GetDlgItem(hWnd, IDC_EDIT_ROOM_ID_CHANGE);
                SetWindowTextW(hEdit, L"");
                break;
            }
            case 1:
            {
                _ChangeLabRoom(hWnd);
                HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME_CHANGE);
                SetWindowTextW(hEdit, L"");
                break;
            }
            case 2:
            {
                _ChangeAccount(hWnd);
                HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME_CHANGE);
                SetWindowTextW(hEdit, L"");
                hEdit = GetDlgItem(hWnd, IDC_EDIT_PASSWORD_CHANGE);
                SetWindowTextW(hEdit, L"");
                hEdit = GetDlgItem(hWnd, IDC_EDIT_ROOM_ID_CHANGE);
                SetWindowTextW(hEdit, L"");
                break;
            }
            case 3:
            {
                _ChangeCategory(hWnd);
                HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_NAME_CHANGE);
                SetWindowTextW(hEdit, L"");
                hEdit = GetDlgItem(hWnd, IDC_EDIT_ROOM_ID_CHANGE);
                SetWindowTextW(hEdit, L"");
                break;
            }
            }
            break;
        case IDC_BUTTON_EXIT:
            ShowWindow(hwndfunc1, SW_HIDE);
            hwndfunc1 = NULL;
            ShowWindow(hwndAdminManagement, SW_SHOW);
            break;
        }
        break;
    }

    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        hwndfunc1 = NULL;
        ShowWindow(hwndAdminManagement, SW_SHOW);
        break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void ShowInfoManagementWindow(HWND hWnd)
{
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = InfoManagementWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"InfoManagementWindow";

    RegisterClass(&wc);

    hwndfunc1 = CreateWindow(L"InfoManagementWindow", L"������Ϣ����",
        WS_OVERLAPPED | WS_SYSMENU, 200, 100, 800, 600, hWnd, NULL, GetModuleHandle(NULL), NULL);

    ShowWindow(hwndfunc1, SW_SHOW);
    UpdateWindow(hwndfunc1);
}


