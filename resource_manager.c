#include "resource_manager.h"
#include "account.h"
#include "category.h"
#include "experimental_equipment.h"
#include <string.h>
#include <windows.h>
#include <stdlib.h>

static ResourceManager* instance = NULL;

ResourceManager* GetResourceManage()
{
    if (instance == NULL)
    {
        instance = (ResourceManager*)malloc(sizeof(ResourceManager));
        if (instance == NULL)
        {
            printf("��Դ����������ʧ��");
            exit(-1);
        }
        instance->account_list = CreateLinkedList();
        instance->equipment_list = CreateLinkedList();
        instance->laboratory_list = CreateLinkedList();
		instance->category_list = CreateLinkedList();
    }

    return instance;
}

void _LoadResource()
{
	LoadLaboratoryList();
	LoadAccountList();
	LoadCategoryList();
	LoadEquipmentList();
}

void SaveResource()
{
	SaveAccountList();
	SaveCategoryList();
	SaveEquipmentList();
	SaveLaboratoryList();
}

void DestoryResourceManage()
{
    if (instance == NULL)
        return;

    destoryLinkedList(instance->account_list);
    destoryLinkedList(instance->equipment_list);
    destoryLinkedList(instance->laboratory_list);
}

bool LoadAccountList()
{
	FILE* fp = _wfopen(L"account.txt", L"rb");
	if (fp == NULL)
	{
		MessageBoxW(NULL, L"�˻��ļ���ʧ��", L"����", MB_ICONERROR);
		return False;
	}

	wchar_t bom;
	fread(&bom, sizeof(wchar_t), 1, fp);
	if (bom != 0xFEFF)
	{
		wprintf(L"������Ч��UTF-16�ļ���\n");
		fclose(fp);
		return False;
	}

	ResourceManager* resource_manager = GetResourceManage();
	wchar_t str[15] = L"";
	while (!feof(fp))
	{
		Account* account = CreateAccount();;
		if (fwscanf_s(fp, L"%d %14ls %255ls %255ls %d",
			&account->id,
			str, (unsigned)_countof(str),
			account->user_name, (unsigned)_countof(account->user_name),
			account->user_password, (unsigned)_countof(account->user_password),
			&account->roomid) != 5)
		{
			free(account);
			continue; // ������ʽ�������
		}

		if (wcscmp(str, L"Admin") == 0) account->account_type = Admin;
		else if (wcscmp(str, L"Experimenter") == 0) account->account_type = Experimenter;
		else if (wcscmp(str, L"User") == 0) account->account_type = User;

		if (account->roomid != -1)
		{
			LabRoom* lab_room = RoomId_to_LabRoom(account->roomid);
			LinkedList_pushback(lab_room->technician_id_list, &(account->roomid));
		}

		LinkedList_pushback(resource_manager->account_list, account);
	}   
	fclose(fp);
	return True;
}

bool SaveAccountList()
{
	FILE* fp = _wfopen(L"account.txt", L"wb");
	if (!fp)
	{
		MessageBoxW(NULL, L"�˻��ļ�����ʧ��", L"����", MB_ICONERROR);
		return False;
	}

	wchar_t bom = 0xFEFF;
	fwrite(&bom, sizeof(wchar_t), 1, fp);

	Node* temp = GetResourceManage()->account_list->head->next;
	while (temp)
	{
		Account* account = (Account*)temp->data;
		const wchar_t* typeStr = L"";
		switch (account->account_type) 
		{
			case Admin: 
				typeStr = L"Admin"; 
				break;
			case Experimenter: 
				typeStr = L"Experimenter"; 
				break;
			case User: 
				typeStr = L"User";
				break;
		}
		fwprintf(fp, L"%d %s %s %s %d\n",
			account->id,
			typeStr,
			account->user_name,
			account->user_password,
			account->roomid);
		temp = temp->next;
	}
	fclose(fp);
	return True;
}

bool LoadEquipmentList()
{
	FILE* fp = _wfopen(L"equipment.txt", L"rb");
	if (fp == NULL) 
	{
		printf("���ļ�ʧ��\n");
		return False;
	}
	ResourceManager* resource_manager = GetResourceManage();
	int category_id = 0;

	wchar_t bom;
	fread(&bom, sizeof(wchar_t), 1, fp);
	if (bom != 0xFEFF)
	{
		wprintf(L"������Ч��UTF-16�ļ���\n");
		fclose(fp);
		return False;
	}

	//�˴��������feof��ȡ���ļ�����ǳ�Σ�գ��������Ƶĵط�Ҳ��
	while (!feof(fp))
	{
		ExperimentalEquipment* eq = (ExperimentalEquipment*)malloc(sizeof(ExperimentalEquipment));
		if (eq == NULL)
		{
			printf("�ڴ����ʧ��\n");
			fclose(fp);
			return False;
		}
		fwscanf_s(fp, L"%d %d ", &eq->id, &category_id);
		fwscanf_s(fp, L"%ls ", eq->name, EQUIPMENT_LENGTH);
		fwscanf_s(fp, L"%d %d ", &eq->room_id, &eq->price);
		fwscanf_s(fp, L"%ls\n", eq->purchase_date, DATE_LENGTH);
		eq->name[wcslen(eq->name)] = L'\0';
		eq->purchase_date[wcslen(eq->purchase_date)] = L'\0';
		eq->category = FindCategoryById(category_id);

		LabRoom* lab_room = RoomId_to_LabRoom(eq->room_id);
		LinkedList_pushback(lab_room->equipments_list, &(eq->id));

		LinkedList_pushback(resource_manager->equipment_list, eq);
	}
	fclose(fp);
	return False;
}

bool SaveEquipmentList()
{
	FILE* fp = _wfopen(L"equipment.txt", L"wb");
	if (fp == NULL)
	{
		printf("�ļ���ʧ��\n");
		return False;
	}

	wchar_t bom = 0xFEFF;
	fwrite(&bom, sizeof(wchar_t), 1, fp);

	Node* temp = GetResourceManage()->equipment_list->head->next;
	while (temp)
	{
		ExperimentalEquipment* eq = (ExperimentalEquipment*)temp->data;
		fwprintf(fp, L"%d %d %ls %d %d %ls\n",
			eq->id, eq->category->id, eq->name,
			eq->room_id, eq->price, eq->purchase_date);
		temp = temp->next;
	}
	fclose(fp);
	return False;
}

bool LoadCategoryList()
{
	FILE* fp = _wfopen(L"category.txt", L"rb");
	if (fp == NULL)
	{
		perror("�ļ���ʧ��\n");
		return False;
	}
	wchar_t bom;
	fread(&bom, sizeof(wchar_t), 1, fp);
	if (bom != 0xFEFF)
	{
		wprintf(L"������Ч��UTF-16�ļ���\n");
		fclose(fp);
		return False;
	}

	ResourceManager* resource_manager = GetResourceManage();
	while (!feof(fp))
	{
		Category* category = (Category*)malloc(sizeof(Category));
		if (category == NULL)
		{
			printf("�ڴ����ʧ��\n");
			fclose(fp);
			return False;
		}
		fwscanf_s(fp, L"%d %ls %d\n", &category->id, category->name, CATEGORY_LENGTH, &category->disposal_years);
		category->name[wcslen(category->name)] = L'\0';
		LinkedList_pushback(resource_manager->category_list, category);
	}
	fclose(fp);
	return False;
}

bool SaveCategoryList()
{
	FILE* fp = _wfopen(L"category.txt", L"wb");
	if (fp == NULL)
	{
		printf("�ļ���ʧ��\n");
		return False;
	}

	wchar_t bom = 0xFEFF;
	fwrite(&bom, sizeof(wchar_t), 1, fp);
	
	Node* temp = GetResourceManage()->category_list->head->next;
	while (temp)
	{
		Category* category = (Category*)temp->data;
		fwprintf(fp, L"%d %ls %d\n", category->id, category->name, category->disposal_years);
		temp = temp->next;
	}
	fclose(fp);
	return False;
}

bool LoadLaboratoryList()
{
	FILE* fp = _wfopen(L"laboratory.txt", L"rb");
	if (fp == NULL)
	{
		printf("�ļ���ʧ��\n");
		return False;
	}

	wchar_t bom;
	fread(&bom, sizeof(wchar_t), 1, fp);
	if (bom != 0xFEFF)
	{
		wprintf(L"������Ч��UTF-16�ļ���\n");
		fclose(fp);
		return False;
	}
	ResourceManager* resource_manager = GetResourceManage();
	while (!feof(fp))
	{
		LabRoom* laboratory = (LabRoom*)malloc(sizeof(LabRoom));
		if (laboratory == NULL)
		{
			printf("�ڴ����ʧ��\n");
			fclose(fp);
			return False;
		}

		fwscanf_s(fp, L"%d %ls\n", &laboratory->id, laboratory->name, CATEGORY_LENGTH);

		laboratory->equipments_list = CreateLinkedList();
		laboratory->technician_id_list = CreateLinkedList();

		LinkedList_pushback(resource_manager->laboratory_list, laboratory);
	}
	fclose(fp);
	return True;
}

bool SaveLaboratoryList()
{
	FILE* fp = _wfopen(L"laboratory.txt", L"wb");
	if (fp == NULL)
	{
		printf("�ļ���ʧ��\n");
		return False;
	}

	wchar_t bom = 0xFEFF;
	fwrite(&bom, sizeof(wchar_t), 1, fp);

	Node* temp = GetResourceManage()->laboratory_list->head->next;
	
	while (temp)
	{
		LabRoom* laboratory = (LabRoom*)temp->data;
		fwprintf(fp, L"%d %ls\n", laboratory->id, laboratory->name);
		temp = temp->next;
	}
	return True;
}
