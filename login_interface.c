#include "login_interface.h"
#include "account.h"

int ShowLoginMenu()
{
    system("cls");

    int option = 0;
    printf("---   ��¼����   ---\n\n");
    printf("---   1. ��¼    ---\n");
    printf("---   2. ע��    ---\n");
    printf("---   0. �˳�    ---\n");
    printf("--- ������ѡ��->");

    scanf_s("%d", &option);
    while (getchar() != '\n');

    system("pause");
    return option;
}

void RunLoginInterface(int option, EquipmentManagement* em)
{
    switch (option)
    {
    case 0:
        em->running = False;
        break;
    case 1:
    {
        Account* account = AccountLogin();
        if (account == NULL)
        {
            printf("��¼ʧ��\n");
            system("pause");
            return;
        }
        em->current_state = Management;
        em->current_account = account;
        printf("��¼�ɹ�\n");
    }
        break;
    case 2:
        em->current_state = Register;
        break;
    default:
        printf("�����Ƿ���\n");
        break;
    }
    system("pause");
}

Account* AccountLogin()
{
    Account* account = (Account*)malloc(sizeof(Account));

    printf("�������û���(������4-12λ����ֻ�ܰ�����ĸ�����֣�\n");
    fgets(account->user_name, USER_NMAE_LENGTH, stdin);
    account->user_name[strcspn(account->user_name, "\n")] = '\0';

    printf("�������û����루������8-20λ����ֻ�ܰ�����ĸ�����Լ�@+����\n");
    fgets(account->user_password, sizeof(account->user_password), stdin);
    account->user_password[strcspn(account->user_password, "\n")] = '\0';

    if (!IsCorrectAccount(account))
    {
		free(account);
        return NULL;
    }

    return account;
}
