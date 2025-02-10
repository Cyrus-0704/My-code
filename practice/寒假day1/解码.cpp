#include<iostream>
#include<string>
#include<cctype>

using namespace std;

string change(const string input)
{
    string result = "";
    int i = 0;
    while (i<input.size())
    {
        char ch = input[i];
        result += ch;
        i++;
        if(i<input.size() && isdigit(input[i]))
        {
            int count = input[i] - '0';
            for (int j = 1; j < count;j++)
            {
                result += ch;
            }
            i++;
        }
    }
    return result;
}
int main()
{
    string input;
    cin >> input;
    cout << change(input);
}