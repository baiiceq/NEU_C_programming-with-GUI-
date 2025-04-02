#pragma once
#ifndef LOGIN_INTERFACE
#define LOGIN_INTERFACE

#include "configs.h"
#include "linked_list.h"
#include "equipment_management.h"

int ShowLoginMenu();

void RunLoginInterface(int option, EquipmentManagement* em);

// ��¼���������������˺ź����룬����˺źϷ��ԵȲ�����, �����˺�
Account* AccountLogin();

#endif // !LOGIN_INTERFACE