#ifndef EXPERIMENTAL_EQUIPMENT_H
#define EXPERIMENTAL_EQUIPMENT_H

#include "configs.h"
#include "category.h"
#include "lab_room.h"
#include "resource_manager.h"
#include <Windows.h>


typedef enum _EquipmentState
{
	Using,     // 正在使用
	Idle,      // 空闲
	Lost,      // 遗失
	Damaged,   // 损坏（未在维修）
	Scrapped,  // 报废
	Repairing,  // 正在维修
	None
}EquipmentState;

typedef struct _ExperimentalEquipment
{
	int id;                                   // 设备id
	Category* category;                        // 设备类别
	wchar_t name[EQUIPMENT_LENGTH];              // 设备名字
	int room_id;                              // 所在实验室ID
	int price;                                // 设备价格
	wchar_t purchase_date[DATE_LENGTH];          // 购入日期 (YYYY-MM-DD)
	EquipmentState state;                          // 设备状态
}ExperimentalEquipment;

void state_to_string(EquipmentState state, wchar_t* string);

ExperimentalEquipment* CreateExperimentalEquipment(Category* category, wchar_t* name, int room_id, int price, wchar_t* purchase_date);

void DestoryExperimentalEquipment(ExperimentalEquipment* experimental_equipment);

void AddExperimentalEquipment();

// 从界面获取数据处理设备
void _AddExperimentalEquipment(HWND hWnd);
void _DeleteExperimentalEquipment(HWND hWnd);
void _ChangeExperimentalEquipment(HWND hWnd);

bool ChangeName(ExperimentalEquipment* eq, wchar_t* newname);
bool ChangePrice(ExperimentalEquipment* eq, int newprice);
bool ChangeRoom_id(ExperimentalEquipment* eq, int newroomid);
bool ChangePurchaseDate(ExperimentalEquipment* eq, wchar_t* newdate);
bool ChangeExperimentalCategory(ExperimentalEquipment* eq, Category* newcategory);

LinkedList* EFindByName(LinkedList* eqlist, wchar_t* name);
LinkedList* EFindById(LinkedList* eqlist, int id);
LinkedList* EFindByRoom_id(LinkedList* eqlist, int roomid);
LinkedList* EFindByCategory(LinkedList* eqlist, int categoryid);
LinkedList* EFindByDate(LinkedList* eqlist, char* start, char* end);
LinkedList* EFindByPrice(LinkedList* eqlist, int min, int max);

ExperimentalEquipment* id_to_equipment(int id);
#endif // !EXPERIMENTAL_EQUIPMENT_H
