#include <windows.h>
#include "main_ui.h"
#include "equipment_management.h"

EquipmentManagement* em = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MainWindow";

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, L"窗口注册失败！", L"错误", MB_ICONERROR);
        return 1;
    }

    em = CreateEquipmentManagement();

    HWND hWnd = CreateWindow(L"MainWindow", L"实验设备管理系统", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        MessageBox(NULL, L"窗口创建失败！", L"错误", MB_ICONERROR);
        return 1;
    }

    ShowWindow(hWnd, SW_HIDE);
    UpdateWindow(hWnd);

    // 默认显示登录界面
    ShowLoginWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DestoryEquipmentManagement(em);

    return (int)msg.wParam;
}

// 处理窗口消息
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}