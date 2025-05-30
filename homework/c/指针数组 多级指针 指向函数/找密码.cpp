/*在一个古堡的大门上有5行字符。其中隐藏着打开大门的密码。密码共有4位数字（0到9）。小明发现了一种找密码的方法：最后一行中的字符在第一行字符中出现的总次数是密码的第一个数字，依此类推。输入数据保证每行得到的数字在0到9之间。
请编写一个函数 decode帮助小明找出密码。

函数接口定义：
int decode(char**s);
通过参数s向函数传递5行字符串，函数返回密码值。

裁判测试程序样例：
#include <stdio.h>
#include <stdlib.h>
#define N 100
#define M 5
int decode(char**s);
int main()
{

    char *s[M];
    int i;
    int key;
    for(i=0; i<M; i++){
        s[i] = (char *)malloc((N+1)*sizeof(char));
        gets(s[i]);
    }


    key = decode(s);
    printf("%04d",key);
    for(i=0; i<M; i++){
        free(s[i]);
    }

    return 0;
}
/* 您提交的代码将放置在这里 */
/*输入样例：
1111 
222 
33 
41 
2341 
输出样例： 
4322 */