/*本题要求将给定的n个整数从大到小排序后输出。

输入格式：
输入第一行给出一个不超过10的正整数n。第二行给出n个整数，其间以空格分隔。

输出格式：
在一行中输出从大到小有序的数列，相邻数字间有一个空格，行末不得有多余空格。

输入样例：
4
5 1 7 6
输出样例：
7 6 5 1*/
// #include <stdio.h>
// int main()
// {
//     int n, m;
//     scanf("%d", &n);
//     int z[n];
//     for (int i = 0; i < n; i++)
//     {
//         scanf("%d ", &m);
//         z[i] = m;
//     }
//     int temp;
//     for (int i = 0; i < n - 1; i++)
//     {
//         for (int j = 0; j < n - 1 - i; j++)
//         {
//             if (z[j] < z[j + 1])
//             {
//                 temp = z[j];
//                 z[j] = z[j + 1];
//                 z[j + 1] = temp;
//             }
//         }
//     }
//     for (int i = 0; i < n - 1; i++)
//     {
//         printf("%d ", z[i]);
//     }
//     printf("%d", z[n - 1]);
//     return 0;
// }
#include <stdio.h>

int main()
{
    int n;
    scanf("%d", &n);
    int arr[10]; // 定义数组用于存储输入的整数，题目限定n不超过10
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    // 选择排序实现从大到小排序
    for (int i = 0; i < n - 1; i++)
    {
        int max_index = i;
        for (int j = i + 1; j < n; j++)
        {
            if (arr[j] > arr[max_index])
            {
                max_index = j;
            }
        }
        if (max_index != i)
        {
            int temp = arr[i];
            arr[i] = arr[max_index];
            arr[max_index] = temp;
        }
    }

    // 输出排序后的数组
    for (int i = 0; i < n; i++)
    {
        if (i != 0)
        {
            printf(" ");
        }
        printf("%d", arr[i]);
    }
    printf("\n");

    return 0;
}