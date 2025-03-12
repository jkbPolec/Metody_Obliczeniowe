#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* Temat zadania:
Zaimplementuj w jezyku "C/C++" algorytm obliczajacy przyblizone wartosci funkcji
f(x) = x^3 / (6[sinh(x) - x]) dla x ∈ [10^(-10), 10^3], korzystajac z funkcji standardowej sinh(). W oparciu
o zbior dokladnych wartosci tej funkcji, udostepniony przez prowadzacego zajecia, zbadaj jak
zmieniaja sie bledy wzgledne przyblizenia funkcji w tym algorytmie, w zaleznosci od x. W tym celu
wykonaj rysunek przedstawiajacy zaleznosc logarytmu dziesietnego z bezwzglednej wartosci bledu
wzglednego od logarytmu dziesietnego z x. Z wykresu odczytaj zakres zmiennej x, w ktorym blad
wzgledny pozostaje na poziomie bledu reprezentacji, oraz zakresy zmiennej x, w ktorych blad
wzgledny jest wiekszy. Wyjasnij przyczyny obserwowanych zmian bledow. Na tej podstawie
zaproponuj alternatywny sposob obliczania wartosci funkcji f(x) w sytuacjach gdy obserwowany
blad jest duzy. Dokonaj stosownej modyfikacji programu, tak aby uzyskac blad wzgledny na
poziomie bledu reprezentacji (czyli tzw. dokladnosc maszynowa) dla dowolnego x ∈ [10^(-10), 10^3].
Jaki typ zmiennych nalezy zastosowac i dlaczego?
Do wykonania rysunku w tym cwiczeniu (a takze w niektorych dalszych cwiczeniach) najlepiej uzyc
programu GNUPLOT (dostepnego za darmo z Internetu).
*/

long double fun(long double x);
long double fun_taylor(long double x);

int main() {



    FILE *file = fopen("../numbers.txt", "r");
    FILE *fileOutput = fopen("../data.txt", "w");
    if (file == NULL) {
        perror("Unable to open file");
        return;
    }

    long double col1, col2, col3;
    long double result;

    while (fscanf(file, "%Lf %Lf %Le", &col1, &col2, &col3) == 3) {
        result = fun(col2);
        long double error = fabsl((result - col3) / col3);
        error = log10l(error);
        fprintf(fileOutput, "%Lf %.20Le\n", col1, &error);
    }

    fclose(fileOutput);
    fclose(file);

    return 0;
}


long double fun(long double x) {
    return (x*x*x / (6 * (sinh(x) - x)));
}

long double fun_taylor(long double x) {
    long double sq = x*x;
    return 1.0L / (1.0L + (sq / 20.0L) + (sq * sq / 120.0L));
}

