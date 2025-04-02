#pragma once
#ifndef LINKED_LIST
#define LINKED_LIST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void* DataType; // ͨ�� void* �����������ͱ���

typedef struct _Node    // ���
{
    DataType data;         // ��㴢�������(ͷ���ʱΪ��)
    struct _Node* next;    // ��һ�����
} Node;

typedef struct _LinkedList
{
    Node* head;    // ͷ���
    size_t size;   // ��������
} LinkedList;

// ��������
LinkedList* CreateLinkedList();

// ��������
void destoryLinkedList(LinkedList* list); 

// �������
Node* createNode(void* data);

// ����������Ԫ��
void LinkedList_pushback(LinkedList* list, void* data);

// �������в���Ԫ��
void LinkedList_insert(LinkedList* list, void* data, int index);

// ɾ�������е�Ԫ��
DataType LinkedList_delete(LinkedList* list, int index);

// �������������һ��Ԫ��
DataType LinkedList_back(LinkedList* list);

// ���������е�Ԫ��
DataType LinkedList_at(LinkedList* list, int index);

// ���������С
size_t LinkedList_size(LinkedList* list);

// �б�����
void LinkedList_sort(LinkedList* list, int (*cmp)(const void*, const void*, void*), void* arg);

// �б�������
void LinkedList_print(LinkedList* list, void (*PrintResult)(void*));

#endif // !LINKED_LIST
