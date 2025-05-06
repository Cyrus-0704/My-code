// 数组法

#include <stdio.h>
#include <stdlib.h>

// 定义最大容量
#define MAXSIZE 100

// 定义元素类型为整型
typedef int ElemType;

// 定义顺序表结构
typedef struct
{
    ElemType data[MAXSIZE]; // 存储元素的数组
    int length;             // 当前长度
} SqList;

/*判断顺序表是否为空*/
int ListEmpty(SqList *L)
{
    return L->length == 0;
}

/* 获取顺序表的长度*/
int ListLength(SqList *L)
{
    return L->length;
}

/*
获取顺序表中指定位置的元素
L 顺序表指针
i 位置（从1开始）
e 用于存储获取到的元素
*/
int GetElem(SqList *L, int i, ElemType *e)
{
    // 检查位置有效性
    if (i < 1 || i > L->length)
        return 0;
    // 获取元素
    *e = L->data[i - 1];
    return 1;
}

/*比较两个元素是否相等*/
int equal(ElemType a, ElemType b)
{
    return a == b;
}

/*
查找元素在顺序表中的位置
e 要查找的元素
compare 比较函数指针
元素的位置，0表示未找到
*/
int LocateElem(SqList *L, ElemType e, int (*compare)(ElemType, ElemType))
{
    // 遍历顺序表，查找元素
    for (int i = 0; i < L->length; i++)
    {
        if (compare(L->data[i], e))
            return i + 1; // 返回位置（从1开始）
    }
    return 0; // 未找到
}

/*
在顺序表中插入元素
L 顺序表指针
i 插入位置（从1开始）
e 要插入的元素
*/
int ListInsert(SqList *L, int i, ElemType e)
{
    // 检查位置是否有效，以及顺序表是否已满
    if (i < 1 || i > L->length + 1 || L->length == MAXSIZE)
        return 0;

    // 将插入位置及之后的元素后移一位
    for (int j = L->length - 1; j >= i - 1; j--)
    {
        L->data[j + 1] = L->data[j];
    }

    // 插入元素
    L->data[i - 1] = e;
    L->length++; // 长度加1
    return 1;
}

/*
求两个顺序表的并集
将Lb中不在La中的元素插入到La中
*/
void Union(SqList *La, SqList Lb)
{
    int La_len = ListLength(La);
    int Lb_len = ListLength(&Lb);
    ElemType e;

    // 遍历Lb中的每个元素
    for (int i = 1; i <= Lb_len; i++)
    {
        // 获取Lb中的元素
        GetElem(&Lb, i, &e);
        // 如果不在La中，就插入到La中
        if (!LocateElem(La, e, equal))
        {
            ListInsert(La, ++La_len, e);
        }
    }
}

/*合并两个有序顺序表*/
void MergeList(SqList La, SqList Lb, SqList *Lc)
{
    int i = 0, j = 0, k = 0; // i, j分别指向La, Lb的当前位置，k指向Lc的当前位置
    int La_len = ListLength(&La);
    int Lb_len = ListLength(&Lb);

    // 初始化Lc的长度为0
    Lc->length = 0;

    // 比较两个数组的元素，将较小的元素放入Lc
    while (i < La_len && j < Lb_len)
    {
        if (La.data[i] <= Lb.data[j])
        {
            // La中的元素较小或相等，放入Lc
            Lc->data[k++] = La.data[i++];
            Lc->length++;
        }
        else
        {
            // Lb中的元素较小，放入Lc
            Lc->data[k++] = Lb.data[j++];
            Lc->length++;
        }
    }

    // 处理La中剩余的元素
    while (i < La_len)
    {
        Lc->data[k++] = La.data[i++];
        Lc->length++;
    }

    // 处理Lb中剩余的元素
    while (j < Lb_len)
    {
        Lc->data[k++] = Lb.data[j++];
        Lc->length++;
    }
}

void PrintList(SqList *L)
{
    for (int i = 0; i < L->length; i++)
    {
        printf("%d ", L->data[i]);
    }
    printf("\n");
}

int main()
{
    int init_data = 1; // 0表示不初始化，1表示正常初始化

    // 创建并初始化数组A和B
    SqList A = {{0}, 0}; // 先设置为空
    SqList B = {{0}, 0}; // 先设置为空

    // 使用init_data控制是否初始化
    if (init_data)
    {
        A.data[0] = 1;
        A.data[1] = 3;
        A.data[2] = 5;
        A.data[3] = 7;
        A.length = 4;

        B.data[0] = 2;
        B.data[1] = 3;
        B.data[2] = 6;
        B.data[3] = 8;
        B.length = 4;
    }

    // 打印原始数组
    printf("Set A: ");
    PrintList(&A);
    printf("Set B: ");
    PrintList(&B);

    // 创建数组A的副本用于并集操作
    SqList A_copy = A;

    // 执行并集操作
    Union(&A_copy, B);
    printf("A Union B: ");
    PrintList(&A_copy);

    // 创建数组C用于合并操作
    SqList C;
    C.length = 0;

    // 使用原始的A和B数组执行合并操作
    MergeList(A, B, &C);
    printf("Merged sorted list C: ");
    PrintList(&C);

    return 0;
}