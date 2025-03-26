#include <math.h>
#include <stdio.h>

#define MAXN 1000
#define EPS 0.000000001

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
    return asinhl(1 - x/4);
}

long double newtonSin(long double x) {
    return coshl(x) + 1.0L / 4.0L;
}

void Picard() {
    int n = 0;
    long double x0 = 0.0L;
    long double oldX0;
    long double fx;

    printf("Picard tan\n");
    printf("x0\t\testymator bledu\t\tfx\n");
    while (n < MAXN) {
        oldX0 = x0;
        x0 = picardTan(x0);
        fx = tanFunction(x0);
        printf("%d %0.9Lf\t%0.9Lf\t%0.9Lf\n", n, x0, x0-oldX0, fx);
        if (fabsl(x0 - oldX0) < EPS || fabsl(fx) < EPS) {
            break;
        }
        n += 1;
    }

    printf("\n\n");

    n = 0;
    x0 = 0.0L;
    oldX0 = 0.0L;

    printf("Picard sin\n");
    printf("x0\t\testymator bledu\t\tfx\n");
    while (n < MAXN) {
        oldX0 = x0;
        x0 = picardSin(x0);
        fx = sinFunction(x0);
        printf("%d %0.9Lf\t%0.9Lf\t%0.9Lf\n", n, x0, x0-oldX0, fx);
        if (fabsl(x0 - oldX0) < EPS || fabsl(fx) < EPS) {
            break;
        }
        n += 1;
    }
    printf("\n\n");
}

void Bisection() {
    long double a = 0.0L;
    long double b = 1.0L;
    long double c = 0.0L;
    long double fx;

    int n = 0;

    printf("Bisection tan\n");
    printf("c\t\testymator bledu\t\tfx\n");
    while (n < MAXN) {
        c = (a + b) / 2.0L;
        fx = tanFunction(c);
        if ((tanFunction(c) > 0 && tanFunction(a) < 0) || (tanFunction(c) < 0 && tanFunction(a) > 0)) {
            b = c;
        } else {
            a = c;
        }
        printf("%d %0.9Lf\t%0.9Lf\t%0.9Lf\n", n, c, fabsl((b-a))/2, fx);
        if (fabsl((b-a))/2 < EPS || fabsl(fx) < EPS) {
            break;
        }
        n += 1;
    }
    printf("\n\n");

    a = 0.0L;
    b = 1.0L;
    c = 0.0L;

    n = 0;

    printf("Bisection sin\n");
    printf("c\t\testymator bledu\t\tfx\n");
    while (n < MAXN) {
        c = (a + b) / 2.0L;
        fx = sinFunction(c);
        if ((sinFunction(c) > 0 && sinFunction(a) < 0) || (sinFunction(c) < 0 && sinFunction(a) > 0)) {
            b = c;
        } else {
            a = c;
        }
        printf("%d %0.9Lf\t%0.9Lf\t%0.9Lf\n", n, c, fabsl((b-a))/2, fx);
        if (fabsl((b-a))/2 < EPS || fabsl(fx) < EPS) {
            break;
        }
        n += 1;
    }
    printf("\n\n");
}

void Newton() {
    long double x = 0;
    long double x0 = 0;
    long double fx;
    int n = 0;

    printf("Newton tan\n");
    printf("x0\t\testymator bledu\t\tfx\n");
    while (n < MAXN) {
        x0 = x - tanFunction(x)/newtonTan(x);
        fx = tanFunction(x0);
        printf("%d %0.9Lf\t%0.9Lf\t%0.9Lf\n", n, x0, x0-x, fx);
        if (fabsl(x0 - x) < EPS || fabsl(fx) < EPS) {
            break;
        }
        x = x0;
        n += 1;
    }
    printf("\n\n");

    printf("Newton sin\n");
    printf("x0\t\testymator bledu\t\tfx\n");
    n = 0;
    while (n < MAXN) {
        x0 = x - sinFunction(x)/newtonSin(x);
        fx = sinFunction(x0);
        printf("%d %0.9Lf\t%0.9Lf\t%0.9Lf\n", n, x0, x0-x, fx);
        if (fabsl(x0 - x) < EPS || fabsl(fx) < EPS) {
            break;
        }
        x = x0;
        n += 1;
    }
    printf("\n\n");
}

void Secant() {
    long double x0 = 0.0L;
    long double x1 = 1.0L;
    long double x2 = 0.0L;
    long double fx;
    int n = 0;

    printf("Secant tan\n");
    printf("x2\t\testymator bledu\t\tfx\n");
    while (n < MAXN) {
        x2 = x1 - tanFunction(x1) * (x1 - x0) / (tanFunction(x1) - tanFunction(x0));
        fx = tanFunction(x2);
        printf("%d %0.9Lf\t%0.9Lf\t%0.9Lf\n", n, x2, x2-x1, fx);
        if (fabsl(x2 - x1) < EPS || fabsl(fx) < EPS) {
            break;
        }
        x0 = x1;
        x1 = x2;
        n += 1;
    }
    printf("\n\n");

    x0 = 0.0L;
    x1 = 1.0L;
    x2 = 0.0L;
    n = 0;

    printf("Secant sin\n");
    printf("x2\t\testymator bledu\t\tfx\n");
    while (n < MAXN) {
        x2 = x1 - sinFunction(x1) * (x1 - x0) / (sinFunction(x1) - sinFunction(x0));
        fx = sinFunction(x2);
        printf("%d %0.9Lf\t%0.9Lf\t%0.9Lf\n", n, x2, x2-x1, fx);
        if (fabsl(x2 - x1) < EPS || fabsl(fx) < EPS) {
            break;
        }
        x0 = x1;
        x1 = x2;
        n += 1;
    }
    printf("\n\n");
}

int main() {
    Picard();
    Bisection();
    Newton();
    Secant();
    return 0;
}