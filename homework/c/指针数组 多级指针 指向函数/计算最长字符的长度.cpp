/*本题要求实现一个函数，用于计算有n个元素的指针数组s中最长的字符串的长度。

函数接口定义：
int max_len( char *s[], int n );
其中n个字符串存储在s[]中，函数max_len应返回其中最长字符串的长度。

裁判测试程序样例：
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXN 10
#define MAXS 20

int max_len( char *s[], int n );

int main()
{
    int i, n;
    char *string[MAXN] = {NULL};

    scanf("%d", &n);
    for(i = 0; i < n; i++) {
        string[i] = (char *)malloc(sizeof(char)*MAXS);
        scanf("%s", string[i]);
    }
    printf("%d\n", max_len(string, n));

    return 0;
}
输入样例：
4 blue
yellow
red
green
输出样例：
6 */
// int max_len(char *s[], int n)
// {
//     int max = strlen(s[0]);
//     for (int i = 1; i < n; i++)
//     {
//         if (strlen(s[i]) > max)
//         {
//             max = strlen(s[i]);
//         }
//     }
//     return max;
// }