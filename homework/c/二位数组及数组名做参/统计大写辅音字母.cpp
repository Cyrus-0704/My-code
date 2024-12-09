/*英文辅音字母是除A、E、I、O、U以外的字母。本题要求编写程序，统计给定字符串中大写辅音字母的个数。

输入格式：
输入在一行中给出一个不超过80个字符、并以回车结束的字符串。

输出格式：
输出在一行中给出字符串中大写辅音字母的个数。

输入样例：
HELLO World!
输出样例：
4
*/
#include <stdio.h>
#include <ctype.h>

int main()
{
    char c[81];
    int count = 0;

    if (fgets(c, 81, stdin) != NULL)
    {
        for (int i = 0; c[i] != '\0'; i++)
        { // isupper函数判断是否为大写字母
            if (isupper(c[i]) && c[i] != 'A' && c[i] != 'E' && c[i] != 'I' && c[i] != 'O' && c[i] != 'U')
            {
                count++;
            }
        }
    }

    printf("%d", count);

    return 0;
}