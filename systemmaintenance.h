#pragma once
#include "resource_manager.h"
#include "account.h"
#include "configs.h"

//// ϵͳά�����˵�������Ա��
//void ASystemMaintenance(Account* account);
//// ϵͳά�����˵����û�/ʵ��Ա��
//void USystemMaintenance(Account* account);

// ��¼ϵͳ������־
bool LogSystemOperation(const char* operation, int userId);
// ����ϵͳ����
bool BackupSystemData(int adminId);
// �ָ�ϵͳ����
bool RestoreSystemData(int adminId);
