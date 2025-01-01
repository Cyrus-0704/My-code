// 1
// 以下函数的功能是删除字符串s中的所有数字字符。例如，若输入字符串为“abcd1234efg567”，则删除数字字符后的字符串为“abcdefg”。请填空。

// #include <stdio.h>
//     void
//     deleDigits(char *s)
// {
//     int n = 0, i;
//     for (i = 0; s[i]; i++)
//         if (
//             s[i] < '0' || s[i] > '9')
//             s[n++] = s[i];
//     s[n] =
//         '\0';
// }
// int main()
// {
//     char str[80];
//     scanf("%s", str);
//     deleDigits(str);
//     printf("%s\n", str);
//     return 0;
// }

// 2
//  本题要求输入一个字符串S和两个字符A和B，补足程序中缺失的代码部分，使运行程序时可以将字符串S中的字符A替换为字符B。

// #include <stdio.h>
//     int
//     main()
// {
//     char s[50], a, b, *p; // 程序要实现用字符变量b替换字符串s中的字符变量a
//     gets(s);
//     scanf("%c %c", &a, &b);
//     for (
//         int i = 0; i < 50; i++)
//         if (
//             s[i] == a)

//             a = b;
//     puts(s);
//     return 0;
// }

// 3
// 本程序的功能是将字符串a的所有字符传送到字符串b中，要求每传送三个字符后再存放一个空格。

// #include <stdio.h>
//     int
//     main()
// {
//     int i, k = 0;
//     char a[80], b[80], *p;
//     p = a;
//     gets(p);
//     while (*p)
//     {
//         for (i = 1;
//              i <= 3 && *p; p++, k++, i++)
//             b[k] = *p;
//         if (
//             i == 3 && *p)
//         {
//             b[k] = ' ';
//             k++;
//         }
//     }
//     b[k] = '\0';
//     puts(b);
//     return 0;
// }

// 4
// 本题目要求写一个函数 mystrcmp实现字符串比较，相等输出 0，不等输出其差值，在主函数输出比较结果。

// #include <stdio.h>
// #define N 20
//     int
//     mystrcmp(char *s1, char *s2);
// int main()
// {
//     char str1[N], str2[N];

//     gets(str1);

//     gets(str2);

//     printf("compare result = %d\n",
//            mystrcmp(s1, s2));

//     return 0;
// }

// int mystrcmp(char *s1, char *s2)
// {
//     while (*s1 != '\0' && *s2 != '\0')
//     {
//         if
//             *s1 == *s2

//             {
//                 s1++;
//                 s2++;
//             }
//         else
//             return *s1 - *s2
//     }
//     while (*s1 != '\0')
//         return *s1;
//     while (*s2 != '\0')
//         return -*s2;
//     return 0;
// }

// 5
// 使用函数实现字符串复制，将输入字符串t中的全部字符复制到字符串s中。

//     输入样例 : happy
//     输出样例 : happy
// #include <stdio.h>
// #define MAXS 80
// void  cpy(char *s, char *t);

// int main()
// {
//     char s[MAXS], t[MAXS];
//     scanf("%s", t);
//     cpy(s, t);
//     printf("%s\n", s);
// }

// void cpy(char *s, char *t)
// {
//     while
//         *t != '\0'
//         {
//             *(s++) = *(t++);
//         }
//     *s = '\0';
// }

// 6
// 程序的功能是从键盘读入一个不超过80个字符的字符串(包括数字、非数字字符、空格等，如：a123x456 17960? 302tab5876)，将其中连续的数字作为一个整数，依次存放到一数组a中，例如针对刚才的例子，则123放在a[0]中，17960放在a[2]中……统计共有多少个整数，假定连续的数字值在int表示范围内。

// #include <stdio.h>
// #include <string.h>
// int main(void)
// {
//     char str[80], *pstr;
//     int i, j, k, m, e10, digit, ndigit, a[80], *pa;

//     gets(str);
//     pstr = str; /* 字符指针pstr置于数组str首地址 */

//     pa = a;
//     /* 指针pa置于a数组首地址 */
//     ndigit = 0; /* ndigit代表有多少个整数 */
//     i = 0;      /* 代表字符串中第n个字符 */
//     j = 0;      /* 代表连续数字的位数 */
//     while (1)
//     {
//         if ((*(pstr + i) >= '0') && (*(pstr + i) <= '9'))
//             j++;
//         else
//         {
//             if (j > 0)
//             {

//                 digit = (*(pstr + i - 1) - '0');
//                 /* 将个位数赋予digit */
//                 k = 1;
//                 while (k < j)
//                 {
//                     e10 = 1;
//                     for (m = 1; m <= k; m++)
//                         e10 = e10 * 10; /* e10代表该位数所应乘的因子 */
//                     digit = digit + (*(pstr + i - 1 - k) - '0') * e10;
//                     k++;
//                 }

//                 *pa = digit;
//                 /* 将数值赋予数组a */
//                 ndigit++;
//                 pa++;
//                 j = 0;
//             }
//         }
//         if ('\0' == *(pstr + i))
//             break;
//         i++;
//     }

//     printf("There are %d numbers in this line.They are:\n", ndigit);
//     for (j = 0; j < ndigit; j++) /* 显示数据 */
//         printf("%d ", a[j]);
//     printf("\n");
//     return 0;
// }

// 7
// 输入某班学生某门课的成绩（最多不超过40人，具体人数由用户从键盘输入），补全代码统计不及格的人数。（请不要填写多余的空格）

// #include <stdio.h>
// #define N 40
//     int
//     GetFailNum(int score[], int n);

// int main()
// {
//     int i, n, fail, score[N];
//     scanf("%d", &n);
//     for (i = 0; i < n; i++)
//     {
//         scanf("%d", &score[i]);
//     }
//     fail =
//         GetFailNum(score[N], n);
//     printf("Fail Students=%d", fail);
//     return 0;
// }
// int GetFailNum(int score[], int n)
// {
//     int i, count = 0;
//     for (i = 0; i < n; i++)
//     {
//         if (
//             score[i] < 60)
//             count++;
//     }

//     return count;
// }

// 8
// 输入一行字符，统计其中有多少个单词。假设单词之间以空格分开。
// 提示：当前被检验字符不是空格，而前一个被检验字符是空格，则表示有新单词出现。
// 根据上述功能，补全如下代码（请不要填写多余的空格）。

// #include <stdio.h>
// #define N 20
// int CountWords(char str[]);
// int main()
// {
//     char str[20];

//     fgets(str, sizeof(str), stdin);

//     printf("Numbers of words=%d\n",
//         CountWords(str));
//     return 0;
// }
// int CountWords(char str[])
// {
//     int i, num;
//     num = (str[0] != ' ') ? 1 : 0;
//     for (i = 1;
//         str[i] != '\0'; i++)
//     {
//         if (str[i] != '\0' &&
//             str[i] == ' ')
//         {
//             num++;
//         }
//     }
//     return num;
// }

// 9
// 以下为实现了折半查找的程序，请填空。注：折半查找算法要求数组必须是已排序的。

// #include <stdio.h>
// int binary_search(int arr[], int size, int target)
// {
//     int left = 0;
//     int right = size - 1;
//     int mid;
//     int found = -1;
//     while (left <= right)
//     {

//         mid = left + (right - left) / 2;
//         if (arr[mid] == target)
//         {

//             return mid;
//             break;
//         }
//         else if (arr[mid] < target)
//         {

//             right = mid - 1;
//         }
//         else
//         {

//             left = mid + 1;
//         }
//     }
//     return found;
// }

// int main()
// {
//     int arr[] = {2, 3, 4, 10, 40, 56, 70, 90, 96, 101};
//     int target;
//     int result;
//     scanf("%d", &target);
//     result = binary_search(arr, 10, target);
//     if (result == -1)
//     {
//         printf("Not Found\n");
//     }
//     else
//     {
//         printf("arr[%d]:%d\n", result, arr[result]);
//     }
//     return 0;
// }

//

// 11本题查找二维数组中的最大值及其行列下标

// 输入：3 5 -1 4
//     7 -32 0 9
//     -102 45 78 4
// 输出： max = 78 maxr = 2 maxc = 2

// #include <stdio.h>
// #define ROW 3
// #define COL 4
//     int
//     FindMaxbyCol(int *q, int row, int col, int *maxRow, int *maxCcol);
// int main(void)
// {
//     int a[ROW][COL];
//     int max, maxr, maxc;
//     int i, j;

//     for (i = 0; i < ROW; i++)
//     {
//         for (j = 0; j < COL; j++)
//             scanf("%d", &a[i][j]);
//     }

//     max = FindMaxbyCol(&a[0][0], ROW, COL, &maxr, &maxc);

//     printf("max=%d  maxr=%d   maxc=%d\n", max, maxr, maxc);
//     return 0;
// }
// int FindMaxbyCol(int *q, int row, int col, int *maxRow, int *maxCcol)
// {
//     int i, j;
//     int max;
//     max = q[0];

//     *maxRow = 0;
//     *maxCcol = 0;
//     for (i = 0; i < row; i++)
//     {
//         for (j = 0; j < col; j++)
//         {
//             if (
//                 *(q + i * col + j) > max)
//             {

//                 max = *(q + i * col + j);

//                 *maxRow = i;
//                 *maxCcol = j;
//             }
//         }
//     }
//     return max;
// }
// 12
// 本题目要求将M行M列方阵进行转置。

// #include <stdio.h>

//     void
//     convert(int a[][3], int n)
// {
//     int i, j, x;
//     for (i = 0; i < n; i++)
//         for (j = 0; j <n;j++)
//         {
//             x = a[i][j];

//             a[i][j] = a[j][i];

//             a[j][i] = x;
//         }
// }

// int main()
// {
//     int a[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, i, j;
//     convert(
//         a, 3);
//     for (i = 0; i < 3; i++)
//     {
//         for (j = 0; j < 3; j++)
//             printf("%d ", a[i][j]);
//         printf("\n");
//     }
//     return 0;
// }