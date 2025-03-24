#include <stdio.h>
#include <math.h>

#define EPSILON 1e-6  // Kryterium dokładności
#define MAX_ITER 100  // Maksymalna liczba iteracji

// Funkcje układu równań nieliniowych
void computeFunctions(double x, double y, double z, double F[3]) {
    F[0] = x*x + y*y + z*z - 4;
    F[1] = x*x + (y*y)/2 - 1;
    F[2] = x*y - 0.5;
}

// Macierz Jacobiego
void computeJacobian(double x, double y, double z, double J[3][3]) {
    J[0][0] = 2*x;  J[0][1] = 2*y;  J[0][2] = 2*z;
    J[1][0] = 2*x;  J[1][1] = y;    J[1][2] = 0;
    J[2][0] = y;    J[2][1] = x;    J[2][2] = 0;
}

// Rozwiązywanie układu J * dX = -F metodą eliminacji Gaussa
void solveLinearSystem(double J[3][3], double F[3], double dX[3]) {
    int i, j, k;
    double ratio;
    double A[3][4]; // Rozszerzona macierz (J | -F)

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

    // Rozwiązanie układu (podstawianie wsteczne)
    for (i = 2; i >= 0; i--) {
        dX[i] = A[i][3];
        for (j = i + 1; j < 3; j++) {
            dX[i] -= A[i][j] * dX[j];
        }
        dX[i] /= A[i][i];
    }
}

// Norma wektora (używana do warunków stopu)
double vectorNorm(double v[3]) {
    return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

int main() {
    double x = 1.0, y = 1.0, z = 1.0; // Początkowe przybliżenie
    double F[3], J[3][3], dX[3];
    int iter;

    printf("Iteracja | x | y | z | Norma korekty | Norma residuum\n");
    printf("-----------------------------------------------------------\n");

    for (iter = 0; iter < MAX_ITER; iter++) {
        computeFunctions(x, y, z, F);
        computeJacobian(x, y, z, J);
        solveLinearSystem(J, F, dX);

        double norm_dX = vectorNorm(dX);
        double norm_F = vectorNorm(F);

        // Wypisanie wyników pośrednich
        printf("%3d | %.6f | %.6f | %.6f | %.6e | %.6e\n", iter, x, y, z, norm_dX, norm_F);

        // Warunki stopu
        if (norm_dX < EPSILON || norm_F < EPSILON) {
            break;
        }

        // Aktualizacja rozwiązania
        x += dX[0];
        y += dX[1];
        z += dX[2];
    }

    printf("\nRozwiązanie: x = %.6f, y = %.6f, z = %.6f\n", x, y, z);
    return 0;
}
