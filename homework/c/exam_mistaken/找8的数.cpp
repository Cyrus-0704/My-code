/*含8的数字的个数

现代人对数字越来越讲究，都喜欢含有8的数字。现要你编程计算a至b之间的含有数字8的数的个数（比如181,88,8,28这些数都含有数字8，而21,45,49等没有含有数字8）。

输入格式:
在一行中输入两个正整数a和b，用一个空格隔开。0<a<=b<100000。

输出格式:
输出a和b之间含有8的数的个数。包括a和b。

输入样例:
1 30
输出样例:
3
错误代码：运行超时
#include<stdio.h>
int main(){
    int a, b, count = 0;
    scanf("%d %d",&a, &b);
    for(int i = a; i <= b; i++){
        while(i>0){
            if(i%10==8){
                count++;
                continue;
            }
            i/=10;
        }
    }
    printf("%d",count);
    return 0;
}
*/
// 修正代码：仍旧超时
/*#include <stdio.h>
int main()
{
    int a, b, count = 0;
    scanf("%d %d", &a, &b);
    for (int i = a; i <= b; i++)
    {
        int k = i, flag = 0;
        while (i > 0)
        {
            if (k % 10 == 8 && flag == 0)
            {
                count++;
                flag = 1;
            }
            k /= 10;
        }
    }
    printf("%d", count);
    return 0;
}*/

#include <stdio.h>

int main()
{
    int a, b, count = 0;
    scanf("%d %d", &a, &b);

    for (int i = a; i <= b; i++)
    {
        int temp = i;

        // 先进行简单判断，跳过明显不含8或肯定含8的数
        if (temp < 8)
        {
            continue;
        }
        else if (temp >= 80 && temp < 89)
        {
            count++;
            continue;
        }

        // 利用整数除法和取余特性检查是否含8
        while (temp > 0)
        {
            if (temp % 10 == 8 || (temp / 10) % 10 == 8)
            {
                count++;
                break; // break只跳出离他最近的内层循环。
            }
            temp /= 10;
        }
    }

    printf("%d", count);

    return 0;
}
/*以下是更简洁算法 实际上上一个代码更快。
#include <stdio.h>

// 判断一个数是否含有数字8
int containsEight(int num)
{
    while (num > 0)
    {
        if (num % 10 == 8)
        {
            return 1;
        }
        num /= 10;
    }
    return 0;
}

int main()
{
    int a, b;
    scanf("%d %d", &a, &b);

    int count = 0;

    for (int i = a; i <= b; i++)
    {
        if (containsEight(i))
        {
            count++;
        }
    }

    printf("%d\n", count);

    return 0;
}*/