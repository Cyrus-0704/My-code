/*本题目要求编写程序统计一行字符中单词的个数。所谓“单词”是指连续不含空格的字符串，各单词之间用空格分隔，空格数可以是多个。

输入格式:
输入给出一行字符。

输出格式:
在一行中输出单词个数。

输入样例:
Let's go to room 209.

输出样例:
5

*/
#include <stdio.h>
#include <ctype.h>

int main() {
    char line[1000];
    int count = 0,flag = 0;

    fgets(line, sizeof(line), stdin);

    // 遍历输入的字符行，统计单词个数
    for (int i = 0; line[i]!= '\0'; i++) {
        if (isspace(line[i])) {
            flag = 0;
        } else if (!flag) {
            flag = 1;
            count++;
        }
    }

    printf("%d\n", count);

    return 0;
}