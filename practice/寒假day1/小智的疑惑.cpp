#include<bits/stdc++.h>

using namespace std;

int main()
{
    string s, t = "chuanzhi";
    cin >> s;
    int count = 0,pos = 0;
    while ((pos = s.find(t,pos))!= string::npos)
    {
        count++;
        pos++;
    }
    cout << count << endl;
    return 0;
}