#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "systemmaintenance.h"

// ϵͳά�����˵�������Ա��
//void ASystemMaintenance(Account* account)
//{
//    int choice;
//    bool exit = false;
//
//    while (!exit) {
//        system("cls");
//        printf("\n=== ϵͳά���˵�������Ա��===\n");
//        printf("1. �޸ĵ�ǰ�˻�����\n");
//        printf("2. �����û�/ʵ��Ա����\n");
//        printf("3. �޸ĵ�ǰ�˻��û���\n");
//        printf("4. �޸��û�/ʵ��Ա�û���\n");
//        printf("5. ����ϵͳ����\n");
//        printf("6. �ָ�ϵͳ����\n");
//        printf("0. ����\n");
//        printf("��ѡ��: ");
//
//        scanf_s("%d", &choice);
//        getchar();
//
//        system("cls");
//        switch (choice)
//        {
//        case 1:
//            ChangePassword(account);
//            break;
//
//        case 2:
//        {
//            int targetUserId;
//            printf("������Ҫ����������û�ID: ");
//            scanf_s("%d", &targetUserId);
//            fflush(stdin);
//            ResetUserPassword(targetUserId, account->id);
//            break;
//        }
//        case 3:
//            ChangeAccountName(account);
//            break;
//        case 4:
//        {
//            int targetUserId;
//            printf("������Ҫ�޸��û������û�ID: ");
//            scanf_s("%d", &targetUserId);
//            fflush(stdin);
//            AdminChangeUsername(account->id, targetUserId);
//            break;
//        }
//        case 5:
//            BackupSystemData(account->id);
//            break;
//        case 6:
//            RestoreSystemData(account->id);
//            break;
//        case 0:
//            exit = true;
//            break;
//
//        default:
//            printf("��Ч��ѡ��������\n");
//            break;
//        }
//        system("pause");
//    }
//}

// ϵͳά�����˵����û�/ʵ��Ա��
//void USystemMaintenance(Account* account)
//{
//    int choice;
//    bool exit = false;
//
//    while (!exit) {
//        system("cls");
//        printf("\n=== ϵͳά���˵� ===\n");
//        printf("1. �޸�����\n");
//        printf("2.�޸��û���\n");
//        printf("0. ����\n");
//        printf("��ѡ��: ");
//
//        scanf_s("%d", &choice);
//        fflush(stdin);
//
//        system("cls");
//        switch (choice) {
//        case 1:
//            ChangePassword(account);
//            break;
//        case 2:
//            ChangeAccountName(account);
//            break;
//        case 0:
//            exit = true;
//            break;
//
//        default:
//            printf("��Ч��ѡ��������\n");
//            break;
//        }
//        system("pause");
//    }
//}

// ��¼ϵͳ������־
bool LogSystemOperation(const char* operation, int userId)
{
    FILE* fp;
    fopen_s(&fp, "system_log.txt", "a");
    if (fp == NULL)
    {
        printf("�޷�����־�ļ�\n");
        return false;
    }

    time_t now;
    time(&now);
    char time[26];
    ctime_s(time, sizeof(time), &now);
    time[24] = '\0'; //ctime�������Զ������з������������Ƴ����з�

    Account* account = FindById(userId);
    const char* username = account ? account->user_name : "δ֪�û�";

    fprintf(fp, "[%s] �û�: %s (ID: %d) ����: %s\n",
        time, username, userId, operation);

    fclose(fp);
    return true;
}

bool BackupSystemData(int adminId)
{
    //����backup�ļ���
    system("if not exist backups mkdir backups");
    //�н���ǰ�ӱ����ļ���
    time_t now;
    time(&now);
    char time[26];
    ctime_s(time, sizeof(time), &now);
    time[24] = '\0'; //ctime�������Զ������з������������Ƴ����з�

    char backupDir[100];
    char mkdirCmd[150];
    char formattedTime[30];
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%d_%H-%M-%S", &timeinfo);

    // ʹ�ø�ʽ�����ʱ�䴴��Ŀ¼��
    sprintf_s(backupDir, sizeof(backupDir), "backups\\%s", formattedTime);
    sprintf_s(mkdirCmd, sizeof(mkdirCmd), "mkdir %s", backupDir);

    system(mkdirCmd);
    char path[100];
	wchar_t wpath[100];
    size_t converted_chars;
    // ��������ͷ�ļ���������readme
    SaveBackUp(time, formattedTime);
    snprintf(path, 100, "backups/%s/account.txt", formattedTime);
    mbstowcs_s(&converted_chars, wpath, 100, path, _TRUNCATE);
    SaveAccountList(wpath);

    snprintf(path, 100, "backups/%s/category.txt", formattedTime);
    mbstowcs_s(&converted_chars, wpath, 100, path, _TRUNCATE);
    SaveCategoryList(wpath);

    snprintf(path, 100, "backups/%s/equipment.txt", formattedTime);
    mbstowcs_s(&converted_chars, wpath, 100, path, _TRUNCATE);
    SaveEquipmentList(wpath);

    snprintf(path, 100, "backups/%s/laboratory.txt", formattedTime);
    mbstowcs_s(&converted_chars, wpath, 100, path, _TRUNCATE);
    SaveLaboratoryList(wpath);

	snprintf(path, 100, "backups/%s/service.txt", formattedTime);
	mbstowcs_s(&converted_chars, wpath, 100, path, _TRUNCATE);
	SaveServiceList(wpath);

    LogSystemOperation("ϵͳ����", adminId);

    printf("ϵͳ���ݱ�����ɣ�������: %s\n", backupDir);

    return True;
}

// �ָ�ϵͳ����
bool RestoreSystemData(int adminId,wchar_t* formattedtime)
{
    char operation[50];
    char lformattedtime[100];
	wcstombs_s(NULL, lformattedtime, sizeof(lformattedtime), formattedtime, _TRUNCATE);
    snprintf(operation, sizeof(operation), "ϵͳ���ݻָ���%s", lformattedtime);
    LogSystemOperation(operation, adminId);
    DestoryResourceManage();
    wchar_t path[100];
    swprintf_s(path, sizeof(path) / sizeof(wchar_t), L"backups/%s/laboratory.txt", formattedtime);
    LoadLaboratoryList(path);
    swprintf_s(path, sizeof(path) / sizeof(wchar_t), L"backups/%s/account.txt", formattedtime);
    LoadAccountList(path);
    swprintf_s(path, sizeof(path) / sizeof(wchar_t), L"backups/%s/category.txt", formattedtime);
    LoadCategoryList(path);
    swprintf_s(path, sizeof(path) / sizeof(wchar_t), L"backups/%s/equipment.txt", formattedtime);
    LoadEquipmentList(path);
	swprintf_s(path, sizeof(path) / sizeof(wchar_t), L"backups/%s/service.txt", formattedtime);
	LoadServiceList(path);
    SaveResource();
	MessageBox(NULL, L"���ݻָ��ɹ������ֶ�����", L"��ʾ", MB_OK | MB_ICONINFORMATION);
    ExitProcess(0);
    return true;
}
