#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "systemmaintenance.h"

// 系统维护主菜单（管理员）
//void ASystemMaintenance(Account* account)
//{
//    int choice;
//    bool exit = false;
//
//    while (!exit) {
//        system("cls");
//        printf("\n=== 系统维护菜单（管理员）===\n");
//        printf("1. 修改当前账户密码\n");
//        printf("2. 重置用户/实验员密码\n");
//        printf("3. 修改当前账户用户名\n");
//        printf("4. 修改用户/实验员用户名\n");
//        printf("5. 备份系统数据\n");
//        printf("6. 恢复系统数据\n");
//        printf("0. 返回\n");
//        printf("请选择: ");
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
//            printf("请输入要重置密码的用户ID: ");
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
//            printf("请输入要修改用户名的用户ID: ");
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
//            printf("无效的选择，请重试\n");
//            break;
//        }
//        system("pause");
//    }
//}

// 系统维护主菜单（用户/实验员）
//void USystemMaintenance(Account* account)
//{
//    int choice;
//    bool exit = false;
//
//    while (!exit) {
//        system("cls");
//        printf("\n=== 系统维护菜单 ===\n");
//        printf("1. 修改密码\n");
//        printf("2.修改用户名\n");
//        printf("0. 返回\n");
//        printf("请选择: ");
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
//            printf("无效的选择，请重试\n");
//            break;
//        }
//        system("pause");
//    }
//}

// 记录系统操作日志
bool LogSystemOperation(const char* operation, int userId)
{
    FILE* fp;
    fopen_s(&fp, "system_log.txt", "a");
    if (fp == NULL)
    {
        printf("无法打开日志文件\n");
        return false;
    }

    time_t now;
    time(&now);
    char time[26];
    ctime_s(time, sizeof(time), &now);
    time[24] = '\0'; //ctime函数会自动补换行符，所以这里移除换行符

    Account* account = FindById(userId);
    const char* username = account ? account->user_name : "未知用户";

    fprintf(fp, "[%s] 用户: %s (ID: %d) 操作: %s\n",
        time, username, userId, operation);

    fclose(fp);
    return true;
}

bool BackupSystemData(int adminId)
{
    //创建backup文件夹
    system("if not exist backups mkdir backups");
    //切进当前子备份文件夹
    time_t now;
    time(&now);
    char time[26];
    ctime_s(time, sizeof(time), &now);
    time[24] = '\0'; //ctime函数会自动补换行符，所以这里移除换行符

    char backupDir[100];
    char mkdirCmd[150];
    char formattedTime[30];
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%d_%H-%M-%S", &timeinfo);

    // 使用格式化后的时间创建目录名
    sprintf_s(backupDir, sizeof(backupDir), "backups\\%s", formattedTime);
    sprintf_s(mkdirCmd, sizeof(mkdirCmd), "mkdir %s", backupDir);

    system(mkdirCmd);
    char path[100];
	wchar_t wpath[100];
    size_t converted_chars;
    // 创建备份头文件，类似于readme
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

    LogSystemOperation("系统备份", adminId);

    printf("系统数据备份完成，保存在: %s\n", backupDir);

    return True;
}

// 恢复系统数据
bool RestoreSystemData(int adminId,wchar_t* formattedtime)
{
    char operation[50];
    char lformattedtime[100];
	wcstombs_s(NULL, lformattedtime, sizeof(lformattedtime), formattedtime, _TRUNCATE);
    snprintf(operation, sizeof(operation), "系统数据恢复至%s", lformattedtime);
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
	MessageBox(NULL, L"数据恢复成功，请手动重启", L"提示", MB_OK | MB_ICONINFORMATION);
    ExitProcess(0);
    return true;
}
