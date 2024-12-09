/*本题要求编写程序，将给定n×n方阵中的每个元素循环向右移m个位置，即将第0、1、⋯、n−1列变换为第n−m、n−m+1、⋯、n−1、0、1、⋯、n−m−1列。

输入格式：
输入第一行给出两个正整数m和n（1≤n≤6）。接下来一共n行，每行n个整数，表示一个n阶的方阵。

输出格式：
按照输入格式输出移动后的方阵：即输出n行，每行n个整数，每个整数后输出一个空格。

输入样例：
2 3
1 2 3
4 5 6
7 8 9
输出样例：
2 3 1
5 6 4
8 9 7 */
/*第一版：
#include <stdio.h>
int main()
{
    int m, n, k;
    scanf("%d %d", &m, &n);
    int z[n][n];
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            scanf("%d", &k);
            z[i][j] = k;
        }
    }
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < m; j++)
        {
            int temp = z[i + 1][j];
            z[i + 1][j] = z[i][j];
            z[i][j] = temp;
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", z[i][j]);
        }
        printf("\n");
    }
    return 0;
}*/
#include <stdio.h>
int main()
{
    int m, n, k;
    scanf("%d %d", &m, &n);
    int z[n][n];
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            scanf("%d", &k);
            z[i][j] = k;
        }
    }
    for (int l = 0; l < m; l++)
    {
        int temp[n];
        for (int i = 0; i < n; i++)
        {
            temp[i] = z[i][n - 1];
            for (int j = n - 1; j > 0; j--)
            {
                z[i][j] = z[i][j - 1];
            }
            z[i][0] = temp[i];
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", z[i][j]);
        }
        printf("\n");
    }
    return 0;
}