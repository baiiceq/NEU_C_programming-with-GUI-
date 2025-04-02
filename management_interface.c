#include "management_interface.h"
#include "query.h"
#include "sort.h"
#include "statistics.h"


int ShowManagementMenu(AccountType account_type)
{
    system("cls");

    int option = 0;
    printf("---         �������        ---\n\n");
    switch (account_type)
    {
    case Admin:
        printf("---        1. ������Ϣ����      ---\n");
        printf("---        2. ��Ϣ��������      ---\n");
        printf("---        3. ��Ϣ��ѯ          ---\n");
        printf("---        4. ��Ϣ����          ---\n");
        printf("---        5. ��Ϣͳ��          ---\n");
        printf("---        6. ϵͳά��          ---\n");
        break;
    case Experimenter:
        printf("---        1. �豸����          ---\n");
        printf("---        2. ��Ϣ��ѯ          ---\n");
        printf("---        3. ��Ϣ����          ---\n");
        printf("---        4. ��Ϣͳ��          ---\n");
        printf("---        5. ϵͳά��          ---\n");
        break;
    case User:
        printf("---        1. ��Ϣ��ѯ          ---\n");
        printf("---        2. ��Ϣ����          ---\n");
        printf("---        3. ��Ϣͳ��          ---\n");
        printf("---        4. ϵͳά��          ---\n");
        break;
    }

    printf("---        0. �˳�          ---\n");
    printf("--- ������ѡ��->");

    scanf_s("%d", &option);
    while (getchar() != '\n');

    system("pause");
    return option;
} 

void RunManagementInterface(int option, EquipmentManagement* em, AccountType account_type)
{
    switch (account_type)
    {
    case Admin:
        switch (option)
        {
        case 0:
            free(em->current_account);
            em->current_account = NULL;
            em->current_state = Login;
            return ;
        case 1:
            BasicInformationManagement();
            break;
        case 3:
            QueryInformation();
            break;
        case 4:
            SortInformation();
            break;
        case 5:
            Statistics();
            break;
        default:
            break;
        }
        break;
    case Experimenter:
        switch (option)
        {
        case 0:
            free(em->current_account);
            em->current_account = NULL;
            em->current_state = Login;
            return;
        default:
            break;
        }
        break;

    case User:
        switch (option)
        {
        case 0:
            free(em->current_account);
            em->current_account = NULL;
            em->current_state = Login;
            return;
        default:
            break;
        }
        break;
    }
}

void QueryInformation()
{
    system("cls");

    printf("---             ��Ϣ��ѯ           ---\n\n");
    printf("---        ��ѡ����Ҫ��ѯ����Ϣ      ---\n");
    printf("---          1. ʵ����               ---\n");
    printf("---          2. �˻�                 ---\n");
    printf("---          3. ʵ���豸���         ---\n");
    printf("---          4. ʵ���豸             ---\n");
    printf("---          0. ��  ��               ---\n");
    printf("ѡ��->");

    int option = 0;
    scanf_s("%d", &option);

    switch (option)
    {
    case 0:
        return;
    case 1:
        QueryLabroomMenu();
        break;
    case 2:
        QueryAccountMenu();
        break;
    case 3:
        QueryCategoryMenu();
        break;
    case 4:
        QueryEquipmentMenu();
        break;
    default:
        printf("�Ƿ�ָ��\n");
        system("pause");
        break;
    }
}

void SortInformation()
{
    system("cls");

    printf("---             ��Ϣ����           ---\n\n");
    printf("---        ��ѡ����Ҫ�������Ϣ      ---\n");
    printf("---          1. ʵ����               ---\n");
    printf("---          2. �˻�                 ---\n");
    printf("---          3. ʵ���豸���         ---\n");
    printf("---          4. ʵ���豸             ---\n");
    printf("---          0. ��  ��               ---\n");
    printf("ѡ��->");

    int option = 0;
    scanf_s("%d", &option);

    switch (option)
    {
    case 0:
        return;
    case 1:
        SortLabroomMenu();
        break;
    case 2:
        SortAccountMenu();
        break;
    case 3:
        SortCategoryMenu();
        break;
    case 4:
        SortEquipmentMenu();
        break;
    default:
        printf("�Ƿ�ָ��\n");
        system("pause");
        break;
    }
}

void Statistics()
{
    system("cls");

    printf("---             ��Ϣͳ��           ---\n\n");
    printf("---        ��ѡ����Ҫͳ�Ƶ���Ϣ      ---\n");
    printf("---          1. ʵ����               ---\n");
    printf("---          2. �˻�                 ---\n");
    printf("---          3. ʵ���豸���         ---\n");
    printf("---          4. ʵ���豸             ---\n");
    printf("---          0. ��  ��               ---\n");
    printf("ѡ��->");

    int option = 0;
    scanf_s("%d", &option);

    switch (option)
    {
    case 0:
        return;
    case 1:
        printf("��ǰ����%d��ʵ����\n", GetResourceManage()->laboratory_list->size);
        system("pause");
        break;
    case 2:
        StatisticsAccountMenu();
        break;
    case 3:
        printf("��ǰ����%d��ʵ���豸\n", GetResourceManage()->category_list->size);
        system("pause");
        break;
    case 4:
        StatisticsEquipmentMenu();
        break;
    default:
        printf("�Ƿ�ָ��\n");
        system("pause");
        break;
    }
}

void SystemMaintenance(AccountType account_type)
{
}

void BasicInformationManagement()
{
    system("cls");

    printf("---            ������Ϣ����          ---\n\n");
    printf("---        ��ѡ����Ҫ�������Ϣ      ---\n");
    printf("---          1. ʵ����               ---\n");
    printf("---          2. ʵ��Ա               ---\n");
    printf("---          3. ʵ���豸���         ---\n");
    printf("---          4. ʵ���豸             ---\n");
    printf("---          0. ��  ��               ---\n");
    printf("ѡ��->");

    int option = 0;
    scanf_s("%d", &option);

    switch (option)
    {
    case 0:
        return;
    case 1:
        ManageLabRoom();
        break;
    case 2:
        ManageAccount();
        break;
    case 3:
        ManageCategory();
        break;
    case 4:
        ManageExperimentalEquipment();
        break;
    default:
        printf("�Ƿ�ָ��\n");
        system("pause");
        break;
    }
}

void AdjustmentInformation()
{
}

void EquipmentsManagement()
{
}

