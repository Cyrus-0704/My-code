#include <cstdio>
#include <queue>
#define int long long
using namespace std;

// 定义两个队列
queue<int> q1, q2;
int to[100005]; // 计数数组，用于记录每种重量的堆出现次数

// 快速读入函数，减少 IO 消耗
void read(int &x)
{
    int f = 1;
    x = 0;
    char s = getchar();
    while (s < '0' || s > '9')
    {
        if (s == '-')
            f = -1;
        s = getchar();
    }
    while (s >= '0' && s <= '9')
    {
        x = x * 10 + s - '0';
        s = getchar();
    }
    x *= f;
}

signed main()
{
    int n;
    read(n); // 读入堆数
    for (int i = 1; i <= n; ++i)
    {
        int a;
        read(a); // 读入每个堆的重量
        to[a]++; // 计数
    }

    // 初始化队列 q1，按堆的重量顺序加入队列
    for (int i = 1; i <= 100000; ++i)
    {
        while (to[i] > 0)
        {
            q1.push(i);
            to[i]--;
        }
    }

    // 计算最小耗费体力
    int ans = 0;
    for (int i = 1; i < n; ++i)
    { // n-1 次合并
        int x, y;

        // 从两个队列中取最小值
        if ((!q1.empty() && (q2.empty() || q1.front() < q2.front())))
        {
            x = q1.front();
            q1.pop();
        }
        else
        {
            x = q2.front();
            q2.pop();
        }

        if ((!q1.empty() && (q2.empty() || q1.front() < q2.front())))
        {
            y = q1.front();
            q1.pop();
        }
        else
        {
            y = q2.front();
            q2.pop();
        }

        // 合并两堆，计算体力并将新堆加入队列 q2
        ans += x + y;
        q2.push(x + y);
    }

    // 输出结果
    printf("%lld\n", ans);
    return 0;
}
