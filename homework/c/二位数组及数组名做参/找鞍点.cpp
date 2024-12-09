/*一个矩阵元素的“鞍点”是指该位置上的元素值在该行上最大、在该列上最小。

本题要求编写程序，求一个给定的n阶方阵的鞍点。

输入格式：
输入第一行给出一个正整数n（1≤n≤6）。随后n行，每行给出n个整数，其间以空格分隔。

输出格式：
输出在一行中按照“行下标 列下标”（下标从0开始）的格式输出鞍点的位置。如果鞍点不存在，则输出“NONE”。题目保证给出的矩阵至多存在一个鞍点。

输入样例1：
4
1 7 4 1
4 8 3 6
1 6 1 2
0 7 8 9
输出样例1：
2 1
输入样例2：
2
1 7
4 1
输出样例2：
NONE
*/
#include <stdio.h>

int main()
{
    int n;
    scanf("%d", &n);

    int z[6][6];

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            scanf("%d", &z[i][j]);
        }
    }

    int hang, lie;
    int flag = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int hangmax = 1;
            int liemin = 1;

            // 检查当前元素是否为该行的最大值
            for (int k = 0; k < n; k++)
            {
                if (z[i][k] > z[i][j])
                {
                    hangmax = 0;
                    break;
                }
            }

            // 检查当前元素是否为该列的最小值
            for (int k = 0; k < n; k++)
            {
                if (z[k][j] < z[i][j])
                {
                    liemin = 0;
                    break;
                }
            }

            // 如果当前元素既是行最大又是列最小，则找到鞍点
            if (hangmax && liemin)
            {
                hang = i;
                lie = j;
                flag = 1;
                break;
            }
        }

        // 如果已经找到鞍点，跳出循环
        if (flag == 1)
            break;
    }

    if (flag == 1)
        printf("%d %d\n", hang, lie);
    else
        printf("NONE\n");

    return 0;
}