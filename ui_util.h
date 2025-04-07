#pragma once

#include <Windows.h>
#include <corecrt_wstdio.h>
#include "configs.h"

int GetInputNumber(HWND hwnd, int editControlID)
{
    wchar_t text[256];
    GetWindowText(GetDlgItem(hwnd, editControlID), text, 256);

    // 将文本转换为整数
    int number = _wtoi(text);

    return number;
}

void ShowNumber(HWND hwnd, int number)
{
    wchar_t buffer[256];
    swprintf(buffer, 256, L"数字: %d", number);

    MessageBox(hwnd, buffer, L"提示", MB_OK);
}

bool IsEditTextInteger(HWND hEdit)
{
    wchar_t buffer[100];
    GetWindowText(hEdit, buffer, 100);

    if (wcslen(buffer) == 0)
        return False;  

    wchar_t* endPtr = NULL;
    wcstol(buffer, &endPtr, 10); 

    if (endPtr == buffer || *endPtr != L'\0')
        return False;

    return True;
}