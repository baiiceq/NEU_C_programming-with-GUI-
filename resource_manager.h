#pragma once
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "linked_list.h"
#include "configs.h"

typedef struct _ResourceManager
{
	LinkedList* account_list;      // 账户
	LinkedList* equipment_list;    // 实验设备
	LinkedList* category_list;     // 实验设备类型
	LinkedList* laboratory_list;   // 实验室
	LinkedList* service_list;      // 设备管理记录
}ResourceManager;


ResourceManager* GetResourceManage();

void _LoadResource();
void SaveResource();

bool LoadAccountList(wchar_t*);
bool SaveAccountList(wchar_t*);


bool LoadEquipmentList(wchar_t*);
bool SaveEquipmentList(wchar_t*);


bool LoadCategoryList(wchar_t*);
bool SaveCategoryList(wchar_t*);


bool LoadLaboratoryList(wchar_t*);
bool SaveLaboratoryList(wchar_t*);

bool LoadServiceList(wchar_t*);
bool SaveServiceList(wchar_t*);

bool SaveBackUp(char* time, char* formattedTime);

void DestoryResourceManage();

#endif // !RESOURCE_MANAGER_H