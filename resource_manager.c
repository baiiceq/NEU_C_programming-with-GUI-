#include "resource_manager.h"
#include "account.h"
#include "category.h"
#include "service.h"
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
            printf("资源管理器加载失败");
            exit(-1);
        }
        instance->account_list = CreateLinkedList();
        instance->equipment_list = CreateLinkedList();
        instance->laboratory_list = CreateLinkedList();
		instance->category_list = CreateLinkedList();
		instance->service_list = CreateLinkedList();
    }

    return instance;
}

void _LoadResource()
{
	wchar_t path[100];
	wcscpy_s(path,100 , L"laboratory.txt");
	LoadLaboratoryList(path);
	wcscpy_s(path, 100, L"account.txt");
	LoadAccountList(path);
	wcscpy_s(path, 100, L"category.txt");
	LoadCategoryList(path);
	wcscpy_s(path, 100, L"equipment.txt");
	LoadEquipmentList(path);
	wcscpy_s(path, 100, L"service.txt");
	LoadServiceList(path);
}

void SaveResource()
{
	wchar_t path[100];
	wcscpy_s(path, 100, L"account.txt");
	SaveAccountList(path);
	wcscpy_s(path, 100, L"category.txt");
	SaveCategoryList(path);
	wcscpy_s(path, 100, L"equipment.txt");
	SaveEquipmentList(path);
	wcscpy_s(path, 100, L"laboratory.txt");
	SaveLaboratoryList(path);
	wcscpy_s(path, 100, L"service.txt");
	SaveServiceList(path);
}

void DestoryResourceManage()
{
	if (instance == NULL)
		return;

	destoryLinkedList(instance->account_list);
	destoryLinkedList(instance->equipment_list);
	destoryLinkedList(instance->laboratory_list);
	destoryLinkedList(instance->category_list);
	destoryLinkedList(instance->service_list);
	instance = NULL;
}

bool LoadAccountList(wchar_t* path)
{
	FILE* fp = _wfopen(path, L"rb");
	if (fp == NULL)
	{
		MessageBoxW(NULL, L"账户文件打开失败", L"错误", MB_ICONERROR);
		return False;
	}

	wchar_t bom;
	fread(&bom, sizeof(wchar_t), 1, fp);
	if (bom != 0xFEFF)
	{
		wprintf(L"不是有效的UTF-16文件！\n");
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
			continue; // 跳过格式错误的行
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

bool SaveAccountList(wchar_t* path)
{
	FILE* fp = _wfopen(path, L"wb");
	if (!fp)
	{
		MessageBoxW(NULL, L"账户文件保存失败", L"错误", MB_ICONERROR);
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

bool LoadEquipmentList(wchar_t* path)
{
	FILE* fp = _wfopen(path, L"rb");
	if (fp == NULL) 
	{
		printf("打开文件失败\n");
		return False;
	}
	ResourceManager* resource_manager = GetResourceManage();
	int category_id = 0;

	wchar_t bom;
	fread(&bom, sizeof(wchar_t), 1, fp);
	if (bom != 0xFEFF)
	{
		wprintf(L"不是有效的UTF-16文件！\n");
		fclose(fp);
		return False;
	}

	//此处如果是用feof读取空文件将会非常危险，后面相似的地方也是
	while (!feof(fp))
	{
		ExperimentalEquipment* eq = (ExperimentalEquipment*)malloc(sizeof(ExperimentalEquipment));
		if (eq == NULL)
		{
			printf("内存分配失败\n");
			fclose(fp);
			return False;
		}
		fwscanf_s(fp, L"%d %d ", &eq->id, &category_id);
		fwscanf_s(fp, L"%ls ", eq->name, EQUIPMENT_LENGTH);
		fwscanf_s(fp, L"%d %d ", &eq->room_id, &eq->price);
		fwscanf_s(fp, L"%ls ", eq->purchase_date, DATE_LENGTH);
		wchar_t state[50];
		fwscanf_s(fp, L"%ls\n", state, 50);
		if (wcscmp(state, L"Using") == 0) eq->state = Using;
		else if (wcscmp(state, L"Idle") == 0) eq->state = Idle;
		else if (wcscmp(state, L"Lost") == 0) eq->state = Lost;
		else if (wcscmp(state, L"Damaged") == 0) eq->state = Damaged;
		else if (wcscmp(state, L"Scrapped") == 0) eq->state = Scrapped;
		else eq->state = Repairing;
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

bool SaveEquipmentList(wchar_t* path)
{
	FILE* fp = _wfopen(path, L"wb");
	if (fp == NULL)
	{
		printf("文件打开失败\n");
		return False;
	}

	wchar_t bom = 0xFEFF;
	fwrite(&bom, sizeof(wchar_t), 1, fp);

	Node* temp = GetResourceManage()->equipment_list->head->next;
	while (temp)
	{
		ExperimentalEquipment* eq = (ExperimentalEquipment*)temp->data;
		fwprintf(fp, L"%d %d %ls %d %d %ls ",
			eq->id, eq->category->id, eq->name,
			eq->room_id, eq->price, eq->purchase_date);
		switch (eq->state)
		{
		case Using: fwprintf(fp, L"Using\n"); break;
		case Idle: fwprintf(fp, L"Idle\n"); break;
		case Lost: fwprintf(fp, L"Lost\n"); break;
		case Damaged: fwprintf(fp, L"Damaged\n"); break;
		case Scrapped: fwprintf(fp, L"Scrapped\n"); break;
		default: fwprintf(fp, L"Repairing\n"); break;
		}
		temp = temp->next;
	}
	fclose(fp);
	return False;
}

bool LoadCategoryList(wchar_t* path)
{
	FILE* fp = _wfopen(path, L"rb");
	if (fp == NULL)
	{
		perror("文件打开失败\n");
		return False;
	}
	wchar_t bom;
	fread(&bom, sizeof(wchar_t), 1, fp);
	if (bom != 0xFEFF)
	{
		wprintf(L"不是有效的UTF-16文件！\n");
		fclose(fp);
		return False;
	}

	ResourceManager* resource_manager = GetResourceManage();
	while (!feof(fp))
	{
		Category* category = (Category*)malloc(sizeof(Category));
		if (category == NULL)
		{
			printf("内存分配失败\n");
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

bool SaveCategoryList(wchar_t* path)
{
	FILE* fp = _wfopen(path, L"wb");
	if (fp == NULL)
	{
		printf("文件打开失败\n");
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

bool LoadLaboratoryList(wchar_t* path)
{
	FILE* fp = _wfopen(path, L"rb");
	if (fp == NULL)
	{
		printf("文件打开失败\n");
		return False;
	}

	wchar_t bom;
	fread(&bom, sizeof(wchar_t), 1, fp);
	if (bom != 0xFEFF)
	{
		wprintf(L"不是有效的UTF-16文件！\n");
		fclose(fp);
		return False;
	}
	ResourceManager* resource_manager = GetResourceManage();
	while (!feof(fp))
	{
		LabRoom* laboratory = (LabRoom*)malloc(sizeof(LabRoom));
		if (laboratory == NULL)
		{
			printf("内存分配失败\n");
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

bool SaveLaboratoryList(wchar_t* path)
{
	FILE* fp = _wfopen(path, L"wb");
	if (fp == NULL)
	{
		printf("文件打开失败\n");
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

bool LoadServiceList(wchar_t* path)
{
	FILE* fp = _wfopen(path, L"rb");
	if (fp == NULL)
	{
		printf("文件打开失败\n");
		return False;
	}
	wchar_t bom;
	fread(&bom, sizeof(wchar_t), 1, fp);
	if (bom != 0xFEFF)
	{
		wprintf(L"不是有效的UTF-16文件！\n");
		fclose(fp);
		return False;
	}
	ResourceManager* resource_manager = GetResourceManage();
	while (!feof(fp))
	{
		Service* service = (Service*)malloc(sizeof(Service));
		if (service == NULL)
		{
			printf("内存分配失败\n");
			fclose(fp);
			return False;
		}
		fwscanf_s(fp, L"%d %d %ls %d %ls ",
			&service->service_id,
			&service->equipment_id,
			service->equipment_name, EQUIPMENT_LENGTH,
			&service->user_id,
			service->data, DATE_LENGTH);

		wchar_t type[50], reason[NOTE_LENGTH];
		fwscanf_s(fp, L"%ls %ls\n", type, 50, service->reason, NOTE_LENGTH);

		if (wcscmp(type, L"LostRegister") == 0) service->type = LostRegister;
		else if (wcscmp(type, L"DamagedRegister") == 0) service->type = DamagedRegister;
		else if (wcscmp(type, L"ServiceRegister") == 0) service->type = ServiceRegister;
		else if (wcscmp(type, L"ServiceFinish") == 0) service->type = ServiceFinish;
		else if (wcscmp(type,L"ScrapRegister")==0)service->type = ScrapRegister;
		else if (wcscmp(type, L"UsingRegister") == 0)service->type = UsingRegister;
		else service->type = UsingFinish;

		LinkedList_pushback(resource_manager->service_list, service);
	}
	fclose(fp);
	return True;
}

bool SaveServiceList(wchar_t* path)
{
	FILE* fp = _wfopen(path, L"wb");
	if (fp == NULL)
	{
		printf("文件打开失败\n");
		return False;
	}
	wchar_t bom = 0xFEFF;
	fwrite(&bom, sizeof(wchar_t), 1, fp);
	Node* temp = GetResourceManage()->service_list->head->next;
	while (temp)
	{
		wchar_t wstr[100]=L"";
		Service* service = (Service*)temp->data;
		fwprintf(fp, L"%d %d %ls %d %ls ",
			service->service_id,
			service->equipment_id,
			service->equipment_name,
			service->user_id,
			service->data);
		switch (service->type)
		{
		case LostRegister: fwprintf(fp, L"LostRegister "); break;
		case DamagedRegister: fwprintf(fp, L"DamagedRegister "); break;
		case ServiceRegister: fwprintf(fp, L"ServiceRegister "); break;
		case ServiceFinish: fwprintf(fp, L"ServiceFinish "); break;
		case ScrapRegister: fwprintf(fp, L"ScrapRegister "); break;
		case UsingRegister: fwprintf(fp, L"UsingRegister "); break;
		default: fwprintf(fp, L"UsingFinish "); break;
		}
		fwprintf(fp, L"%ls\n", service->reason);
		temp = temp->next;
	}
	fclose(fp);
	return True;
}

//备份文件
bool SaveBackUp(char* time, char* formattedTime)
{
	char str[100];
	snprintf(str, 100, "backups/%s/readme.txt", formattedTime);
	FILE* fp = fopen(str, "w");
	if (fp == NULL)
	{
		printf("文件打开失败\n");
		return False;
	}
	fprintf(fp, "=== 系统备份信息 ===\n");
	fprintf(fp, "备份时间%s\n", time);
	ResourceManager* rm = GetResourceManage();
	fprintf(fp, "账户数据备份共 %zu 条记录\n", rm->account_list->size);
	fprintf(fp, "设备数据备份共 %zu 条记录\n", rm->equipment_list->size);
	fprintf(fp, "设备类别数据备份共 %zu 条记录\n", rm->category_list->size);
	fprintf(fp, "实验室数据备份共 %zu 条记录\n", rm->laboratory_list->size);
	fclose(fp);
}
