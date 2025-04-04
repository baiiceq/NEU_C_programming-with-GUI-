#pragma once
#include "resource_manager.h"
#include "account.h"
#include "configs.h"

//// 系统维护主菜单（管理员）
//void ASystemMaintenance(Account* account);
//// 系统维护主菜单（用户/实验员）
//void USystemMaintenance(Account* account);

// 记录系统操作日志
bool LogSystemOperation(const char* operation, int userId);
// 备份系统数据
bool BackupSystemData(int adminId);
// 恢复系统数据
bool RestoreSystemData(int adminId);
