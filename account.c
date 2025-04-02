#include <ctype.h>
#include <string.h>
#include "resource_manager.h"
#include "account.h"

Account* CreateAccount()
{
	Account* new_account = (Account*)malloc(sizeof(Account));
	if (new_account == NULL)
	{
		printf("�˻�����ʧ�ܣ�\n");
		return NULL;
	}
	return new_account;
}

void DestoryAccount(Account* account)
{
	free(account);
}

bool ValidAccount(Account* account)
{
	if (!IsValidUsername(account->user_name))
	{
		printf("�û������Ϸ�\n");
		return False;
	}
	if (FindByUsername(account->user_name))
	{
		printf("�û����Ѵ���\n");
		return False;
	}
	if (!IsValidPassword(account->user_password))
	{
		printf("���벻�Ϸ�\n");
		return False;
	}
	return True;
}

bool IsValidUsername(wchar_t* username)
{
	int count = 0;
	while (*username)
	{
		if (!iswalpha(*username) && !iswdigit(*username))
			return False;
		count++;
		if (count > 12)
			return False;
		username++;
	}
	if (count < 4)
		return False;
	return 1;
}

Account* FindByUsername(wchar_t* username)
{
	ResourceManager* resource_manager = GetResourceManage();

	Node* temp = resource_manager->account_list->head->next;
	while (temp)
	{
		Account* account = (Account*)temp->data;
		if (wcscmp(account->user_name, username) == 0)
			return account;
		temp = temp->next;
	}	
	return NULL;
}
Account* FindById(int id)
{
	ResourceManager* resource_manager = GetResourceManage();
	Node* temp = resource_manager->account_list->head->next;
	while (temp)
	{
		Account* account = (Account*)temp->data;
		if (account->id == id)
			return account;
		temp = temp->next;
	}
	return NULL;
}

bool IsValidPassword(wchar_t* password)
{
	int count = 0;
	while (*password)
	{
		if (!(iswalnum(*password) || *password == L'@' || *password == L'+' || *password == L'?'))
			return False;
		count++;
		if (count > 20)
			return False;
		password++;
	}
	if (count <= 8)
		return False;
	return True;
}

bool IsCorrectAccount(Account* account)
{
	Account* temp = FindByUsername(account->user_name);
	if (temp == NULL)
	{
		printf("�û���������\n");
		return False;
	}
	if (wcscmp(temp->user_password, account->user_password) != 0)
	{
		printf("�������\n");
		return False;
	}

	account->account_type = temp->account_type;
	return True;
}

