/*
反素数是指： 一个素数的逆序数也是素数，例如13是一个素数，13的逆序数31也是一个素数,则称13为反素数。若该素数是回文数（逆序数等于本身，例如11）,则该素数不能称为反素数。现要求编程实现：输入两个正整数n，k，输出k个≥n的反素数。

输入格式:
在一行中输入两个不超过1000的正整数m和k。

输出格式:
分K行输出k个反素数。

输入样例:
11  5
输出样例:
13
17
31
37
71
错误代码：
第一版：
#include<stdio.h>
#include<math.h>
int f(int n){
    for(int i = 2; i < sqrt(n); i++){
        if(n%i==0){
            return 0;
        }
    }
    return 1;
}
int g(int n){
    int z[3]={0}, m = 0;
    for(int i = 0; i < 3; i++){
        if(n>0){
            z[i] = n % 10;
            n /= 10;
        }
    }
    for(int j = 0; j < 3; j++){
        m += z[j]*pow(10,3-j);
    }
    if(f(n) && f(m) && (n != m))
        return 1;
    else
        return 0;
}
int main(){
    int n,k,count = 0;
    scanf("%d %d",&n,&k);
    for(int i = n;;i++){
        if(g(i)){
            printf("%d\n",i);
            count++;
        }
        if(count>k)
            break;
    }
    return 0;
}
第二版：
#include <stdio.h>
#include <math.h>
int f(int n) // 判断素数
{
    for (int i = 2; i < sqrt(n); i++) 应该改为i <= sqrt(n),
    {
        if (n % i == 0)
        {
            return 0;
        }
    }
    return 1;
}
int g(int n) // 计算逆数
{
    int k = 0;
    while (k > 0)  错误的while循环条件，应该是n>0.
    {
        k = k * 10 + n % 10;
        n /= 10;
    }
    return k;
}
int h(int n) // 判断是否为逆素数
{
    if (n != g(n) && f(n) && f(g(n)))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int main()
{
    int n, k, count = 0;
    scanf("%d %d", &n, &k);
    for (int i = n;; i++)
    {
        if (h(i))
        {
            printf("%d\n", i);
            count++;
        }
        if (count > k)
            break;
    }
    return 0;
}
*/
#include <stdio.h>
#include <math.h>
int f(int n) // 判断素数
{
    for (int i = 2; i <= sqrt(n); i++)
    {
        if (n % i == 0)
        {
            return 0;
        }
    }
    return 1;
}
int g(int n) // 计算逆数
{
    int k = 0;
    while (n > 0)
    {
        k = k * 10 + n % 10;
        n /= 10;
    }
    return k;
}
int h(int n) // 判断是否为逆素数
{
    if (n != g(n))
    {
        if (f(n) && f(g(n)))
            return 1;
    }
    return 0;
}
int main()
{
    int n, k, count = 0;
    scanf("%d %d", &n, &k);
    for (int i = n;; i++)
    {
        if (h(i))
        {
            printf("%d\n", i);
            count++;
        }
        if (count >= k)
            break;
    }
    return 0;
}