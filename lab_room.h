#pragma once
#ifndef LAB_ROOM_H
#define LAB_ROOM_H

#include "configs.h"
#include "linked_list.h"
#include "experimental_equipment.h"
#include "account.h"
#include "function_ui.h"

typedef struct _LabRoom {
    int id;                            // 房间编号
    wchar_t name[LABROOM_LENGTH];         // 房间名称
    LinkedList* technician_id_list;     // 负责实验员ID链
    LinkedList* equipments_list;        // 所属设备id链
} LabRoom;

LabRoom* CreateLabRoom(wchar_t* name);

void DestoryLabRoom(LabRoom* lab_room);

void AddLabRoom();

void _AddLabRoom(HWND hWnd);
void _DeleteLabRoom(HWND hWnd);
void _ChangeLabRoom(HWND hWnd);

bool ChangeLabRoom();
bool DeleteLabRoom();
bool ChangeLabName(LabRoom* lab_room, wchar_t* newname);
bool AddEquipment(LabRoom* lab_room, int eqid);
bool DeleteEquipment(LabRoom* lab_room, int eqid);
bool DeleteTechnician(LabRoom* lab_room, int techid);
bool AddTechnician(LabRoom* lab_room, int techid);
LabRoom* RoomId_to_LabRoom(int room_id);

#endif // !LAB_ROOM_H
