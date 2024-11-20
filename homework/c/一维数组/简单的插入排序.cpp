/*本题要求编写程序，将一个给定的整数插到原本有序的整数序列中，使结果序列仍然有序。

输入格式：
输入在第一行先给出非负整数N（<10）；第二行给出N个从小到大排好顺序的整数；第三行给出一个整数X。

输出格式：
在一行内输出将X插入后仍然从小到大有序的整数序列，每个数字后面有一个空格。

输入样例：
5
1 2 4 5 7
3
输出样例：
1 2 3 4 5 7 */
#include <stdio.h>
int main()
{
    int n, m, x;
    scanf("%d", &n);
    if (n >= 10 || n < 0)
    {
        return 0;
    }
    int z[n + 1];               // 含有未知数的数组不可初始化
    for (int i = 0; i < n; i++) // 把输入的数存入数组
    {
        scanf("%d", &m);
        z[i] = m;
    }
    scanf("%d", &x);
    z[n] = x;
    for (int i = 0; i < n; i++)
    {
        if (x < z[0])
        {
            for (int j = n; j >= 0; j--)
            {
                z[j + 1] = z[j];
            }
            z[0] = x;
        }
        else if (x > z[n])
        {
        }
        else
        {
            if (z[n] > z[i] && z[n] < z[i + 1])
            {
                for (int j = n; j > i; j--)
                {
                    z[j + 1] = z[j];
                }
                z[i + 1] = x;
                break;
            }
        }
    }
    for (int i = 0; i < n + 1; i++)
    {
        printf("%d ", z[i]);
    }
    return 0;
}
