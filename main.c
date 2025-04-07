#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 5

// Alokacja i pomocnicze
void print_vector(long double* v, int n);
void print_matrix(long double A[N][N]);

// Procedura 1: LU dekompozycja z pivotowaniem
void lu_decomposition(long double A[N][N], long double L[N][N], long double U[N][N], long double P[N]);

// Procedura 2: Rozwiązywanie LUx = Pb
void solve_lu(long double L[N][N], long double U[N][N], long double P[N], long double* b, long double* x);

int main() {
    long double A[N][N] = {
        { 5, 4, 3, 2, 1 },
        {10, 8, 7, 6, 5 },
        {-1, 2, -3, 4, -5 },
        { 6, 5, -4, 3, -2 },
        { 1, 2, 3, 4, 5 }
    };

    long double b[N] = { 37, 99, -9, 12, 53 };
    long double L[N][N], U[N][N];
    long double P[N];
    long double x[N];

    // Procedura 1
    lu_decomposition(A, L, U, P);

    // Procedura 2
    solve_lu(L, U, P, b, x);

    // Wynik
    printf("\nRozwiazanie L:\n");
    print_matrix(L);
    printf("\nRozwiazanie U:\n");
    print_matrix(U);
    printf("\nRozwiazanie X:\n");
    print_vector(x, N);
    return 0;
}

void lu_decomposition(long double A[N][N], long double L[N][N], long double U[N][N], long double P[N]) {
    // Inicjalizacja macierzy L i U
    for (int i = 0; i < N; i++) {
        P[i] = (long double)i;
        for (int j = 0; j < N; j++) {
            U[i][j] = A[i][j];
            L[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }

    //Przechodzimy po wszystkich kolumnach, gdzie k to numer kolumny
    for (int k = 0; k < N; k++) {
        long double max = fabsl(U[k][k]);
        int maxRow = k;

        //Szukamy najwieszzego elementu w kolumnie k
        for (int i = k + 1; i < N; i++) {
            if (fabsl(U[i][k]) > max) {
                max = fabsl(U[i][k]);
                maxRow = i;
            }
        }

        // Zamiana wierszy, aby element o największej wartości był na górze
        if (maxRow != k) {
            for (int j = 0; j < N; j++) {
                long double tmp = U[k][j];
                U[k][j] = U[maxRow][j];
                U[maxRow][j] = tmp;

                // Jesli zamieniamy wiersze w U, to zamieniamy tez wiersze w L, jesli wiersze w L maja jakis elementy
                if (j < k) {
                    long double tmpL = L[k][j];
                    L[k][j] = L[maxRow][j];
                    L[maxRow][j] = tmpL;
                }
            }

            // Zamiana wierszy w macierzy permutacji
            long double tmpP = P[k];
            P[k] = P[maxRow];
            P[maxRow] = tmpP;
        }

        // Eliminacja aktualnej kolumny
        for (int i = k + 1; i < N; i++) {
            L[i][k] = U[i][k] / U[k][k];
            for (int j = k; j < N; j++) {
                U[i][j] -= L[i][k] * U[k][j];
            }
        }
    }
}

void solve_lu(long double L[N][N], long double U[N][N], long double P[N], long double* b, long double* x) {
    long double y[N], Pb[N];

    //Jesli byly jakies zamiany wierszy, to musimy je uwzglednic
    for (int i = 0; i < N; i++)
        Pb[i] = b[(int)P[i]];

    // Rozwiązywanie Ly = b
    for (int i = 0; i < N; i++) {
        y[i] = Pb[i];
        for (int j = 0; j < i; j++)
            y[i] -= L[i][j] * y[j];
    }

    // Rozwiązywanie Ux = y
    for (int i = N - 1; i >= 0; i--) {
        x[i] = y[i];
        for (int j = i + 1; j < N; j++)
            x[i] -= U[i][j] * x[j];
        x[i] /= U[i][i];
    }
}

void print_vector(long double* v, int n) {
    for (int i = 0; i < n; i++)
        printf("x[%d] = %.4Lf\n", i, v[i]);
}

void print_matrix(long double A[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%8.4Lf ", A[i][j]);
        printf("\n");
    }
}
