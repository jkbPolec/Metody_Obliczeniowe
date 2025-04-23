#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define N 5
#define MAX_ITER 1000
#define EPSILON 1e-6
#define OMEGA 0.5

void print_vector(double x[N]) {
    for (int i = 0; i < N; i++)
        printf("%.6f ", x[i]);
    printf("\n");
}

double norm(double x1[N], double x2[N]) {
    double sum = 0;
    for (int i = 0; i < N; i++)
        sum += (x1[i] - x2[i]) * (x1[i] - x2[i]);
    return sqrt(sum);
}

double residuum(int A[N][N], double x[N], int b[N]) {
    double res = 0;
    for (int i = 0; i < N; i++) {
        double ax = 0;
        for (int j = 0; j < N; j++)
            ax += A[i][j] * x[j];
        res += (b[i] - ax) * (b[i] - ax);
    }
    return sqrt(res);
}

void jacobi(int A[N][N], int b[N], double x0[N]) {
    double x[N], x_prev[N];
    for (int i = 0; i < N; i++) x_prev[i] = x0[i];

    printf("\n--- Metoda Jacobiego ---\n");

    for (int k = 0; k < MAX_ITER; k++) {
        for (int i = 0; i < N; i++) {
            double sigma = 0;
            for (int j = 0; j < N; j++)
                if (j != i)
                    sigma += A[i][j] * x_prev[j];
            x[i] = (b[i] - sigma) / A[i][i];
        }

        printf("Iteracja %d: ", k + 1);
        print_vector(x);
        printf("Residuum: %.6f, Blad: %.6f\n", residuum(A, x, b), norm(x, x_prev));

        if (norm(x, x_prev) < EPSILON || residuum(A, x, b) < EPSILON)
            break;

        for (int i = 0; i < N; i++)
            x_prev[i] = x[i];
    }
}

void gauss_seidel(int A[N][N], int b[N], double x0[N]) {
    double x[N];
    for (int i = 0; i < N; i++) x[i] = x0[i];

    printf("\n--- Metoda Gaussa-Seidela ---\n");

    for (int k = 0; k < MAX_ITER; k++) {
        double x_prev[N];
        for (int i = 0; i < N; i++) x_prev[i] = x[i];

        for (int i = 0; i < N; i++) {
            double sigma = 0;
            for (int j = 0; j < N; j++)
                if (j != i)
                    sigma += A[i][j] * x[j];
            x[i] = (b[i] - sigma) / A[i][i];
        }

        printf("Iteracja %d: ", k + 1);
        print_vector(x);
        printf("Residuum: %.6f, Blad: %.6f\n", residuum(A, x, b), norm(x, x_prev));

        if (norm(x, x_prev) < EPSILON || residuum(A, x, b) < EPSILON)
            break;
    }
}

void sor(int A[N][N], int b[N], double x0[N], double omega) {
    double x[N];
    for (int i = 0; i < N; i++) x[i] = x0[i];

    printf("\n--- Metoda SOR (ω = %.2f) ---\n", omega);

    for (int k = 0; k < MAX_ITER; k++) {
        double x_prev[N];
        for (int i = 0; i < N; i++) x_prev[i] = x[i];

        for (int i = 0; i < N; i++) {
            double sigma = 0;
            for (int j = 0; j < N; j++)
                if (j != i)
                    sigma += A[i][j] * x[j];
            double new_x = (b[i] - sigma) / A[i][i];
            x[i] = (1 - omega) * x[i] + omega * new_x;
        }

        printf("Iteracja %d: ", k + 1);
        print_vector(x);
        printf("Residuum: %.6f, Blad: %.6f\n", residuum(A, x, b), norm(x, x_prev));

        if (norm(x, x_prev) < EPSILON || residuum(A, x, b) < EPSILON)
            break;
    }
}

int main() {
    int A[N][N] = {
        {50, 5, 4, 3, 2},
        {1, 40, 1, 2, 3},
        {4, 5, 30, -5, -4},
        {-3, -2, -1, 20, 0},
        {1, 2, 3, 4, 30}
    };

    int b[N] = {140, 67, 62, 89, 153};
    double x0[N] = {6, 6, 6, 6, 6};

    jacobi(A, b, x0);
    gauss_seidel(A, b, x0);
    sor(A, b, x0, OMEGA);

    return 0;
}
