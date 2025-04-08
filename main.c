#include <stdio.h>
#include <stdlib.h>

// Procedura eliminacji współczynników (modyfikuje c i d)
void thomas_algorithm(int n, long double* low, long double* diag, long double* up, long double* b) {
    for (int i = 1; i < n; i++) {
        long double m = low[i-1] / diag[i-1];
        diag[i] -= m * up[i-1];
        b[i] -= m * b[i-1];
    }
}


// Procedura rozwiązania układu (podstawianie wsteczne)
void back_substitution(int n, long double* diag, long double* up, long double* b, long double* x) {
    x[n-1] = b[n-1] / diag[n-1];
    for (int i = n - 2; i >= 0; i--) {
        x[i] = (b[i] - up[i] * x[i+1]) / diag[i];
    }
}

int main() {
    int n = 5;

    long double low[] = {2, 4, -6, -8};     // dolna przekątna (n-1)
    long double diag[] = {100, 200, 300, 200, 100}; // główna przekątna (n)
    long double up[] = {-1, -3, 5, -7};         // górna przekątna (n-1)
    long double b[] = {199, 195, 929, 954, 360}; // wektor b (prawa strona)
    long double x[5]; // rozwiązanie

    thomas_algorithm(n, low, diag, up, b);
    back_substitution(n, diag, up, b, x);

    // Wypisanie wyniku
    printf("Rozwiazanie:\n");
    for (int i = 0; i < n; i++) {
        printf("x[%d] = %Lf\n", i, x[i]);
    }

    return 0;
}
