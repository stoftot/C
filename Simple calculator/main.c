#include <stdbool.h>
#include <stdio.h>

int main(void) {
    while (true) {
        double a, b;
        char op;
        printf("Enter two numbers: ");
        scanf("%lf %lf", &a, &b);
        printf("\nEnter operator: ");
        scanf(" %c", &op);

        switch (op) {
            case '+':
                printf("%.2lf + %.2lf = %.2lf", a, b, a+b);
                break;
            case '-':
                printf("%.2lf - %.2lf = %.2lf", a, b, a-b);
                break;
            case '*':
                printf("%.2lf * %.2lf = %.2lf", a, b, a*b);
                break;
            case '/':
                printf("%.2lf / %.2lf = %.2lf", a, b, a/b);
                break;
            default:
                printf("Non valid operator");
                break;
        }
        printf("\n");
    }

    return 0;
}