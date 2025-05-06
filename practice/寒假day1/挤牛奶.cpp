#include <bits/stdc++.h>

using namespace std;

// 定义一个结构体表示时间段
struct Interval
{
    int start, end;
};

int main()
{
    int n;
    cin >> n;

    // 输入所有的时间段
    vector<Interval> intervals(n);
    for (int i = 0; i < n; i++)
    {
        cin >> intervals[i].start >> intervals[i].end;
    }

    // 按开始时间排序
    sort(intervals.begin(), intervals.end(), [](Interval a, Interval b)
        { return a.start < b.start; });

    // 记录最长的连续有人挤奶和无人挤奶的时间
    int longestMilkTime = 0;
    int longestIdleTime = 0;

    // 初始化第一个时间段
    int currentStart = intervals[0].start;
    int currentEnd = intervals[0].end;

    // 遍历所有时间段
    for (int i = 1; i < n; i++)
    {
        if (intervals[i].start <= currentEnd)
        {
            // 当前时间段和之前的时间段重叠或相连
            currentEnd = max(currentEnd, intervals[i].end);
        }
        else
        {
            // 当前时间段和之前的时间段不重叠，计算无人挤奶的时间
            longestMilkTime = max(longestMilkTime, currentEnd - currentStart);
            longestIdleTime = max(longestIdleTime, intervals[i].start - currentEnd);

            // 更新当前时间段
            currentStart = intervals[i].start;
            currentEnd = intervals[i].end;
        }
    }

    // 最后一个时间段的处理
    longestMilkTime = max(longestMilkTime, currentEnd - currentStart);
// 初始：
// currentStart = 300
// currentEnd = 1000
// 第一轮（i = 1）：
// intervals[1] = {700, 1200}，与前一个时间段重叠（700 <= 1000）。
// 更新 currentEnd = max(1000, 1200) = 1200。
// 第二轮（i = 2）：
// intervals[2] = {1500, 2100}，与前一个时间段不重叠（1500 > 1200）。
// 计算：
// 挤奶时间：longestMilkTime = max(0, 1200 - 300) = 900
// 空闲时间：longestIdleTime = max(0, 1500 - 1200) = 300
// 更新：currentStart = 1500，currentEnd = 2100
// 最后处理：
// 最后一个时间段 currentStart = 1500，currentEnd = 2100，计算 挤奶时间：
// longestMilkTime = max(900, 2100 - 1500) = 900

    // 输出结果
    cout << longestMilkTime << " " << longestIdleTime << endl;

    return 0;
}