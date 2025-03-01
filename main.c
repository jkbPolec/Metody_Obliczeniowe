#include <stdio.h>
#include <float.h>
#include <math.h>

/*  Temat zadania:
Napisz program w języku „C/C++”, umożliwiający „doświadczalne”
wyznaczenie liczby bitów mantysy oraz tzw. epsilon maszynowego,
tj. najmniejszej liczby ε takiej, że fl(ε + 1) > 1.
Wyznacz epsilon maszynowy dla zmiennych typu float i double i
sprawdź czy da się go wyznaczyć dla zmiennych typu long double.
Sprawdź też ile dokładnych cyfr znaczących posiada epsilon maszynowy.
Aby znaleźć odpowiedź na pytanie jak napisać taki program,
zacznij od wyjaśnienia kwestii jaki jest związek ε z precyzją arytmetyki.
 */

void epsFloat(float prec);
void epsDouble(double prec);
void epsLongDouble(long double prec);
int significantDigits(double epsilon);

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
    printf("Float epsilon: %e\n", ep);
    printf("Float significant digits: %d\n", significantDigits(ep));
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
    printf("Double epsilon: %e\n", ep);
    printf("Double significant digits: %d\n", significantDigits(ep));
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
    printf("Long double epsilon: %Le\n", ep);
    printf("Long double significant digits: %d\n", significantDigits(ep));

}

int significantDigits(double epsilon) {
    return (int)(-log10(epsilon));
}