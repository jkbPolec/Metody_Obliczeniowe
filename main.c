#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265358979323846
#define N 50 // liczba kroków (różne h)
#define START 0.0
#define END (PI / 2.0)

// Wybierz precyzję: double albo long double
#define TYPE long double
#define COS(x) cosl(x)
#define FABS(x) fabsl(x)

TYPE f(TYPE x) {
    return COS(x);
}

TYPE df_exact(TYPE x) {
    return -sin(x);
}

TYPE forward_diff(TYPE (*f)(TYPE), TYPE x, TYPE h) {
    return (f(x + h) - f(x)) / h;
}

TYPE backward_diff(TYPE (*f)(TYPE), TYPE x, TYPE h) {
    return (f(x) - f(x - h)) / h;
}

TYPE central_diff(TYPE (*f)(TYPE), TYPE x, TYPE h) {
    return (f(x + h) - f(x - h)) / (2 * h);
}

TYPE central_diff_3pt(TYPE (*f)(TYPE), TYPE x, TYPE h) {
    return (f(x - h) - 8*f(x - h/2.0) + 8*f(x + h/2.0) - f(x + h)) / (6.0 * h);
}

int main() {
    FILE *fp = fopen("blad_roznicowy.csv", "w");
    if (!fp) {
        printf("Nie można otworzyć pliku do zapisu.\n");
        return 1;
    }

    fprintf(fp, "h,blad_forward,blad_backward,blad_central,blad_central3\n");

    for (int i = 1; i <= N; i++) {
        TYPE h = (TYPE)powl(2.0, -i);  // h: 2^-1, 2^-2, ..., 2^-N
        TYPE x = PI / 4; // środek przedziału

        TYPE df_f = forward_diff(f, x, h);
        TYPE df_b = backward_diff(f, x, h);
        TYPE df_c = central_diff(f, x, h);

        TYPE exact = df_exact(x);

        TYPE err_f = FABS(df_f - exact);
        TYPE err_b = FABS(df_b - exact);
        TYPE err_c = FABS(df_c - exact);

        TYPE df_3c = central_diff_3pt(f, x, h);
        TYPE err_3c = FABS(df_3c - exact);

        fprintf(fp, "%.20Lf,%.20Lf,%.20Lf,%.20Lf,%.20Lf\n", h, err_f, err_b, err_c, err_3c);
    }

    fclose(fp);
    printf("Zapisano dane do pliku blad_roznicowy.csv\n");

    return 0;
}
