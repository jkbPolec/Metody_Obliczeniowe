#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Napisz program w języku „C/C++”, demonstrujący zjawisko Rungego w interpolacji
// wielomianowej Lagrange’a, na przykładzie interpolacji funkcji f(x) = 1/(1 + x^3), określonej na
// przedziale [−1, 1]. Zastosuj bazę Lagrange’a do konstrukcji wielomianów interpolacyjnych.
// Porównaj wyniki interpolacji na węzłach równoodległych z wynikami interpolacji na węzłach
// Czebyszewa. Wykonaj wykres interpolowanej funkcji oraz uzyskanych wielomianów
// interpolacyjnych.

#define N 60  // liczba węzłów
#define X_MIN (-1.0)
#define X_MAX 1.0
#define POINTS 1000

double f(double x) {
    return 1.0 / (1 + pow(x, 4));
}

void generate_equispaced_nodes(double *x) {
    for (int i = 0; i < N; i++) {
        x[i] = X_MIN + i * (X_MAX - X_MIN) / (N - 1);
    }
}

void generate_chebyshev_nodes(double *x) {
    for (int i = 0; i < N; i++) {
        x[i] = cos((2.0 * i + 1) * M_PI / (2.0 * N));
    }
}

double lagrange_interpolate(double *x, double *y, double xi) {
    double result = 0.0;
    for (int i = 0; i < N; i++) {
        double term = y[i];
        for (int j = 0; j < N; j++) {
            if (j != i) {
                term *= (xi - x[j]) / (x[i] - x[j]);
            }
        }
        result += term;
    }
    return result;
}

void interpolate_and_save(const char *filename, double *x_nodes, double *y_nodes) {
    FILE *fp = fopen(filename, "w");
    for (int i = 0; i <= POINTS; i++) {
        double xi = X_MIN + i * (X_MAX - X_MIN) / POINTS;
        double yi = lagrange_interpolate(x_nodes, y_nodes, xi);
        fprintf(fp, "%lf %lf\n", xi, yi);
    }
    fclose(fp);
}

void save_original_function(const char *filename) {
    FILE *fp = fopen(filename, "w");
    for (int i = 0; i <= POINTS; i++) {
        double xi = X_MIN + i * (X_MAX - X_MIN) / POINTS;
        fprintf(fp, "%lf %lf\n", xi, f(xi));
    }
    fclose(fp);
}

int main() {
    double x_equi[N], y_equi[N];
    double x_cheb[N], y_cheb[N];

    generate_equispaced_nodes(x_equi);
    for (int i = 0; i < N; i++) y_equi[i] = f(x_equi[i]);

    generate_chebyshev_nodes(x_cheb);
    for (int i = 0; i < N; i++) y_cheb[i] = f(x_cheb[i]);

    save_original_function("original.txt");
    interpolate_and_save("interpol_equi.txt", x_equi, y_equi);
    interpolate_and_save("interpol_cheb.txt", x_cheb, y_cheb);

    printf("Zapisano dane do plików:\n - original.txt\n - interpol_equi.txt\n - interpol_cheb.txt\n");
    printf("Użyj gnuplot lub Excela do narysowania wykresów.\n");

    return 0;
}
