#pragma once
#include "resource_manager.h"
#include "account.h"
#include "configs.h"


// ��¼ϵͳ������־
bool LogSystemOperation(const char* operation, int userId);
// ����ϵͳ����
bool BackupSystemData(int adminId);
// �ָ�ϵͳ����
bool RestoreSystemData(int adminId,wchar_t*);
