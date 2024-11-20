/*程序每次读入一个正3位数，然后输出按位逆序的数字。注意：当输入的数字含有结尾的0时，输出不应带有前导的0。比如输入700，输出应该是7。

输入格式：
每个测试是一个3位的正整数。

输出格式：
输出按位逆序的数。

输入样例：
123
输出样例：
321*/
/*错误的代码：
#include<stdio.h>
int main(){
    int n;
    scanf("%d", &n);
    int z[3];
    for(int i = 0; i < 3; i++){
        z[i] = n % 10;
        n /= 10;
    }
    if(z[0]!=0)
        printf("%d",z[0]);
    for(int i = 1; i < 3; i++){
        printf("%d", z[i]);
    }
    return 0;
}*/
//改正后：
#include <stdio.h>
int main()
{
    int n;
    scanf("%d", &n);
    int z[3];
    for (int i = 0; i < 3; i++)
    {
        z[i] = n % 10;
        n /= 10;
    }
    int a = z[0] * 100 + z[1] * 10 + z[2];
    printf("%d", a);
    return 0;
}