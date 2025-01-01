/*编写函数fun，其功能是：实现两个字符串的连接(不要使用库函数strcat)，即把p2所指的字符串连接到p1所指的字符串的后面。

例如，分别输入下面两个字符串：

FirstString－－

SecondString

程序输出：

FirstString－－SecondString

函数接口定义：
void fun(char p1[], char p2[])
其中 p1和p2代表2个字符串。

裁判测试程序样例：

void fun(char p1[], char p2[])；
int main()
{
    char s1[80], s2[40] ;


    scanf("%s%s", s1, s2) ;

    fun(s1, s2) ;

    printf("%s", s1) ；

    return 0；

}


/* 请在这里填写答案
输入样例：
FirstString－－
SecondString
输出样例：
FirstString－－SecondString */
/*void fun(char p1[], char p2[]){
    int i, j;
    for( i = 0; p1[i] != '\0'; i++);
    for( j = 0; p2[j] != '\0'; j++){
        p1[i+j] = p2[j];
    }
    p1[i+j] = '\0';
}
*/