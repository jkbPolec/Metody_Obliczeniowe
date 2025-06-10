#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> // Dla memcpy
#include <time.h>   // Dla pomiaru czasu

// --- Definicje typów dla czytelności ---
typedef enum { LAASONEN, CRANK_NICOLSON } TimeScheme;
typedef enum { THOMAS, LU_DECOMP } LinearSolver;

// --- Parametry globalne z zadania ---
const double D = 1.0;
const double T_MAX = 0.5;
const double L = 1.0; // Długość pręta/membrany

// --- Funkcje pomocnicze ---

// Funkcja analityczna
double analytical_solution(double x, double t) {
    return 1.0 + exp(-M_PI * M_PI * D * t) * cos(M_PI * x);
}

// Funkcja warunku początkowego
double initial_condition(double x) {
    return 1.0 + cos(M_PI * x);
}

// --- Implementacja solverów liniowych ---

// Algorytm Thomasa dla układów trójdiagonalnych
// a - dolna przekątna, b - główna przekątna, c - górna przekątna
// d - wektor prawej strony (modyfikowany), x - wektor rozwiązania
void thomas_solver(const double *a, const double *b, const double *c, double *d, double *x, int n) {
    // Kopia wektorów, aby nie modyfikować oryginalnych macierzy A
    double *c_prime = (double *)malloc(n * sizeof(double));
    double *d_prime = (double *)malloc(n * sizeof(double));
    if (!c_prime || !d_prime) {
        perror("Błąd alokacji pamięci w Thomasie");
        exit(1);
    }
    memcpy(d_prime, d, n * sizeof(double));

    // Krok 1: Eliminacja w przód
    c_prime[0] = c[0] / b[0];
    d_prime[0] = d[0] / b[0];
    for (int i = 1; i < n; i++) {
        double m = 1.0 / (b[i] - a[i-1] * c_prime[i-1]);
        c_prime[i] = c[i] * m; // c[n-1] nie jest używane, ale liczymy dla spójności
        d_prime[i] = (d[i] - a[i-1] * d_prime[i-1]) * m;
    }

    // Krok 2: Podstawienie wstecz
    x[n-1] = d_prime[n-1];
    for (int i = n - 2; i >= 0; i--) {
        x[i] = d_prime[i] - c_prime[i] * x[i+1];
    }

    free(c_prime);
    free(d_prime);
}

// Dekompozycja LU dla pełnej macierzy (z częściowym wyborem elementu głównego)
// A - macierz do dekompozycji (modyfikowana w miejscu), p - wektor permutacji
// Zwraca: 0 w przypadku sukcesu, 1 w przypadku macierzy osobliwej
int lu_decomposition(double **A, int *p, int n) {
    for (int i = 0; i < n; i++) p[i] = i;

    for (int i = 0; i < n; i++) {
        // Wybór elementu głównego (pivoting)
        double max_val = 0;
        int max_idx = i;
        for (int k = i; k < n; k++) {
            if (fabs(A[k][i]) > max_val) {
                max_val = fabs(A[k][i]);
                max_idx = k;
            }
        }
        if (max_val < 1e-12) return 1; // Macierz osobliwa

        // Zamiana wierszy
        if (max_idx != i) {
            int temp_p = p[i]; p[i] = p[max_idx]; p[max_idx] = temp_p;
            double *temp_row = A[i]; A[i] = A[max_idx]; A[max_idx] = temp_row;
        }

        // Dekompozycja
        for (int j = i + 1; j < n; j++) {
            A[j][i] /= A[i][i];
            for (int k = i + 1; k < n; k++) {
                A[j][k] -= A[j][i] * A[i][k];
            }
        }
    }
    return 0;
}

// Rozwiązanie układu LUx=b
// LU - macierz po dekompozycji, p - wektor permutacji, b - wektor prawej strony, x - wektor rozwiązania
void lu_solve(double **LU, const int *p, const double *b, double *x, int n) {
    // Rozwiązanie Ly = Pb (podstawienie w przód)
    for (int i = 0; i < n; i++) {
        x[i] = b[p[i]];
        for (int j = 0; j < i; j++) {
            x[i] -= LU[i][j] * x[j];
        }
    }

    // Rozwiązanie Ux = y (podstawienie wstecz)
    for (int i = n - 1; i >= 0; i--) {
        for (int j = i + 1; j < n; j++) {
            x[i] -= LU[i][j] * x[j];
        }
        x[i] /= LU[i][i];
    }
}


// --- Główna funkcja symulacji ---
double solve_pde(int Nx, double lambda, TimeScheme scheme, LinearSolver solver, int generate_files) {
    // --- Inicjalizacja siatki i parametrów ---
    double h = L / (Nx - 1);
    double dt = lambda * h * h / D;
    int Nt = (int)(T_MAX / dt);

    if (generate_files) {
        printf("\n--- Uruchomienie symulacji dla: ---\n");
        printf("Schemat czasowy: %s\n", (scheme == LAASONEN) ? "Laasonen" : "Crank-Nicolson");
        printf("Solver liniowy:  %s\n", (solver == THOMAS) ? "Thomas" : "LU dla pełnej macierzy");
        printf("Nx = %d, h = %f\n", Nx, h);
        printf("Nt = %d, dt = %f\n", Nt, dt);
        printf("lambda = %f\n", lambda);
        printf("----------------------------------\n");
    }

    // --- Alokacja pamięci ---
    double *U = (double*)malloc(Nx * sizeof(double));
    double *U_next = (double*)malloc(Nx * sizeof(double));
    double *RHS = (double*)malloc(Nx * sizeof(double));
    if (!U || !U_next || !RHS) {
        perror("Błąd alokacji pamięci dla wektorów U");
        exit(1);
    }

    // --- Warunek początkowy ---
    for (int i = 0; i < Nx; i++) {
        double x = i * h;
        U[i] = initial_condition(x);
    }

    // --- Przygotowanie macierzy A dla układu Ax=b ---
    // Wersja trójdiagonalna (dla Thomasa)
    double *a_diag = (double*)malloc((Nx - 1) * sizeof(double)); // dolna
    double *b_diag = (double*)malloc(Nx * sizeof(double));       // główna
    double *c_diag = (double*)malloc((Nx - 1) * sizeof(double)); // górna
    // Wersja pełna (dla LU)
    double **A_full = NULL;
    int *pivot = NULL;

    if (solver == LU_DECOMP) {
        A_full = (double**)malloc(Nx * sizeof(double*));
        for (int i=0; i<Nx; i++) A_full[i] = (double*)calloc(Nx, sizeof(double));
        pivot = (int*)malloc(Nx * sizeof(int));
    }

    // --- Wypełnianie macierzy A w zależności od schematu ---
    if (scheme == LAASONEN) {
        for(int i = 1; i < Nx - 1; i++) {
            a_diag[i-1] = -lambda;
            b_diag[i] = 1.0 + 2.0 * lambda;
            c_diag[i] = -lambda;
        }
        // Warunki brzegowe ∂U/∂x=0 (dysk. centralna z punktem-widmo)
        b_diag[0] = 1.0 + 2.0 * lambda; c_diag[0] = -2.0 * lambda;
        a_diag[Nx-2] = -2.0 * lambda; b_diag[Nx-1] = 1.0 + 2.0 * lambda;
    } else { // CRANK_NICOLSON
        double l_half = lambda / 2.0;
        for(int i = 1; i < Nx - 1; i++) {
            a_diag[i-1] = -l_half;
            b_diag[i] = 1.0 + lambda;
            c_diag[i] = -l_half;
        }
        // Warunki brzegowe
        b_diag[0] = 1.0 + lambda; c_diag[0] = -lambda;
        a_diag[Nx-2] = -lambda; b_diag[Nx-1] = 1.0 + lambda;
    }

    if (solver == LU_DECOMP) {
        for(int i=0; i<Nx; i++) A_full[i][i] = b_diag[i];
        for(int i=0; i<Nx-1; i++) {
            A_full[i+1][i] = a_diag[i];
            A_full[i][i+1] = c_diag[i];
        }
        if (lu_decomposition(A_full, pivot, Nx) != 0) {
            fprintf(stderr, "Macierz jest osobliwa!\n");
            exit(1);
        }
    }

    // --- Pliki wyjściowe (jeśli wymagane) ---
    FILE *f_sol = NULL, *f_err_t = NULL;
    if (generate_files) {
        f_sol = fopen("solution_snapshots.dat", "w");
        f_err_t = fopen("error_vs_time.dat", "w");
        fprintf(f_sol, "# x U_num(t=0) U_an(t=0) U_num(t=0.1) U_an(t=0.1) U_num(t=0.25) U_an(t=0.25) U_num(t=0.5) U_an(t=0.5)\n");
        fprintf(f_err_t, "# t max_abs_error\n");
    }

    int snapshot_idx = 0;
    double snapshot_times[] = {0.0, 0.1, 0.25, T_MAX};
    double **snapshot_data = NULL;
    if (generate_files) {
        snapshot_data = (double**)malloc(4 * sizeof(double*));
        for(int i=0; i<4; i++) snapshot_data[i] = (double*)malloc(Nx * sizeof(double));
    }

    // --- Główna pętla czasowa ---
    for (int k = 0; k <= Nt; k++) {
        double t = k * dt;

        // Zapisywanie danych do plików
        if (generate_files) {
            if (snapshot_idx < 4 && t >= snapshot_times[snapshot_idx]) {
                 memcpy(snapshot_data[snapshot_idx], U, Nx * sizeof(double));
                 snapshot_idx++;
            }
            double max_err_t = 0.0;
            for (int i = 0; i < Nx; i++) {
                double err = fabs(U[i] - analytical_solution(i * h, t));
                if (err > max_err_t) max_err_t = err;
            }
            fprintf(f_err_t, "%f %e\n", t, max_err_t);
        }

        if (k == Nt) break; // Ostatni krok to tylko pomiar błędu

        // Konstrukcja wektora prawej strony (RHS)
        if (scheme == LAASONEN) {
            memcpy(RHS, U, Nx * sizeof(double));
        } else { // CRANK_NICOLSON
            double l_half = lambda / 2.0;
            // Wnętrze
            for (int i = 1; i < Nx - 1; i++) {
                RHS[i] = l_half * U[i-1] + (1.0 - lambda) * U[i] + l_half * U[i+1];
            }
            // Brzegi (z warunkami ∂U/∂x=0)
            RHS[0] = (1.0 - lambda) * U[0] + lambda * U[1];
            RHS[Nx-1] = lambda * U[Nx-2] + (1.0 - lambda) * U[Nx-1];
        }

        // Rozwiązanie układu liniowego
        if (solver == THOMAS) {
            thomas_solver(a_diag, b_diag, c_diag, RHS, U_next, Nx);
        } else { // LU_DECOMP
            lu_solve(A_full, pivot, RHS, U_next, Nx);
        }

        // Aktualizacja rozwiązania
        memcpy(U, U_next, Nx * sizeof(double));
    }

    // --- Końcowe obliczenie błędu i zapis ---
    double max_error = 0.0;
    for (int i = 0; i < Nx; i++) {
        double x = i * h;
        double error = fabs(U[i] - analytical_solution(x, T_MAX));
        if (error > max_error) {
            max_error = error;
        }
    }

    if (generate_files) {
        for (int i = 0; i < Nx; i++) {
            double x = i * h;
            fprintf(f_sol, "%f ", x);
            for(int j=0; j<4; j++) {
                fprintf(f_sol, "%f %f ", snapshot_data[j][i], analytical_solution(x, snapshot_times[j]));
            }
            fprintf(f_sol, "\n");
        }
        fclose(f_sol);
        fclose(f_err_t);
        for(int i=0; i<4; i++) free(snapshot_data[i]);
        free(snapshot_data);
    }

    // --- Zwolnienie pamięci ---
    free(U); free(U_next); free(RHS);
    free(a_diag); free(b_diag); free(c_diag);
    if (solver == LU_DECOMP) {
        for (int i = 0; i < Nx; i++) free(A_full[i]);
        free(A_full);
        free(pivot);
    }

    return max_error;
}


int main() {
    // =================================================================================
    // CZĘŚĆ 1: Badanie zbieżności - zależność błędu od kroku h
    // =================================================================================
    printf("CZĘŚĆ 1: Badanie zbieżności (max błąd dla t=T_MAX w funkcji h)\n");
    printf("lambda = 1.0 dla wszystkich metod niejawnych\n");
    printf("--------------------------------------------------------------------------------------------------------\n");
    printf("%-5s %-15s %-22s %-22s %-22s %-22s\n", "Nx", "h", "Błąd (Laasonen+Thomas)", "Błąd (Laasonen+LU)", "Błąd (CN+Thomas)", "Błąd (CN+LU)");
    printf("--------------------------------------------------------------------------------------------------------\n");

    FILE *f_err_h = fopen("error_vs_h.dat", "w");
    fprintf(f_err_h, "# h err_laasonen_thomas err_laasonen_lu err_cn_thomas err_cn_lu\n");

    for (int Nx = 11; Nx <= 321; Nx *= 2, Nx--) { // 11, 21, 41, 81, 161, 321
        Nx++;
        double h = L / (Nx - 1);
        double err_lt = solve_pde(Nx, 1.0, LAASONEN, THOMAS, 0);
        double err_ll = solve_pde(Nx, 1.0, LAASONEN, LU_DECOMP, 0);
        double err_ct = solve_pde(Nx, 1.0, CRANK_NICOLSON, THOMAS, 0);
        double err_cl = solve_pde(Nx, 1.0, CRANK_NICOLSON, LU_DECOMP, 0);
        printf("%-5d %-15.10f %-22.10e %-22.10e %-22.10e %-22.10e\n", Nx, h, err_lt, err_ll, err_ct, err_cl);
        fprintf(f_err_h, "%f %e %e %e %e\n", h, err_lt, err_ll, err_ct, err_cl);
    }
    fclose(f_err_h);

    // =================================================================================
    // CZĘŚĆ 2 i 3: Wykresy rozwiązań i błędu w czasie dla wybranej siatki
    // =================================================================================
    printf("\n\nCZĘŚĆ 2 i 3: Generowanie plików do wykresów dla wybranej siatki\n");
    int Nx_fine = 101; // Dobra siatka dla dokładnych wyników
    double lambda_cn = 1.0;

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    solve_pde(Nx_fine, lambda_cn, CRANK_NICOLSON, THOMAS, 1);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Wygenerowano 'solution_snapshots.dat' i 'error_vs_time.dat' dla metody Crank-Nicolson + Thomas.\n");
    printf("Czas obliczeń: %f sekund\n", cpu_time_used);

    start = clock();
    solve_pde(Nx_fine, lambda_cn, CRANK_NICOLSON, LU_DECOMP, 0); // Uruchamiamy tylko dla porównania czasu
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Porównawczy czas obliczeń dla Crank-Nicolson + LU: %f sekund\n", cpu_time_used);

    printf("\nZakończono. Użyj gnuplota lub innego programu do wizualizacji danych z plików:\n");
    printf("- error_vs_h.dat\n");
    printf("- solution_snapshots.dat\n");
    printf("- error_vs_time.dat\n");

    return 0;
}