#pragma once
#include "configs.h"
#include "id_manager.h"
#include "resource_manager.h"
#include "account.h"
#include "experimental_equipment.h"
#include "equipment_management.h"
#include "linked_list.h"

//操作类型
typedef enum _Service_type
{
	LostRegister,      // 遗失登记
	DamagedRegister,   // 损坏登记
	ServiceRegister,   // 维修登记
	ServiceFinish,     // 维修完成
	ScrapRegister,      // 报废
	UsingRegister,      //使用登记
	UsingFinish,       //使用完成
} Service_type;
// 设备管理记录结构
typedef struct _Service
{
	int service_id;              // 记录ID
	int equipment_id;           // 设备ID
	wchar_t equipment_name[EQUIPMENT_LENGTH]; // 设备名
	int user_id;                // 操作人员ID
	wchar_t data[DATE_LENGTH];                  // 操作时间
	Service_type type;          // 操作类型
	wchar_t reason[NOTE_LENGTH];           // 备注
} Service;

#define IDC_BUTTON_ADD_SERVICE 1001
#define IDC_BUTTON_FIND_SERVICE 1002

Service* CCreateService(int equipment_id, char* equipment_name, int user_id, char* time, Service_type type, char* reason);
void DestoryService(Service* service);
void AddService(Account*);
void DDeleteService(Service* service);

void FillServiceList(HWND hListView);
