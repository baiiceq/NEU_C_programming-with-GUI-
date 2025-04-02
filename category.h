#pragma once
#ifndef CATEGORY_H
#define CATEGORY_H

#include <stdio.h>
#include <string.h>

#include "configs.h"
#include "id_manager.h"
#include "resource_manager.h"

typedef struct _Category
{
	int id;                        // ���ID
	wchar_t name[CATEGORY_LENGTH];    // �������
	int disposal_years;            // ��������
}Category;

Category* CreateCategory(char* name, int disposal_years);

void DestoryCategory(Category* category);

void AddCategory();
Category* FindCategoryById(int id);

Category* Id_to_Category(int id);
#endif // !CATEGORY_H
