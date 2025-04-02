#pragma once
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "configs.h"
#include "linked_list.h"

typedef enum _AccountType
{
	Admin,         // ����Ա
	Experimenter,  // ʵ��Ա
	User,          // һ���û�
	Unknow = -1
}AccountType;

typedef struct _Account
{
	AccountType account_type;
	wchar_t user_name[USER_NMAE_LENGTH];
	wchar_t user_password[USER_PASSWORD_LENGTH];
	int id;  //�˻�ID
	int roomid;   //����ʵ����id
	LinkedList* equipment_id_list;   //ta������豸��id����
} Account;


Account* CreateAccount();

// �����˻�
void DestoryAccount(Account* account);

// �ж��˻��Ƿ���Ч
bool ValidAccount(Account* account);

// �ж��û����Ƿ���Ч
bool IsValidUsername(wchar_t* username);

Account* FindByUsername(wchar_t* username);
Account* FindById(int id);


bool IsValidPassword(wchar_t* password);

bool IsCorrectAccount(Account* account);

// ....��������
#endif // !ACCOUNT
