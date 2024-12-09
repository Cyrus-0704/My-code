/*程序的功能是：调用Input函数读入最多80个字符，要求字符串中只保留字母和空格，遇到读满或者回车结束读入字符，空格用于分隔单词。请将字符串中用空格分隔的单词在屏幕上输出来。
要求用指针完成函数中各参数的传递与访问，自定义函数头和函数体中不得出现数组下标形式的表示法。

函数接口定义：
void Input ( char *str );
int Split_Count ( char *str,char **pStr );
裁判测试程序样例：
#include <stdio.h>
/* userCode(<80字符): 自定义函数之原型声明 */
/*
void Input(char *str);
int Split_Count(char *str, char **pStr);

int main(void)
{
    char String[81] = {0}, *pString[45];
    int i = 0, count;

    Input(String);

    count = Split_Count(String, pString);

    printf("%d Words: ", count);
    for (i = 0; i < count - 1; i++)
    {
        printf("%s-", pString[i]);
    }
    printf("%s", pString[count - 1]);
    putchar('\n');
    return 0;
}*/
/* 请在这里填写答案 */
/*
输入样例：
Actions speak louder than words

输出样例：
5 Words : Actions -speak - louder - than - words
输入样例： 12A good &&beginning is half @done

输出样例： 6 Words : A -good - beginning - is - half - done
*/
// #include <ctype.h>

// void Input(char *str)
// {
//     int char_count = 0;
//     char k;
//     while (char_count < 80)
//     {
//         scanf("%c", &k);
//         if (k == '\n')
//         {
//             break;
//         }
//         if (isalpha(k) || k == ' ')
//         {
//             *str = k;
//             str++;
//             char_count++;
//         }
//     }
//     *str = '\0';
// }
// int Split_Count(char *str, char **pStr)
// {
//     int count = 0;
//     char *start = str;
//     while (*str != '\0')
//     {
//         while (*str == ' ')
//         {
//             str++;
//         }
//         if (*str != '\0')
//         {
//             pStr[count] = str;
//             count++;
//             while (*str != '\0' && *str != ' ')
//             {
//                 str++;
//             }
//             if (*str == ' ')
//             {
//                 *str = '\0';
//                 str++;
//             }
//         }
//     }
//     return count;
// }