#pragma once
#include <Windows.h>
#include "linked_list.h"
#include "category.h"
#include "experimental_equipment.h"
#include "lab_room.h"
#include "account.h"

#define IDC_TAB 1001
#define IDC_BUTTON_ADD 2001
#define IDC_BUTTON_DELETE 2002
#define IDC_BUTTON_MODIFY 2003
#define IDC_EDIT_NAME 2004
#define IDC_EDIT_PRICE 2005
#define IDC_EDIT_ROOM_ID 2006
#define IDC_DATEPICKER_DATE 2007
#define IDC_COMBOX_CATEGORY 2008
#define IDC_EDIT_NAME_CHANGE 2009
#define IDC_EDIT_PRICE_CHANGE 2010
#define IDC_EDIT_ROOM_ID_CHANGE 2011
#define IDC_DATEPICKER_DATE_CHANGE 2012
#define IDC_COMBOX_CATEGORY_CHANGE 2013
#define IDC_EDIT_PASSWORD 2014
#define IDC_EDIT_PASSWORD_CHANGE 2015
#define IDC_CHECKBOX_1 2014
#define IDC_CHECKBOX_2 2015
#define IDC_STATIC_1 10001
#define IDC_STATIC_2 10002
#define IDC_STATIC_3 10003
#define IDC_STATIC_4 10004
#define IDC_STATIC_5 10005
#define IDC_STATIC_6 10006
#define IDC_LISTVIEW 100007

void ShowInfoManagementWindow(HWND hWnd);

void ShowAdjustManagementWindow(HWND hWnd);

void ShowQueryWindow(HWND hWnd);

void ShowSortWindow(HWND hWnd);

void ShowStatisticsWindow(HWND hWnd);


