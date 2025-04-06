#pragma once
#ifndef ID_MANAGER_H
#define ID_MANAGER_H

#include "linked_list.h"
#include "configs.h"

typedef enum _IdType
{
    EquipmentID,
    CategoryID,
    RoomID,
    AccountID,
	ServiceID,
}IdType;

typedef struct _IdManager
{
    int current_equipment_id;   // �豸ID
    int current_category_id;    // �豸���ID
    int current_room_id;        // ����ID
    int current_account_id;     // �˻�ID
	int current_service_id;     // �豸�����¼ID
}IdManager;

// ��ȡһ����ID
int GetNewId(IdType type);

void DestoryIdManager();

// ����ID
bool SaveId();

// ��ȡID
bool LoadId();
#endif // !ID_MANAGER_H
