#include "main_ui.h"
#include "account.h" 
#include "configs.h"
#include "id_manager.h"
#include "resource_manager.h"
#include "equipment_management.h"
#include <string.h>
#include "function_ui2.h"

extern EquipmentManagement* em;

HWND hwndLogin, hwndRegister, hwndAdminManagement;

LRESULT CALLBACK LoginWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        CreateWindow(L"STATIC", L"用户名:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 50, 50, 80, 25, hWnd, NULL, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 50, 180, 25, hWnd, (HMENU)IDC_USERNAME, NULL, NULL);

        CreateWindow(L"STATIC", L"密码:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 50, 100, 80, 25, hWnd, NULL, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 150, 100, 180, 25, hWnd, (HMENU)IDC_PASSWORD, NULL, NULL);

        CreateWindow(L"BUTTON", L"登录", WS_VISIBLE | WS_CHILD, 60, 170, 100, 30, hWnd, (HMENU)IDC_BTN_LOGIN, NULL, NULL);
        CreateWindow(L"BUTTON", L"注册", WS_VISIBLE | WS_CHILD, 210, 170, 100, 30, hWnd, (HMENU)IDC_BTN_GOTO_REGISTER, NULL, NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BTN_LOGIN) 
        {
            wchar_t username[50], password[50];
            GetDlgItemText(hWnd, IDC_USERNAME, username, sizeof(username) / sizeof(wchar_t));
            GetDlgItemText(hWnd, IDC_PASSWORD, password, sizeof(password) / sizeof(wchar_t));

            Account* account = CreateAccount();
            wcscpy_s(account->user_name, sizeof(username) / sizeof(wchar_t), username);
            wcscpy_s(account->user_password, sizeof(password) / sizeof(wchar_t), password);
            account->user_name[wcscspn(account->user_name, L"\n")] = L'\0';
            account->user_password[wcscspn(account->user_password, L"\n")] = L'\0';
            if (!IsCorrectAccount(account))
            {
                free(account);
                MessageBox(hWnd, L"用户名或密码错误", L"错误", MB_ICONERROR);
            }
            else
            {
                MessageBox(hWnd, L"登录成功！", L"提示", MB_OK);

                em->current_account = FindByUsername(account->user_name);

                switch (em->current_account->account_type)
                {
                case Admin:
                    ShowAdminManagementWindow(hWnd);
                    break;
                case Experimenter:
                    break;
                case User:
                    break;
                }
                ShowWindow(hwndLogin, SW_HIDE);
                hwndLogin = NULL;
            }
        }
        else if (LOWORD(wParam) == IDC_BTN_GOTO_REGISTER) 
        {
            ShowRegisterWindow(hWnd);
            ShowWindow(hwndLogin, SW_HIDE);
        }
        break;

    case WM_CLOSE:
        DestroyWindow(hWnd);
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

HWND hComboBox;

LRESULT CALLBACK RegisterWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        CreateWindow(L"STATIC", L"用户名:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 50, 30, 80, 25, hWnd, NULL, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 30, 180, 25, hWnd, (HMENU)IDC_USERNAME, NULL, NULL);

        CreateWindow(L"STATIC", L"密码:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 50, 70, 80, 25, hWnd, NULL, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 150, 70, 180, 25, hWnd, (HMENU)IDC_PASSWORD, NULL, NULL);

        CreateWindow(L"STATIC", L"确认密码:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 50, 110, 80, 25, hWnd, NULL, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 150, 110, 180, 25, hWnd, (HMENU)IDC_CONFIRM_PASSWORD, NULL, NULL);

        CreateWindow(L"BUTTON", L"注册", WS_VISIBLE | WS_CHILD, 60, 195, 100, 30, hWnd, (HMENU)IDC_BTN_REGISTER, NULL, NULL);
        CreateWindow(L"BUTTON", L"返回", WS_VISIBLE | WS_CHILD, 210, 195, 100, 30, hWnd, (HMENU)IDC_BTN_BACK, NULL, NULL);

        CreateWindow(L"STATIC", L"选择类型:", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 50, 150, 80, 25, hWnd, NULL, NULL, NULL);
        hComboBox = CreateWindow(L"COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,
            150, 150, 180, 100, hWnd, (HMENU)IDC_ACCOUNT_TYPE, NULL, NULL);

        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"管理员");
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"实验员");
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"一般用户");
        

        // 设置默认选中项
        SendMessage(hComboBox, CB_SETCURSEL, 0, 0);
        break;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BTN_REGISTER) 
        {
            wchar_t username[50], password[50], confirmPassword[50];
            GetDlgItemText(hWnd, IDC_USERNAME, username, sizeof(username) / sizeof(wchar_t));
            GetDlgItemText(hWnd, IDC_PASSWORD, password, sizeof(password) / sizeof(wchar_t));
            GetDlgItemText(hWnd, IDC_CONFIRM_PASSWORD, confirmPassword, sizeof(confirmPassword) / sizeof(wchar_t));

            if (wcscmp(password, confirmPassword) != 0)
            {
                MessageBox(hWnd, L"两次密码输入不一致！", L"错误", MB_ICONERROR);
            }
            else 
            {
                int index = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);

                Account* account = CreateAccount();
                switch (index)
                {
                case 1:
                    account->account_type = Admin;
                    break;
                case 2:
                    account->account_type = Experimenter;
                    break;
                case 3:
                    account->account_type = User;
                    break;
                }

                wcscpy_s(account->user_name, sizeof(username) / sizeof(wchar_t), username);
                wcscpy_s(account->user_password, sizeof(password) / sizeof(wchar_t), password);

                if (!ValidAccount(account))
                {
                    MessageBox(hWnd, L"账户非法！", L"错误", MB_ICONERROR);
                    free(account);
                    return NULL;
                }
                account->id = GetNewId(AccountID);
                account->roomid = -1;
                ResourceManager* resource_manager = GetResourceManage();
                LinkedList_pushback(resource_manager->account_list, account);
                MessageBox(hWnd, L"账户注册成功！", L"提示", MB_ICONINFORMATION);
            }
        }
        else if (LOWORD(wParam) == IDC_BTN_BACK) 
        {
            ShowLoginWindow(hWnd);
            ShowWindow(hwndRegister, SW_HIDE);
        }
        break;

    case WM_CLOSE:
        DestroyWindow(hWnd);
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK AdminManagementWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        CreateWindow(L"STATIC", L"实验设备管理系统", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 320, 30, 160, 30, hWnd, NULL, NULL, NULL);
        CreateWindow(L"STATIC", L"当前权限 : 管理员", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 320, 70, 160, 30, hWnd, NULL, NULL, NULL);

        CreateWindow(L"BUTTON", L"基本信息管理", WS_VISIBLE | WS_CHILD, 90, 120, 120, 40, hWnd, (HMENU)ID_BTN_INFO_MANAGE, NULL, NULL);
        CreateWindow(L"STATIC", L"对实验中心的房间、实验员、设备类别、设备的基本信息进行管理", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 240, 120, 480, 40, hWnd, NULL, NULL, NULL);

        CreateWindow(L"BUTTON", L"信息查询", WS_VISIBLE | WS_CHILD, 90, 180, 120, 40, hWnd, (HMENU)ID_BTN_QUERY, NULL, NULL);
        CreateWindow(L"STATIC", L"对实验中心的房间、账户、设备类别、设备的信息进行查询", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 240, 180, 480, 40, hWnd, NULL, NULL, NULL);

        CreateWindow(L"BUTTON", L"信息排序", WS_VISIBLE | WS_CHILD, 90, 240, 120, 40, hWnd, (HMENU)ID_BTN_SORT, NULL, NULL);
        CreateWindow(L"STATIC", L"对实验中心的房间、账户、设备类别、设备的信息进行排序", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 240, 240, 480, 40, hWnd, NULL, NULL, NULL);

        CreateWindow(L"BUTTON", L"信息统计", WS_VISIBLE | WS_CHILD, 90, 300, 120, 40, hWnd, (HMENU)ID_BTN_STATISTICS, NULL, NULL);
        CreateWindow(L"STATIC", L"对实验中心的房间、账户、设备类别、设备的信息进行统计", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 240, 300, 480, 40, hWnd, NULL, NULL, NULL);

        CreateWindow(L"BUTTON", L"系统维护", WS_VISIBLE | WS_CHILD, 90, 360, 120, 40, hWnd, (HMENU)ID_BTN_SYSTEM_MAINT, NULL, NULL);
        CreateWindow(L"STATIC", L"对密码进行维护、对其他用户的密码进行重置、数据备份、数据恢复", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 240, 360, 480, 40, hWnd, NULL, NULL, NULL);

        CreateWindow(L"BUTTON", L"退出", WS_VISIBLE | WS_CHILD, 90, 420, 120, 40, hWnd, (HMENU)ID_BTN_EXIT, NULL, NULL);
        CreateWindow(L"STATIC", L"退出当前账户", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 240, 420, 480, 40, hWnd, NULL, NULL, NULL);
        break;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam)) 
        {
        case ID_BTN_INFO_MANAGE:
            ShowWindow(hwndAdminManagement, SW_HIDE);
            ShowInfoManagementWindow(hWnd);
            break;
        case ID_BTN_QUERY:
            ShowQueryWindow(hWnd);
            break;
        case ID_BTN_SORT:
            //ShowSortWindow(hWnd);
            break;
        case ID_BTN_STATISTICS:
            //ShowStatisticsWindow(hWnd);
            break;
        case ID_BTN_SYSTEM_MAINT:
            ShowSystemMaintenanceWindow(hWnd);
            break;
        case ID_BTN_EXIT:
            em->current_account = NULL;
            ShowWindow(hwndAdminManagement, SW_HIDE);
            ShowLoginWindow(hWnd);
            
        }
        break;

    case WM_CLOSE:
        DestroyWindow(hWnd);
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void ShowLoginWindow(HWND hWnd)
{
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = LoginWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"LoginWindow";

    RegisterClass(&wc);

    hwndLogin = CreateWindow(L"LoginWindow", L"登录", WS_OVERLAPPED | WS_SYSMENU,
        400, 300, 400, 300, hWnd, NULL, GetModuleHandle(NULL), NULL);

    ShowWindow(hwndLogin, SW_SHOW);
    UpdateWindow(hwndLogin);
}

void ShowRegisterWindow(HWND hWnd)
{
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = RegisterWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"RegisterWindow";

    RegisterClass(&wc);

    hwndRegister = CreateWindow(L"RegisterWindow", L"注册", WS_OVERLAPPED | WS_SYSMENU,
        400, 300, 400, 300, hWnd, NULL, GetModuleHandle(NULL), NULL);

    ShowWindow(hwndRegister, SW_SHOW);
    UpdateWindow(hwndRegister);
}

void ShowAdminManagementWindow(HWND hWnd)
{
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = AdminManagementWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"AdminManagementWindow";

    RegisterClass(&wc);

    hwndAdminManagement = CreateWindow(L"AdminManagementWindow", L"实验设备管理系统", WS_OVERLAPPED | WS_SYSMENU,
        200, 100, 800, 600, hWnd, NULL, GetModuleHandle(NULL), NULL);

    ShowWindow(hwndAdminManagement, SW_SHOW);
    UpdateWindow(hwndAdminManagement);
}
