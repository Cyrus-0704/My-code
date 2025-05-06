#include <bits/stdc++.h>

using namespace std;

int main()
{
    int n;
    cin >> n; // 输入果子种类数

    // 使用优先队列作为最小堆
    priority_queue<int, vector<int>, greater<int>> minHeap;

    // 输入每堆果子的重量，加入最小堆
    for (int i = 0; i < n; i++)
    {
        int weight;
        cin >> weight;
        minHeap.push(weight);
    }

    int totalCost = 0;

    // 不断从堆中取出最小的两堆果子合并
    while (minHeap.size() > 1)
    {
        int smallest = minHeap.top(); // 取出最小的一堆
        minHeap.pop();
        int secondSmallest = minHeap.top(); // 取出第二小的一堆
        minHeap.pop();

        int cost = smallest + secondSmallest; // 合并体力
        totalCost += cost;                    // 累加总体力耗费

        minHeap.push(cost); // 将合并后的堆重量放回堆中
    }

    cout << totalCost << endl; // 输出最小体力耗费值
    return 0;
}