#pragma once
#include "configs.h"
#include "id_manager.h"
#include "resource_manager.h"
#include "account.h"
#include "experimental_equipment.h"
#include "equipment_management.h"
#include "linked_list.h"

//ά������
typedef enum _Service_type
{
	LostRegister,      // ��ʧ�Ǽ�
	DamagedRegister,   // �𻵵Ǽ�
	ServiceRegister,   // ά�޵Ǽ�
	ServiceFinish,     // ά�����
	ScrapRegister              // ����
} Service_type;
// ά�޼�¼�ṹ
typedef struct _Service
{
	int service_id;              // ��¼ID
	int equipment_id;           // �豸ID
	wchar_t equipment_name[EQUIPMENT_LENGTH]; // �豸��
	int user_id;                // ������ԱID
	wchar_t data[DATE_LENGTH];                  // ά��ʱ��
	Service_type type;          // ά������
	wchar_t reason[NOTE_LENGTH];           // ��ע
} Service;

Service* CCreateService(int equipment_id, char* equipment_name, int user_id, char* time, Service_type type, char* reason);
void DestoryService(Service* service);
void AddService(Account*);
void DDeleteService(Service* service);

void FillServiceList(HWND hListView);
