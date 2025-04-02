#pragma once
#include "account.h"
#include "category.h"
#include "lab_room.h"
#include "experimental_equipment.h"
#include "sort.h"

#include "string.h"

typedef struct _EquipmentsCount
{
    int categoryId;
    int countByCategory; 
    int room_id;
    int countByRoom;     
    int countByDate;
    int countByPrice;
    int min_price;
    int max_price;
    char startDate[DATE_LENGTH];
    char endDate[DATE_LENGTH];
    int count;
} EquipmentsCount;


void CountEquipment(LinkedList* list, EquipmentsCount* Count) 
{
    if (!list || !Count) return;

    Node* node = list->head->next;
    while (node) 
    {
        ExperimentalEquipment* ee = (ExperimentalEquipment*)node->data;

        bool is_category_conform = Count->countByCategory < 0 || 
            (Count->countByCategory >= 0 && Count->categoryId == ee->category->id);
        bool is_roomid_conform = Count->countByRoom < 0 || 
            (Count->countByRoom >= 0 && Count->room_id == ee->room_id);
        bool is_date_conform = Count->countByDate < 0 || 
            (strcmp(ee->purchase_date, Count->startDate) >= 0 && strcmp(ee->purchase_date, Count->endDate) <= 0);
        bool is_price_conform = Count->countByPrice < 0 ||
            (ee->price >= Count->min_price && ee->price <= Count->max_price);

        if (is_category_conform && is_roomid_conform && is_date_conform && is_price_conform)
        {
            Count->count++;
        }
        node = node->next;
    }
}

static void PrintEquipment2(EquipmentsCount* Count)
{
    ResourceManager* rm = GetResourceManage();

    printf("--- �豸ͳ�ƽ�� ---\n");
    if (Count->countByRoom >= 0)
    {
        LabRoom* labroom = RoomId_to_LabRoom(Count->room_id);
        printf("--- ʵ����%s ��:\n", labroom->name);
    }

    if (Count->countByDate >= 0)
    {
        printf("������%s��%s֮��\n", Count->startDate, Count->endDate);
    }

    if (Count->countByPrice >= 0)
    {
        printf("�ڼ۸�%dԪ��%dԪ֮��\n", Count->min_price, Count->max_price);
    }

    if (Count->countByCategory >= 0)
    {
        Category* category = Id_to_Category(Count->categoryId);

        printf("����Ϊ%s\n", category->name);
    }

    printf("����%d��\n",Count->count);
    system("pause");
        
}

void StatisticsEquipmentMenu()
{
    EquipmentsCount Count;
    Count.countByCategory = -1;
    Count.countByDate = -1;
    Count.countByPrice = -1;
    Count.countByRoom = -1;
    Count.count = 0;

    ResourceManager* rm = GetResourceManage();

    system("cls");

    printf("---        �豸ͳ������        ---\n\n");

    while (getchar() != '\n');

    // ���豸����ͳ��
    printf("--- �Ƿ����豸����ͳ�ƣ�1 ȷ�����س�������-> ");
    char input[10];
    fgets(input, sizeof(input), stdin);
    if (input[0] == '1') 
    {
        printf("--- ��ǰͳ��������\n");
        size_t category_count = rm->category_list->size;

        printf("---      ѡ����Ҫͳ�Ƶ�����        ---\n\n");

        Node* temp = rm->category_list->head->next;
        for (size_t i = 0; i < category_count; i++)
        {
            Category* category = (Category*)temp->data;
            printf("--- No%d. %s\n", (int)i + 1, category->name);
            temp = temp->next;
        }

        printf("--- ������Ҫͳ�Ƶ��豸���ͱ��: ");
        fgets(input, sizeof(input), stdin);
        int categoryIndex = atoi(input);
        if (categoryIndex > 0 && categoryIndex <= (int)category_count) 
        {
            Count.countByCategory = 1;
            Count.categoryId = categoryIndex;
        }
    }

    // ��ʵ����ͳ��
    printf("--- �Ƿ����豸����ʵ����ͳ�ƣ�1 ȷ�����س�������-> ");
    fgets(input, sizeof(input), stdin);
    if (input[0] == '1') 
    {
        printf("--- ������ʵ����ID: ");
        fgets(input, sizeof(input), stdin);
        Count.room_id = atoi(input);

        LabRoom* labroom = RoomId_to_LabRoom(Count.room_id);
        if (labroom == NULL)
        {
            printf("��ʵ���Ҳ�����\n");
            system("pause");
            return;
        }
        Count.countByRoom = 1;
    }

    // ���۸�ͳ��
    printf("--- �Ƿ����豸�۸�ͳ�ƣ�1 ȷ�����س�������-> ");
    fgets(input, sizeof(input), stdin);
    if (input[0] == '1') 
    {
        printf("--- ��������ͼ۸�: ");
        fgets(input, sizeof(input), stdin);
        Count.min_price = atof(input);

        printf("--- ��������߼۸�: ");
        fgets(input, sizeof(input), stdin);
        Count.max_price = atof(input);

        Count.countByPrice = 1;
    }

    printf("--- �Ƿ����豸����ͳ�ƣ�1 ȷ�����س�������-> ");
    fgets(input, sizeof(input), stdin);
    if (input[0] == '1') 
    {
        printf("--- ����������ʱ�䣨YYYYMMDD��: ");
        fgets(Count.startDate, sizeof(Count.startDate), stdin);
        Count.startDate[strcspn(Count.startDate, "\n")] = '\0';  

        printf("--- ����������ʱ�䣨YYYYMMDD��: ");
        fgets(Count.endDate, sizeof(Count.endDate), stdin);
        Count.endDate[strcspn(Count.endDate, "\n")] = '\0';

        Count.countByDate = 1;
    }

    CountEquipment(rm->equipment_list, &Count);

    PrintEquipment2(&Count);
}

typedef struct _AccountCount
{
    AccountType type;
    int countByType;
    int room_id;
    int countByRoom;
    int count;
} AccountCount;

void CountAccount(LinkedList* list, AccountCount* Count)
{
    if (!list || !Count) return;

    Node* node = list->head->next;
    while (node)
    {
        Account* account = (Account*)node->data;

        bool is_category_conform = Count->countByType < 0 ||
            (Count->countByType >= 0 && Count->type == account->account_type);
        bool is_roomid_conform = Count->countByRoom < 0 ||
            (Count->countByRoom >= 0 && Count->room_id == account->roomid);

        if (is_category_conform && is_roomid_conform)
        {
            Count->count++;
        }
        node = node->next;
    }
}

static void PrintAccount2(AccountCount* Count)
{
    ResourceManager* rm = GetResourceManage();

    printf("--- �豸ͳ�ƽ�� ---\n");
    if (Count->countByRoom >= 0)
    {
        LabRoom* labroom = RoomId_to_LabRoom(Count->room_id);
        printf("--- ʵ����%s ��:\n", labroom->name);
    }

    if (Count->countByType >= 0)
    {
        char type[10] = "";
        switch (Count->type)
        {
        case Admin:
            strcpy_s(type, 10, "����Ա");
            break;
        case Experimenter:
            strcpy_s(type, 10, "ʵ��Ա");
            break;
        case User:
            strcpy_s(type, 10, "һ���û�");
            break;
        }
        printf("--- �˻�����Ϊ%s :\n", type);
    }

    printf("����%d��\n", Count->count);
    system("pause");

}

void StatisticsAccountMenu()
{
    AccountCount Count;
    Count.countByType = -1;
    Count.type = Unknow;
    Count.countByRoom = -1;
    Count.count = 0;

    ResourceManager* rm = GetResourceManage();

    system("cls");

    printf("---        �豸ͳ������        ---\n\n");

    while (getchar() != '\n');

    // ���˻�����ͳ��
    printf("--- �Ƿ����˻�����ͳ�ƣ�1 ȷ�����س�������-> ");
    char input[10];
    fgets(input, sizeof(input), stdin);
    if (input[0] == '1')
    {
        printf("--- �˻�������\n");

        printf("---      ѡ����Ҫͳ�Ƶ�����        ---\n\n");

        printf("--- No1. ����Ա\n");
        printf("--- No2. ʵ��Ա\n");
        printf("--- No3. һ���û�\n");

        printf("--- ������Ҫͳ�Ƶ��˻�����: ");
        fgets(input, sizeof(input), stdin);
        int accountIndex = atoi(input);
        if (accountIndex > 0 && accountIndex <= 3)
        {
            Count.countByType = 1;
            Count.type = (AccountType)(accountIndex - 1);
        }

        if (Count.type == Experimenter)
        {
            printf("--- �Ƿ���ʵ��Աʵ����ͳ�ƣ�1 ȷ�����س�������-> ");
            fgets(input, sizeof(input), stdin);
            if (input[0] == '1')
            {
                printf("--- ������ʵ����ID: ");
                fgets(input, sizeof(input), stdin);
                Count.room_id = atoi(input);

                LabRoom* labroom = RoomId_to_LabRoom(Count.room_id);
                if (labroom == NULL)
                {
                    printf("��ʵ���Ҳ�����\n");
                    system("pause");
                    return;
                }
                Count.countByRoom = 1;
            }
        }
    }

    CountAccount(rm->equipment_list, &Count);

    PrintAccount2(&Count);
}