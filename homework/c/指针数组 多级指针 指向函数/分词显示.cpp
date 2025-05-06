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
// 先定义了变量count用于统计单词的个数，初始化为 0，start指针用于记录每个单词开始的位置，初始化为传入的字符串str的起始位置。 外层while循环（ *str != '\0'）用于遍历整个输入的字符串，只要还没遇到字符串结束标志就继续循环。 在内层第一个while循环（ *str == ' '）中，跳过字符串开头的空格，让str指针指向第一个非空格字符（也就是单词开始的位置）。 接着通过if语句判断当前位置字符是否为\0（即判断是否已经遍历完字符串），如果不是，说明找到了一个单词，此时将当前单词的起始地址（也就是str当前的位置）赋值给pStr数组中对应位置（pStr [count] = str），然后单词个数count加 1。 再通过内层第二个while循环（ * str != '\0' && *str != ' '）继续向后遍历这个单词，直到遇到空格或者字符串结束标志为止，这样就找到了一个完整的单词。 当遇到空格时（通过if(*str == ' ') 判断），将这个空格替换为字符串结束标志\0（通过 *str = '\0'），这样就把当前单词截断了，然后让str指针再向后移动一位（跳过这个已经处理的空格），继续寻找下一个单词。 最后，函数返回统计得到的单词个数count。