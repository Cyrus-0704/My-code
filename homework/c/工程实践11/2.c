/*链表法*/

#include <stdio.h>
#include <stdlib.h>

// 定义元素类型为整型
typedef int ElemType;

// 定义链表节点结构
typedef struct LNode
{
    ElemType data;
    struct LNode *next;
} LNode, *LinkList; // LNode是节点类型，LinkList是指向节点的指针类型

/*创建带头节点的空链表*/
LinkList CreateList()
{
    // 分配头节点内存
    LinkList L = (LinkList)malloc(sizeof(LNode));
    if (L == NULL)
    {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    // 初始化头节点的next指针为NULL，表示空链表
    L->next = NULL;
    return L;
}

/*判断链表是否为空*/
int ListEmpty(LinkList L)
{
    return L->next == NULL; // 如果头节点的next为NULL，则链表为空
}

/*获取链表的长度*/
int ListLength(LinkList L)
{
    int length = 0;
    LinkList p = L->next; // p指向第一个节点

    // 遍历链表，计数节点个数
    while (p)
    {
        length++;
        p = p->next;
    }
    return length;
}

/*
获取链表中指定位置的元素
L 链表头节点指针
i 指定位置（从1开始）
e 用于存储获取到的元素
*/
int GetElem(LinkList L, int i, ElemType *e)
{
    // 检查位置是否有效
    if (i < 1)
        return 0;

    LinkList p = L->next; // p指向第一个节点
    int j = 1;            // j表示当前是第几个节点

    // 遍历链表，找到第i个节点
    while (p && j < i)
    {
        p = p->next;
        j++;
    }

    // 如果p为NULL或j>i，位置无效
    if (!p || j > i)
        return 0;

    // 获取元素值
    *e = p->data;
    return 1;
}

/*比较两个元素是否相等*/
int equal(ElemType a, ElemType b)
{
    return a == b;
}

/*
查找元素在链表中的位置
L 链表头节点指针
e 要查找的元素
compare 比较函数指针
*/
int LocateElem(LinkList L, ElemType e, int (*compare)(ElemType, ElemType))
{
    LinkList p = L->next; // p指向第一个节点
    int i = 1;            // i表示当前是第几个节点

    // 遍历链表，查找元素
    while (p)
    {
        if (compare(p->data, e))
        {
            return i; // 找到元素，返回位置
        }
        p = p->next;
        i++;
    }

    return 0; // 未找到元素
}

/*
在链表中插入元素
L 链表头节点指针
i 插入位置（从1开始）
e 要插入的元素
*/
int ListInsert(LinkList L, int i, ElemType e)
{
    // 检查位置是否有效
    if (i < 1)
        return 0;

    LinkList p = L; // p指向头节点
    int j = 0;      // j表示当前是第几个节点

    // 遍历链表，找到第i-1个节点
    while (p && j < i - 1)
    {
        p = p->next;
        j++;
    }

    // 如果p为NULL或j>i-1，说明位置无效
    if (!p || j > i - 1)
        return 0;

    // 创建新节点
    LinkList s = (LinkList)malloc(sizeof(LNode));

    // 设置新节点的数据域和指针域
    s->data = e;
    s->next = p->next;
    p->next = s;

    return 1;
}

/*
从链表中删除元素
L 链表头节点指针
i 删除位置（从1开始）
e 用于存储被删除的元素
*/
int ListDelete(LinkList L, int i, ElemType *e)
{
    // 检查位置是否有效
    if (i < 1)
        return 0;

    LinkList p = L; // p指向头节点
    int j = 0;      // j表示当前是第几个节点

    // 遍历链表，找到第i-1个节点
    while (p->next && j < i - 1)
    {
        p = p->next;
        j++;
    }

    // 如果p->next为NULL或j>i-1，说明位置无效
    if (!p->next || j > i - 1)
        return 0;

    // 删除节点
    LinkList q = p->next; // q指向要删除的节点
    p->next = q->next;    // 将q从链表中移除
    *e = q->data;         // 保存被删除的元素值
    free(q);

    return 1;
}

/*求两个链表的并集 将Lb中不在La中的元素插入到La中*/
void Union(LinkList La, LinkList Lb)
{
    ElemType e;
    int Lb_len = ListLength(Lb);

    // 遍历Lb中的每个元素
    for (int i = 1; i <= Lb_len; i++)
    {
        // 获取Lb中的元素
        GetElem(Lb, i, &e);
        // 如果该元素不在La中，则插入到La中
        if (!LocateElem(La, e, equal))
        {
            ListInsert(La, ListLength(La) + 1, e);
        }
    }
}

/*合并两个有序链表*/
void MergeList(LinkList La, LinkList Lb, LinkList *Lc)
{
    LinkList pa = La->next; // pa指向La的第一个节点
    LinkList pb = Lb->next; // pb指向Lb的第一个节点
    LinkList pc;            // pc指向Lc的当前节点
    LinkList temp;          // 临时节点指针

    // 创建新的头节点
    *Lc = (LinkList)malloc(sizeof(LNode));
    if (*Lc == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }
    pc = *Lc;           // pc指向Lc的头节点
    (*Lc)->next = NULL; // 确保新创建的链表初始为空

    // 特殊情况处理：如果两个链表都为空，直接返回
    if (!pa && !pb)
    {
        return;
    }

    // 合并两个有序链表
    while (pa && pb)
    {
        if (pa->data <= pb->data)
        {
            // 创建新节点并复制数据
            temp = (LinkList)malloc(sizeof(LNode));
            if (temp == NULL)
            {
                printf("Memory allocation failed!\n");
                return;
            }
            temp->data = pa->data;
            temp->next = NULL;

            // 链接到结果链表
            pc->next = temp;
            pc = temp;

            // 移动到下一个节点
            pa = pa->next;
        }
        else
        {
            // 创建新节点并复制数据
            temp = (LinkList)malloc(sizeof(LNode));
            if (temp == NULL)
            {
                printf("Memory allocation failed!\n");
                return;
            }
            temp->data = pb->data;
            temp->next = NULL;

            // 链接到结果链表
            pc->next = temp;
            pc = temp;

            // 移动到下一个节点
            pb = pb->next;
        }
    }

    // 处理La中剩余的元素
    while (pa)
    {
        // 创建新节点并复制数据
        temp = (LinkList)malloc(sizeof(LNode));
        if (temp == NULL)
        {
            printf("Memory allocation failed!\n");
            return;
        }
        temp->data = pa->data;
        temp->next = NULL;

        // 链接到结果链表
        pc->next = temp;
        pc = temp;

        // 移动到下一个节点
        pa = pa->next;
    }

    // 处理Lb中剩余的元素
    while (pb)
    {
        // 创建新节点并复制数据
        temp = (LinkList)malloc(sizeof(LNode));
        if (temp == NULL)
        {
            printf("Memory allocation failed!\n");
            return;
        }
        temp->data = pb->data;
        temp->next = NULL;

        // 链接到结果链表
        pc->next = temp;
        pc = temp;

        // 移动到下一个节点
        pb = pb->next;
    }
}

/*打印链表内容*/
void PrintList(LinkList L)
{
    LinkList p = L->next; // p指向第一个节点

    // 遍历链表，打印每个节点的数据
    while (p)
    {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

/*销毁链表，释放所有节点的内存*/
void DestroyList(LinkList L)
{
    LinkList p = L;

    // 遍历链表，释放每个节点的内存
    while (p)
    {
        LinkList q = p;
        p = p->next;
        free(q);
    }
}

int main()
{
    // 创建链表
    LinkList A = CreateList();
    LinkList B = CreateList();

    // 初始化链表A
    ListInsert(A, 1, 1);
    ListInsert(A, 2, 3);
    ListInsert(A, 3, 5);
    ListInsert(A, 4, 7);

    // 初始化链表B
    ListInsert(B, 1, 2);
    ListInsert(B, 2, 3);
    ListInsert(B, 3, 6);
    ListInsert(B, 4, 8);

    // 打印原始链表
    printf("Set A: ");
    PrintList(A);
    printf("Set B: ");
    PrintList(B);

    // 测试并集操作
    Union(A, B);
    printf("A Union B: ");
    PrintList(A);

    // 重新创建链表用于测试合并操作
    LinkList A2 = CreateList();
    LinkList B2 = CreateList();
    LinkList C;

    // 初始化链表A2
    ListInsert(A2, 1, 1);
    ListInsert(A2, 2, 3);
    ListInsert(A2, 3, 5);
    ListInsert(A2, 4, 7);

    // 初始化链表B2
    ListInsert(B2, 1, 2);
    ListInsert(B2, 2, 3);
    ListInsert(B2, 3, 6);
    ListInsert(B2, 4, 8);

    // 测试合并操作
    MergeList(A2, B2, &C);
    printf("Merged sorted list C: ");
    PrintList(C);

    // 释放内存
    DestroyList(A);
    DestroyList(C);

    return 0;
}