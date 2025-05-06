/*从键盘上输入N个字符串(约定：每个字符串中字符数≤80字节)，对其进行升序排序并输出。

函数接口定义：

void sort_Str(char **p);
裁判测试程序样例：
#include<stdio.h>
#include<string.h>

#define N 5


void sort_Str(char **p);

int main(void)
{
    int i;
    char *pstr[N], str[N][81];

    for (i=0; i<N; i++)
    {
        pstr[i] = str[i];
    }

    for (i=0; i<N; i++)
    {
        gets(pstr[i]);
    }

    sort_Str(pstr);

    for (i=0; i<N; i++)
    {
        printf("%s\n", pstr[i]);
    }

    return 0;
}

/* 请在这里填写答案 */
/*
输入样例：
hello
My
Friend
Are you ok?
help me !
输出样例：
Are you ok?
Friend
My
hello
help me !*/
// void sort_Str(char **p)
// {
//     char *temp;
//     for (int i = 0; i < N - 1; i++)
//     {
//         for (int j = 0; j < N - i - 1; j++)
//         {
//             if (strcmp(p[j], p[j + 1]) > 0)
//             {
//                 temp = p[j];
//                 p[j] = p[j + 1];
//                 p[j + 1] = temp;
//             }
//         }
//     }
// }