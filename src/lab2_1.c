
#include <stdio.h>

// This function adds numbers from 1 up to n
int sum_to_n(int n) {
    int sum = 0; // start with nothing
    for (int i = 1; i <= n; i++) { // count from 1 to n
        sum = sum + i; // keep adding
    }
    return sum; // give the total back
}

int main(void) {
    int n;

    printf("Enter a positive integer n: ");
    scanf("%d", &n);

    if (n < 1) {
        printf("Error! You must enter a positive number.\n");
    } else {
        int result = sum_to_n(n); // ask helper for total
        printf("The sum from 1 to %d is %d\n", n, result);
    }

    return 0;
}
