#include<stdio.h>
int main()
{
    int n;
    scanf("%d",&n);
    if(n <= 0)
    {
        printf("0: 0");
    }
    int a[1000],b[10] = {0};
    for(int i = 0; i < n; i++)
    {
        scanf("%d",&a[i]);
        int temp = a[i];
        while(temp != 0)
        {
            int j = temp % 10;
            b[j]++;
            temp /= 10;
        }
    }
    int maxi = 0;
    for(int i = 0; i < 10; i++)
    {
        if(b[maxi] < b[i])
            maxi = i;
    }
    printf("%d:",b[maxi]);
    for(int i = 0; i < 10; i++)
    {
        if(b[i] == b[maxi])
            printf(" %d",i);
    }
    return 0;
}