#include "register_interface.h"
#include "account.h"

int ShowRegisterMenu()
{
    system("cls");

    int option = 0;
    printf("---   ע�����   ---\n\n");
    printf("---   1. ע��    ---\n");
    printf("---   2. ��¼    ---\n");
    printf("---   0. �˳�    ---\n");
    printf("--- ������ѡ��->");

    scanf_s("%d", &option);
    while (getchar() != '\n');

    system("pause");
    return option;
}

void RunRegisterInterface(int option, EquipmentManagement* em)
{
    switch (option)
    {
    case 0:
        em->running = False;
        break;
    case 1:
    {
        Account* account = AccountRegister();
        if (account == NULL)
        {
            printf("ע��ʧ��\n");
            system("pause");
            return;
        }
        
        // �������˻�
        ResourceManager* resource_manager = GetResourceManage();
        LinkedList_pushback(resource_manager->account_list, account);
		printf("ע��ɹ�\n");
    }
    break;
    case 2:
        em->current_state = Login;
        break;
    default:
        printf("�����Ƿ���\n");
        break;
    }
    system("pause");
}

Account* AccountRegister()
{
    Account* account = CreateAccount();
    int a = 0;
    printf("�������û�����1.����Ա2.ʵ��Ա3.һ���û�\n");
    scanf_s("%d",&a);
    while (getchar() != '\n');
    switch (a) 
    {
	case 1:
		account->account_type = Admin;
		break;
    case 2:
		account->account_type = Experimenter;
        break;
	case 3:
		account->account_type = User;
		break;
	default:
		printf("�û����Ͳ��Ϸ�\n");
		return NULL;
    }
    printf("�������û���(������4-12λ����ֻ�ܰ�����ĸ�����֣�\n");
	fgets(account->user_name, USER_NMAE_LENGTH, stdin);
    account->user_name[strcspn(account->user_name, "\n")] = '\0';
    printf("�������û����루������8-20λ����ֻ�ܰ�����ĸ�����Լ�@+����\n");
    fgets(account->user_password, sizeof(account->user_password), stdin);
    account->user_password[strcspn(account->user_password, "\n")] = '\0';

    if (!ValidAccount(account))
    {
		free(account);
		return NULL;
    }
	account->id = GetNewId(AccountID);
    account->roomid = -1;
    return account;
}
