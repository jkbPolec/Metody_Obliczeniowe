#include <stdio.h>
#include <math.h>

#define EPSILON 1e-9L
#define MAX_ITER 100

// Funkcje układu równań nieliniowych
void computeFunctions(long double x, long double y, long double z, long double F[3]) {
    F[0] = x*x + y*y + z*z - 4;
    F[1] = x*x + (y*y)/2 - 1;
    F[2] = x*y - 0.5;
}

// Macierz Jacobiego
void computeJacobian(long double x, long double y, long double z, long double J[3][3]) {
    J[0][0] = 2*x;  J[0][1] = 2*y;  J[0][2] = 2*z;
    J[1][0] = 2*x;  J[1][1] = y;    J[1][2] = 0;
    J[2][0] = y;    J[2][1] = x;    J[2][2] = 0;
}

void solveLinearSystem(long double J[3][3], long double F[3], long double dX[3]) {
    int i, j, k;
    long double ratio;
    long double A[3][4];

    // Tworzenie rozszerzonej macierzy
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            A[i][j] = J[i][j];
        }
        A[i][3] = -F[i];
    }

    // Eliminacja Gaussa
    for (i = 0; i < 3; i++) {
        for (j = i + 1; j < 3; j++) {
            ratio = A[j][i] / A[i][i];
            for (k = 0; k < 4; k++) {
                A[j][k] -= ratio * A[i][k];
            }
        }
    }

    // Rozwiązanie macierzy górnotrójkątnej
    for (i = 2; i >= 0; i--) {
        dX[i] = A[i][3];
        for (j = i + 1; j < 3; j++) {
            dX[i] -= A[i][j] * dX[j];
        }
        dX[i] /= A[i][i];
    }
}

// Norma wektora
long double vectorNorm(long double v[3]) {
    return sqrtl(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

int main() {
    long double x = 1.0L, y = 1.0L, z = 1.0L; // Początkowe przybliżenie
    long double F[3], J[3][3], dX[3];
    int iter;

    printf("Iteracja | x | y | z | Estymator bledu | Norma residuum\n");
    printf("-----------------------------------------------------------\n");

    for (iter = 0; iter < MAX_ITER; iter++) {
        computeFunctions(x, y, z, F);
        computeJacobian(x, y, z, J);
        solveLinearSystem(J, F, dX);

        long double norm_dX = vectorNorm(dX);
        long double norm_F = vectorNorm(F);

        // Wypisanie wyników pośrednich
        printf("%3d | %.9Lf | %.9Lf | %.9Lf | %.9Lf | %.9Lf\n", iter, x, y, z, norm_dX, norm_F);

        // Warunki stopu
        if (norm_dX < EPSILON || norm_F < EPSILON) {
            break;
        }

        x += dX[0];
        y += dX[1];
        z += dX[2];
    }

    printf("\nRozwiazanie: x = %.9Lf, y = %.9Lf, z = %.9Lf\n", x, y, z);
    return 0;
}