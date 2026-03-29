#include <stdio.h>
#include <stdlib.h>

void to_base_n(int num, int base) {
    // Validate base
    if (base < 2 || base > 16) {
        printf("Error: Base must be between 2 and 16.\n");
        return;
    }

    char digits[] = "0123456789ABCDEF";  // Characters for bases > 10
    char result[65];                      // Supports up to 64-bit numbers
    int index = 0;

    // Special case: number is 0
    if (num == 0) {
        result[index++] = '0';
    } else {
        int n = num;
        while (n > 0) {
            int remainder = n % base;
            result[index++] = digits[remainder];
            n /= base;
        }
    }

    // Add prefix for octal and hexadecimal
    if (base == 8) {
        printf("0");       // Octal prefix
    } else if (base == 16) {
        printf("0x");      // Hexadecimal prefix
    }

    // Print digits in reverse
    for (int i = index - 1; i >= 0; i--) {
        printf("%c", result[i]);
    }

    printf("\n");
}

int main() {
    int number, base;

    printf("Enter a decimal number: ");
    scanf("%d", &number);

    printf("Enter the target base (2-16): ");
    scanf("%d", &base);

    printf("Number %d in base %d is: ", number, base);
    to_base_n(number, base);

    return 0;
}