#include <stdio.h>

// This function finds factorial using a loop
long long factorial(int n) {
    long long result = 1; // start with 1
    for (int i = 1; i <= n; i++) { // multiply from 1 up to n
        result = result * i;
    }
    return result;
}

int main(void) {
    int n;

    printf("Enter a non-negative integer n: ");
    scanf("%d", &n);

    if (n < 0) {
        printf("Error! Factorial is not defined for negative numbers.\n");
    } else {
        long long fact = factorial(n); // ask helper for factorial
        printf("%d! = %lld\n", n, fact);
    }

    return 0;
}
