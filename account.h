#pragma once
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "configs.h"
#include "linked_list.h"
#include <Windows.h>

typedef enum _AccountType
{
	Admin,         // 管理员
	Experimenter,  // 实验员
	User,          // 一般用户
	Unknow = -1
}AccountType;

typedef struct _Account
{
	AccountType account_type;
	wchar_t user_name[USER_NMAE_LENGTH];
	wchar_t user_password[USER_PASSWORD_LENGTH];
	int id;  //账户ID
	int roomid;   //所属实验室id
	LinkedList* equipment_id_list;   //ta管理的设备的id链表
} Account;


Account* CreateAccount();

// 销毁账户
void DestoryAccount(Account* account);

// 判断账户是否有效
bool ValidAccount(Account* account);

// 判断用户名是否有效
bool IsValidUsername(wchar_t* username);

Account* FindByUsername(wchar_t* username);
Account* FindById(int id);

void _AddAccount(HWND hWnd);
void _ChangeAccount(HWND hWnd);
void _DeleteAccount(HWND hWnd);

void Change_roomid(Account* account, int id);

bool IsValidPassword(wchar_t* password);

bool IsCorrectAccount(Account* account);

// ....其他函数
#endif // !ACCOUNT
