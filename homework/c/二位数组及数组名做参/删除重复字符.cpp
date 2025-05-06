/*本题要求编写程序，将给定字符串去掉重复的字符后，按照字符ASCII码顺序从小到大排序后输出。

输入格式：
输入是一个以回车结束的非空字符串（少于80个字符）。

输出格式：
输出去重排序后的结果字符串。

输入样例：
ad2f3adjfeainzzzv
输出样例：
23adefijnvz
*/
#include <stdio.h>
#include <string.h>

void f(char *str)
{
    int i, j, length = strlen(str);
    int flag;

    // 先进行去重操作
    for (i = 0; i < length; i++)
    {
        flag = 1;
        for (j = 0; j < i; j++)
        {
            if (str[j] == str[i])
            {
                flag = 0;
                break;
            }
        }
        if (flag)
        {
            // 进行冒泡排序，将当前非重复字符排好序
            for (j = i + 1; j < length; j++)
            {
                if (str[j] < str[i])
                {
                    char temp = str[i];
                    str[i] = str[j];
                    str[j] = temp;
                }
            }
        }
    }
/*     
    if (flag)
    {
        // 进行选择排序，将当前非重复字符排好序
        int minIndex = i;
        for (j = i + 1; j < length; j++)
        {
            if (str[j] < str[minIndex])
            {
                minIndex = j;
            }
        }
        if (minIndex != i)
        {
            char temp = str[i];
            str[i] = str[minIndex];
            str[minIndex] = temp;
        }
    }
*/
    // 输出去重排序后的字符串
    for (i = 0; i < length; i++)
    {
        if (i == 0 || str[i] != str[i - 1])
        {
            printf("%c", str[i]);
        }
    }
    printf("\n");
}

int main()
{
    char str[80];

    // 读取输入字符串
    fgets(str, sizeof(str), stdin);
    // 如果fgets读取到换行符，将其替换为'\0'，以符合后续处理要求
    if (str[strlen(str) - 1] == '\n')
    {
        str[strlen(str) - 1] = '\0';
    }

    f(str);

    return 0;
}