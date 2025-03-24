#include <math.h>
#include <stdio.h>

#define MAXN 1000
#define EPS 0.0001

long double tanFunction(long double x) {
    return tanhl(x) + 2.0L * (x-1.0L);
}

long double picardTan(long double x) {
    return -1.0L/2.0L * tanhl(x)+1.0L;
}

long double newtonTan(long double x) {
    return 1.0L - tanhl(x) * tanhl(x) + 2.0L;
}

long double sinFunction(long double x) {
    return sinhl(x) + x/4 - 1;
}

long double picardSin(long double x) {
    return 4.0L * coshl(x);
}

long double newtonSin(long double x) {
    return coshl(x) + 1.0L / 4.0L;
}


long double Picard() {

    int n = 0;
    long double x0 = 0.0L;
    long double oldX0;

    printf("Picard\n");
    while (n < MAXN) {
        oldX0 = x0;
        x0 = picardTan(x0);
        if (fabsl(x0 - oldX0) < EPS) {
            break;
        }
        n += 1;
    }

    printf("%Lf\n", x0);

    n = 0;
    x0 = 0.0L;
    oldX0 = 0.0L;

    while (n < MAXN) {
        oldX0 = x0;
        x0 = picardSin(x0);
        if (fabsl(x0 - oldX0) < EPS) {
            break;
        }
        n += 1;
    }

    printf("%Lf\n", x0);


}

long double Bisection() {

    long double a = 0.0L;
    long double b = 1.0L;
    long double c = 0.0L;

    int n = 0;

    printf("Bisection\n");

    while (n < MAXN) {
        c = (a + b) / 2.0L;
        if (tanFunction(c) * tanFunction(a) < 0) {
            b = c;
        } else {
            a = c;
        }
        n += 1;
    }

    printf("%Lf\n", c);


    a = 0.0L;
    b = 1.0L;
    c = 0.0L;

    n = 0;


    while (n < MAXN) {
        c = (a + b) / 2.0L;
        if (sinFunction(c) * sinFunction(a) < 0) {
            b = c;
        } else {
            a = c;
        }
        n += 1;
    }



    printf("%Lf\n", c);



}

long double Newton()
{
    long double x = 0;
    long double x0 = 0;
    int n = 0;

    printf("Newton\n");

    while (n < MAXN) {
        x0 = x - tanFunction(x)/newtonTan(x);

        if (fabsl(x0 - x) < EPS)
        {
            break;
        }

        x = x0;
        n += 1;
    }

    printf("%Lf\n", x);


    while (n < MAXN) {
        x0 = x - sinFunction(x)/newtonSin(x);

        if (fabsl(x0 - x) < EPS)
        {
            break;
        }

        x = x0;
        n += 1;
    }

    printf("%Lf\n", x);
}



int main() {


    Picard();

    printf("");

    Bisection();

    printf("");

    Newton();
    return 0;
}
