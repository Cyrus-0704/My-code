#include <bits/stdc++.h>

using namespace std;

int main()
{
    int n, k;
    cin >> n >> k;
    // vector 是动态数组（一种容器）
    vector<pair<string, int>> items(n); // 存储道具名称和品质
    vector<bool> drawn(n, false);       // 道具是否已抽过

    for (int i = 0; i < n; ++i)
    {
        cin >> items[i].first >> items[i].second;
    }

    vector<int> fragment_rewards = {2000, 1000, 200, 36, 6};
    int total_fragments = 0;

    for (int i = 0; i < k; ++i)
    {
        string name;
        cin >> name;

        for (int j = 0; j < n; ++j)
        {
            if (items[j].first == name)
            {
                if (drawn[j])
                {
                    total_fragments += fragment_rewards[items[j].second - 1];
                }
                else
                {
                    drawn[j] = true;
                }
                break;
            }
        }
    }

    cout << total_fragments << endl;
    return 0;
}
