/*本题要求实现一个函数，对给定的一个字符串和两个字符，打印出给定字符串中从与第一个字符匹配的位置开始到与第二个字符匹配的位置之间的所有字符。

函数接口定义：
char *match( char *s, char ch1, char ch2 );
函数match应打印s中从ch1到ch2之间的所有字符，并且返回ch1的地址。

裁判测试程序样例：
#include <stdio.h>

#define MAXS 10

char *match( char *s, char ch1, char ch2 );

int main()
{
    char str[MAXS], ch_start, ch_end, *p;

    scanf("%s\n", str);
    scanf("%c %c", &ch_start, &ch_end);
    p = match(str, ch_start, ch_end);
    printf("%s\n", p);

    return 0;
}

输入样例1：
program
r g
输出样例1：
rog
rogram
输入样例2：
program
z o
输出样例2：(空行)(空行)
输入样例3：
program
g z
输出样例3：
gram
gram
*/
// #include <string.h>
// char *match(char *s, char ch1, char ch2)
// {
//     if (*s == '\0')
//         return NULL;
//     int k1 = -1, k2 = -1;
//     for (int i = 0; s[i] != '\0'; i++)
//     {
//         if (s[i] == ch1 && k1 == -1)
//         {
//             k1 = i;
//         }
//         if (k1 != -1)
//         {
//             printf("%c", s[i]);
//         }
//         if (s[i] == ch2 && k1 != -1)
//         {
//             k2 = i;
//             break;
//         }
//     }
//     printf("\n");
//     if (k1 != -1)
//     {
//         return &s[k1];
//     }
//     return s + strlen(s);
// }