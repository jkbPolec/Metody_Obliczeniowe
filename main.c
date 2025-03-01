#include <stdio.h>
#include <float.h>

void epsFloat(float prec);
void epsDouble(double prec);
void epsLongDouble(long double prec);


int main() {
    epsFloat(1.0f);
    epsDouble(1.0);
    epsLongDouble(1.0L);

    printf("\nWartosci z biblioteki float.h\n");
    printf("Float epsilon: %.65f\n", FLT_EPSILON);
    printf("Double epsilon: %.65lf\n", DBL_EPSILON);
    printf("Long double epsilon: %.65Lf\n", LDBL_EPSILON);


    return 0;
}

void epsFloat(float prec) {
    float ep = prec;
    int mantissa = 0;
    while ((prec + ep) > prec) {
        ep = ep / 2.0f;
        mantissa++;
    }
    mantissa--;
    ep = 2.0f * ep;

    printf("Float epsilon: %.65f\nFloat mantiss: %d\n", ep, mantissa);
}

void epsDouble(double prec) {
    double ep = prec;
    int mantissa = 0;
    while ((prec + ep) > prec) {
        ep = ep / 2.0;
        mantissa++;
    }
    mantissa--;
    ep = 2.0 * ep;

    printf("Double epsilon: %.65lf\nDouble mantiss: %d\n", ep, mantissa);
}

void epsLongDouble(long double prec) {
    long double ep = prec;
    int mantissa = 0;
    while ((prec + ep) > prec) {
        ep = ep / 2.0L;
        mantissa++;
    }
    mantissa--;
    ep = 2.0L * ep;

    printf("Long double epsilon: %.65Lf\nLong double mantiss: %d\n", ep, mantissa);
}