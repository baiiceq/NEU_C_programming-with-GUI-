#pragma once
#include "query.h"

typedef struct _SortAccount
{
    int sortById;    
    int sortByType; 
    int sortByUsername;     
} SortAccount;

int CompareAccount(const void* a, const void* b, void* arg)
{
    Account* accountA = (Account*)a;
    Account* accountB = (Account*)b;

    SortAccount* sort_account = (SortAccount*)arg;

    if (sort_account->sortById != 0)
    {
        int cmp = accountA->id - accountB->id;
        if (cmp != 0) return sort_account->sortById == 1 ? cmp : -cmp;
    }

    if (sort_account->sortByType != 0)
    {
        int cmp = accountA->account_type - accountB->account_type;
        if (cmp != 0) return sort_account->sortByType == 1 ? -cmp : cmp;
    }

    if (sort_account->sortByUsername != 0)
    {
        int cmp = strcmp(accountA->user_name, accountB->user_name);
        if (cmp != 0) return sort_account->sortByUsername == 1 ? cmp : -cmp;
    }

    return 0;
}

void SortAccountMenu()
{
    SortAccount sort_account;
    system("cls");
    printf("---        �˻���������        ---\n\n");

    while (getchar() != '\n');

    printf("--- �����˻�ID����ʽ��1 ����-1 ���򣬻س�������-> ");
    char input[10];
    fgets(input, sizeof(input), stdin);
    sort_account.sortById = (strlen(input) > 1) ? atoi(input) : 0;

    printf("--- �����˻���������ʽ��1 ����-1 ���򣬻س�������-> ");
    fgets(input, sizeof(input), stdin);
    sort_account.sortByType = (strlen(input) > 1) ? atoi(input) : 0;

    printf("--- �����û�������ʽ��1 ����-1 ���򣬻س�������-> ");
    fgets(input, sizeof(input), stdin);
    sort_account.sortByUsername = (strlen(input) > 1) ? atoi(input) : 0;

    LinkedList* list = GetResourceManage()->account_list;

    LinkedList_sort(list, CompareAccount, &sort_account);

    LinkedList_print(list, PrintAccount);
}

typedef struct _SortCategory
{
    int sortById;
    int sortByDisposalYears;
    int sortByName;
} SortCategory;

int CompareCategory(const void* a, const void* b, void* arg)
{
    Category* categoryA = (Category*)a;
    Category* categoryB = (Category*)b;

    SortCategory* sort_category = (SortCategory*)arg;

    if (sort_category->sortById != 0)
    {
        int cmp = categoryA->id - categoryB->id;
        if (cmp != 0) return sort_category->sortById == 1 ? cmp : -cmp;
    }

    if (sort_category->sortByDisposalYears != 0)
    {
        int cmp = categoryA->disposal_years - categoryB->disposal_years;
        if (cmp != 0) return sort_category->sortByDisposalYears == 1 ? cmp : -cmp;
    }

    if (sort_category->sortByName != 0)
    {
        int cmp = strcmp(categoryA->name, categoryB->name);
        if (cmp != 0) return sort_category->sortByName == 1 ? cmp : -cmp;
    }

    return 0;
}

void SortCategoryMenu()
{
    SortCategory sort_category;
    system("cls");
    printf("---        �豸������������        ---\n\n");

    while (getchar() != '\n');

    printf("--- �����豸����ID����ʽ��1 ����-1 ���򣬻س�������-> ");
    char input[10];
    fgets(input, sizeof(input), stdin);
    sort_category.sortById = (strlen(input) > 1) ? atoi(input) : 0;

    printf("--- �����豸���ͱ�����������ʽ��1 ����-1 ���򣬻س�������-> ");
    fgets(input, sizeof(input), stdin);
    sort_category.sortByDisposalYears = (strlen(input) > 1) ? atoi(input) : 0;

    printf("--- �����豸������������ʽ��1 ����-1 ���򣬻س�������-> ");
    fgets(input, sizeof(input), stdin);
    sort_category.sortByName = (strlen(input) > 1) ? atoi(input) : 0;

    LinkedList* list = GetResourceManage()->category_list;

    LinkedList_sort(list, CompareCategory, &sort_category);

    LinkedList_print(list, PrintCategory);

    sort_category.sortByDisposalYears = 0;
    sort_category.sortById = 1;
    sort_category.sortByName = 0;
    LinkedList_sort(list, CompareCategory, &sort_category);
}


typedef struct _SortLabroom
{
    int sortById;
    int sortByName;
} SortLabroom;

int CompareLabroom(const void* a, const void* b, void* arg)
{
    LabRoom* labroomA = (LabRoom*)a;
    LabRoom* labroomB = (LabRoom*)b;

    SortLabroom* sort_labroom = (SortLabroom*)arg;

    if (sort_labroom->sortById != 0)
    {
        int cmp = labroomA->id - labroomB->id;
        if (cmp != 0) return sort_labroom->sortById == 1 ? cmp : -cmp;
    }

    if (sort_labroom->sortByName != 0)
    {
        int cmp = strcmp(labroomA->name, labroomB->name);
        if (cmp != 0) return sort_labroom->sortByName == 1 ? cmp : -cmp;
    }

    return 0;
}

void SortLabroomMenu()
{
    SortLabroom sort_labroom;
    system("cls");
    printf("---        ʵ������������        ---\n\n");

    while (getchar() != '\n');

    printf("--- ����ʵ����ID����ʽ��1 ����-1 ���򣬻س�������-> ");
    char input[10];
    fgets(input, sizeof(input), stdin);
    sort_labroom.sortById = (strlen(input) > 1) ? atoi(input) : 0;

    printf("--- ����ʵ������������ʽ��1 ����-1 ���򣬻س�������-> ");
    fgets(input, sizeof(input), stdin);
    sort_labroom.sortByName = (strlen(input) > 1) ? atoi(input) : 0;

    LinkedList* list = GetResourceManage()->laboratory_list;

    LinkedList_sort(list, CompareLabroom, &sort_labroom);

    LinkedList_print(list, PrintLabroom);
}

typedef struct _SortEquiment
{
    int sortById;
    int sortByName;
    int sortByPrice;
    int sortByDate;
    int sortByCategoryId;
    int sortByLabroomId;
    
} SortEquipment;

int CompareEquiment(const void* a, const void* b, void* arg)
{
    ExperimentalEquipment* equimenA = (ExperimentalEquipment*)a;
    ExperimentalEquipment* equimenB = (ExperimentalEquipment*)b;

    SortEquipment* sort_equiment = (SortEquipment*)arg;

    if (sort_equiment->sortById != 0)
    {
        int cmp = equimenA->id - equimenB->id;
        if (cmp != 0) return sort_equiment->sortById == 1 ? cmp : -cmp;
    }

    if (sort_equiment->sortByName != 0)
    {
        int cmp = strcmp(equimenA->name, equimenB->name);
        if (cmp != 0) return sort_equiment->sortByName == 1 ? cmp : -cmp;
    }

    if (sort_equiment->sortByPrice != 0)
    {
        int cmp = equimenA->price - equimenB->price;
        if (cmp != 0) return sort_equiment->sortByPrice == 1 ? cmp : -cmp;
    }

    if (sort_equiment->sortByDate != 0)
    {
        int cmp = strcmp(equimenA->purchase_date, equimenB->purchase_date);
        if (cmp != 0) return sort_equiment->sortByDate == 1 ? cmp : -cmp;
    }

    if (sort_equiment->sortByCategoryId != 0)
    {
        int cmp = equimenA->category->id - equimenB->category->id;
        if (cmp != 0) return sort_equiment->sortByCategoryId == 1 ? cmp : -cmp;
    }

    if (sort_equiment->sortByLabroomId != 0)
    {
        int cmp = equimenA->room_id - equimenB->room_id;
        if (cmp != 0) return sort_equiment->sortByLabroomId == 1 ? cmp : -cmp;
    }

    return 0;
}

void SortEquipmentMenu()
{
    SortEquipment sort_equipment;
    system("cls");
    printf("---        ʵ���豸��������        ---\n\n");

    while (getchar() != '\n');

    printf("--- ����ʵ���豸ID����ʽ��1 ����-1 ���򣬻س�������-> ");
    char input[10];
    fgets(input, sizeof(input), stdin);
    sort_equipment.sortById = (strlen(input) > 1) ? atoi(input) : 0;

    printf("--- ����ʵ���豸��������ʽ��1 ����-1 ���򣬻س�������-> ");
    fgets(input, sizeof(input), stdin);
    sort_equipment.sortByName = (strlen(input) > 1) ? atoi(input) : 0;

    printf("--- ����ʵ���豸�۸�����ʽ��1 ����-1 ���򣬻س�������-> ");
    fgets(input, sizeof(input), stdin);
    sort_equipment.sortByPrice = (strlen(input) > 1) ? atoi(input) : 0;

    printf("--- ����ʵ���豸������������ʽ��1 ����-1 ���򣬻س�������-> ");
    fgets(input, sizeof(input), stdin);
    sort_equipment.sortByDate = (strlen(input) > 1) ? atoi(input) : 0;

    printf("--- ����ʵ���豸����ID����ʽ��1 ����-1 ���򣬻س�������-> ");
    fgets(input, sizeof(input), stdin);
    sort_equipment.sortByCategoryId = (strlen(input) > 1) ? atoi(input) : 0;

    printf("--- ����ʵ���豸����ʵ����ID����ʽ��1 ����-1 ���򣬻س�������-> ");
    fgets(input, sizeof(input), stdin);
    sort_equipment.sortByLabroomId = (strlen(input) > 1) ? atoi(input) : 0;

    LinkedList* list = GetResourceManage()->equipment_list;

    LinkedList_sort(list, CompareEquiment, &sort_equipment);

    LinkedList_print(list, PrintEquipment);
}