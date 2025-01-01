/*输入1行句子（不多于200个单词，每个单词长度不超过100），只包含字母、空格和逗号。单词由至少一个连续的字母构成，空格和逗号都是单词间的间隔。

试输出第1个最长的单词和第1个最短单词。

提示
如果所有单词长度相同，那么第一个单词既是最长单词也是最短单词。

输入格式:
一行句子。

输出格式:
两行输出：
第1行，第一个最长的单词。
第2行，第一个最短的单词。

输入样例:
在这里给出一组输入。例如：

I am studying Programming language C in Peking University
输出样例:
在这里给出相应的输出。例如：

Programming
I
*/
// 直接遍历
// #include <stdio.h>
// #include <string.h>
// #include <ctype.h>

// int main()
// {
//     char input[201]; // 假设最大长度不超过200字符
//     char word[101], longest[101] = "", shortest[101] = "";
//     int i = 0, wordIndex = 0;

//     fgets(input, sizeof(input), stdin); // 读取输入

//     while (input[i] != '\0')
//     {
//         if (isalpha(input[i]) || input[i] == '-')
//         { // 判断是否为单词的一部分
//             word[wordIndex++] = input[i];
//         }
//         else if (wordIndex > 0)
//         { // 一个单词结束
//             word[wordIndex] = '\0';
//             if (strlen(longest) == 0 || strlen(word) > strlen(longest))
//             {
//                 strcpy(longest, word);
//             }
//             if (strlen(shortest) == 0 || strlen(word) < strlen(shortest))
//             {
//                 strcpy(shortest, word);
//             }
//             wordIndex = 0; // 重置索引
//         }
//         i++;
//     }

//     // 检查最后一个单词
//     if (wordIndex > 0)
//     {
//         word[wordIndex] = '\0';
//         if (strlen(word) > strlen(longest))
//         {
//             strcpy(longest, word);
//         }
//         if (strlen(word) < strlen(shortest) || strlen(shortest) == 0)
//         {
//             strcpy(shortest, word);
//         }
//     }

//     printf("%s\n%s\n", longest, shortest);
//     return 0;
// }

/*用strtok函数分割*/


#include <stdio.h>
#include <string.h>

int main() {
    char input[201], *token;
    char longest[101] = "", shortest[101] = "";

    fgets(input, sizeof(input), stdin); // 输入句子
    token = strtok(input, " ,.!?"); // 分割单词

    while (token != NULL) {
        if (strlen(longest) == 0 || strlen(token) > strlen(longest)) {
            strcpy(longest, token); // 更新最长单词
        }
        if (strlen(shortest) == 0 || strlen(token) < strlen(shortest)) {
            strcpy(shortest, token); // 更新最短单词
        }
        token = strtok(NULL, " ,.!?"); // 处理下一个单词
    }

    printf("%s\n%s\n", longest, shortest); // 输出
    return 0;
}

/*单词分离 + 手动比较


#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char input[201], longest[101] = "", shortest[101] = "";
    char word[101];
    int wordLen = 0;

    fgets(input, sizeof(input), stdin); // 输入句子

    for (int i = 0; input[i] != '\0'; i++) {
        if (isalpha(input[i]) || input[i] == '-') { // 单词字符
            word[wordLen++] = input[i];
        } else if (wordLen > 0) { // 单词结束
            word[wordLen] = '\0';
            if (strlen(longest) == 0 || wordLen > strlen(longest)) {
                strcpy(longest, word);
            }
            if (strlen(shortest) == 0 || wordLen < strlen(shortest)) {
                strcpy(shortest, word);
            }
            wordLen = 0;
        }
    }

    if (wordLen > 0) { // 处理最后一个单词
        word[wordLen] = '\0';
        if (strlen(longest) == 0 || wordLen > strlen(longest)) {
            strcpy(longest, word);
        }
        if (strlen(shortest) == 0 || wordLen < strlen(shortest)) {
            strcpy(shortest, word);
        }
    }

    printf("%s\n%s\n", longest, shortest);
    return 0;
}

*/

/*用数组记录


#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char input[201], words[100][101];
    int wordCount = 0, wordIndex = 0;

    fgets(input, sizeof(input), stdin); // 输入句子

    for (int i = 0; input[i] != '\0'; i++) {
        if (isalpha(input[i]) || input[i] == '-') { // 单词字符
            words[wordCount][wordIndex++] = input[i];
        } else if (wordIndex > 0) { // 单词结束
            words[wordCount][wordIndex] = '\0';
            wordCount++;
            wordIndex = 0;
        }
    }
    if (wordIndex > 0) { // 最后一个单词
        words[wordCount][wordIndex] = '\0';
        wordCount++;
    }

    char *longest = words[0], *shortest = words[0];
    for (int i = 1; i < wordCount; i++) {
        if (strlen(words[i]) > strlen(longest)) {
            longest = words[i];
        }
        if (strlen(words[i]) < strlen(shortest)) {
            shortest = words[i];
        }
    }

    printf("%s\n%s\n", longest, shortest);
    return 0;
}
*/