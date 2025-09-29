#include <stdio.h>
#include <math.h>

// Function to check if n is prime
int is_prime(int n) {
    if (n < 2) return 0; // primes start at 2
    for (int i = 2; i <= (int)sqrt(n); i++) {
        if (n % i == 0) {
            return 0; // found a divisor → not prime
        }
    }
    return 1; // no divisors → prime
}

int main(void) {
    int n;

    printf("Enter an integer n (>= 2): ");
    scanf("%d", &n);

    if (n < 2) {
        printf("Error! n must be at least 2.\n");
    } else {
        printf("Prime numbers up to %d:\n", n);
        for (int i = 2; i <= n; i++) {
            if (is_prime(i)) {
                printf("%d ", i);
            }
        }
        printf("\n");
    }

    return 0;
}
