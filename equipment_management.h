#pragma once
#ifndef EQUIPMENT_MANAGEMENT
#define EQUIPMENT_MANAGEMENT

#include <stdio.h>

#include "configs.h"
#include "id_manager.h"
#include "account.h"


typedef enum _CurrentState
{
	Login,       // ��¼����
	Register,    // ע�����
	Management	 // �������
}CurrentState;

typedef struct _EquipmentManagement
{
	bool running;    // �Ƿ�����
	CurrentState current_state;
	Account* current_account;

} EquipmentManagement;

void Run(EquipmentManagement* em);

EquipmentManagement* CreateEquipmentManagement();

void DestoryEquipmentManagement(EquipmentManagement* em);

#endif // EQUIPMENT_MANAGEMENT