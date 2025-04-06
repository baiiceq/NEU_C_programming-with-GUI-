#include "id_manager.h"

static IdManager* instance = NULL;

static void init_instance()
{
    if (instance == NULL)
    {
        instance = (IdManager*)malloc(sizeof(IdManager));
        if (instance == NULL)
        {
            printf("ID����������ʧ��");
            exit(-1);
        }
    }
}

int GetNewId(IdType type)
{
    init_instance();

    switch (type)
    {
    case AccountID:
        return ++instance->current_account_id;
        break;
    case CategoryID:
        return ++instance->current_category_id;
        break;
    case EquipmentID:
        return ++instance->current_equipment_id;
        break;
    case RoomID:
        return ++instance->current_room_id;
	case ServiceID:
		return ++instance->current_service_id;
		break;
        break;
    }
}

void DestoryIdManager()
{
    if (instance)
        free(instance);
}

bool SaveId()
{
    init_instance();

    FILE* fp = fopen("current_id.txt", "w");
    if (fp == NULL)
    {
        printf("�ļ���ʧ��\n");
        //system("pause");
        return False;
    }
    fprintf(fp, "%d %d %d %d %d\n",instance->current_account_id,
        instance->current_category_id,
        instance->current_equipment_id,
        instance->current_room_id,
        instance->current_service_id);

    fclose(fp);
    return True;
}

bool LoadId()
{
    init_instance();
    FILE* fp = fopen("current_id.txt", "r");
    if (fp == NULL)
    {
        printf("�ļ���ʧ��\n");
        return False;
    }
    while (!feof(fp))
    {
        fscanf_s(fp, "%d %d %d %d %d\n",&instance->current_account_id,
            &instance->current_category_id,
            &instance->current_equipment_id,
            &instance->current_room_id,
            &instance->current_service_id);
    }
    fclose(fp);
}
