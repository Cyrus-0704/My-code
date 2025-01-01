#include <stdio.h>
#include <math.h>

const double N = 1e-1;

typedef struct
{
    double real;
    double imag;
} ComNum;

void printComNum(ComNum *n)
{
    if (fabs(n->real) < N)
    {
        n->real = 0.0;
    }
    if (fabs(n->imag) < N)
    {
        n->imag = 0.0;
    } // 对接近0的double进行处理
    printf("(");
    printf("%.1lf", n->real);
    if (n->imag >= 0)
    {
        printf("+");
    }
    printf("%.1lfi", n->imag);
    printf(")");
}

void printCom(ComNum *n)
{
    if (fabs(n->real) < N)
    {
        n->real = 0.0;
    }
    if (fabs(n->imag) < N)
    {
        n->imag = 0.0;
    } // 对接近0的double进行处理

    if (n->real != 0.0 && n->imag != 0.0)
    {
        printf("%.1lf", n->real);
        if (n->imag > 0)
        {
            printf("+");
        }
        printf("%.1lfi", n->imag);
    } // 实部虚部皆存在
    else if (n->real == 0.0 && n->imag != 0.0)
    {
        printf("%.1lfi", n->imag);
    } // 仅存在虚部
    else if (n->real != 0.0 && n->imag == 0.0)
    {
        printf("%.1lf", n->real);
    } // 仅存在实部
    else
    {
        printf("0.0");
    } // 实部虚部皆不存在
}

void output(ComNum *n1, ComNum *n2, char op)
{
    ComNum temp;
    if (op == '+')
    {
        temp.real = n1->real + n2->real;
        temp.imag = n1->imag + n2->imag;
    }
    else if (op == '-')
    {
        temp.real = n1->real - n2->real;
        temp.imag = n1->imag - n2->imag;
    }
    else if (op == '*')
    {
        temp.real = n1->real * n2->real - n1->imag * n2->imag;
        temp.imag = n1->real * n2->imag + n1->imag * n2->real;
    }
    else if (op == '/')
    {
        double denom = n2->real * n2->real + n2->imag * n2->imag;
        temp.real = (n1->real * n2->real + n1->imag * n2->imag) / denom;
        temp.imag = (n1->imag * n2->real - n1->real * n2->imag) / denom;
    }

    printComNum(n1);
    printf(" %c ", op);
    printComNum(n2);
    printf(" = ");
    printCom(&temp);
    printf("\n");
}

int main()
{
    ComNum n1, n2;
    scanf("%lf %lf %lf %lf", &n1.real, &n1.imag, &n2.real, &n2.imag);
    output(&n1, &n2, '+');
    output(&n1, &n2, '-');
    output(&n1, &n2, '*');
    output(&n1, &n2, '/');
    return 0;
}