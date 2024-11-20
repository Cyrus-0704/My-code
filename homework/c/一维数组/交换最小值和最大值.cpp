/*本题要求编写程序，先将输入的一系列整数中的最小值与第一个数交换，然后将最大值与最后一个数交换，最后输出交换后的序列。

注意：题目保证最大和最小值都是唯一的。

输入格式：
输入在第一行中给出一个正整数N（≤10），第二行给出N个整数，数字间以空格分隔。

输出格式：
在一行中顺序输出交换后的序列，每个整数后跟一个空格。

输入样例：
5
8 2 5 1 4
输出样例：
1 2 5 4 8 */
#include <stdio.h>
int main()
{
    int N;
    int m;
    scanf("%d", &N);
    int z[N];
    for (int i = 0; i < N; i++)
    {
        scanf("%d ", &m);
        z[i] = m;
    }
    int flagi1 = 0;
    for (int i = 1; i < N; i++)
    {
        if (z[i] < z[flagi1])
        {
            flagi1 = i;
        }
    }
    int temp1 = z[0];
    z[0] = z[flagi1];
    z[flagi1] = temp1;
    int flagi2 = 0;
    for (int i = 1; i < N; i++)
    {
        if (z[i] > z[flagi2])
        {
            flagi2 = i;
        }
    }
    int temp2 = z[N - 1];
    z[N - 1] = z[flagi2];
    z[flagi2] = temp2;
    for (int i = 0; i < N; i++)
    {
        printf("%d ", z[i]);
    }
    return 0;
}