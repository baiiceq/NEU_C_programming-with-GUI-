#pragma once
#include "resource_manager.h"
#include "account.h"
#include "configs.h"


// 记录系统操作日志
bool LogSystemOperation(const char* operation, int userId);
// 备份系统数据
bool BackupSystemData(int adminId);
// 恢复系统数据
bool RestoreSystemData(int adminId,wchar_t*);
