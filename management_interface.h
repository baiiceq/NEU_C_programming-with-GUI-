#pragma once
#ifndef MANAGEMENT_INTERFACE
#define MANAGEMENT_INTERFACE

#include "linked_list.h"
#include "equipment_management.h"
#include "id_manager.h"
#include "info_operations.h"


int ShowManagementMenu(AccountType account_type);

void RunManagementInterface(int option, EquipmentManagement* em, AccountType account_type);

// ��Ϣ��ѯ����
void QueryInformation();

// ��Ϣ������
void SortInformation();

// ��Ϣͳ��
void Statistics();

// ϵͳά��
void SystemMaintenance(AccountType account_type);

// ������Ϣά��
void BasicInformationManagement();

// ��Ϣ��������
void AdjustmentInformation();

// �豸����
void EquipmentsManagement();
#endif // !MANAGEMENT_INTERFACE
