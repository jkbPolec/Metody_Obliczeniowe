#include <stdio.h>
#include <math.h>

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


void sinhlVersion();
void sinhVersion();

int main() {

    sinhlVersion();

    return 0;
}

//"Zaczyna dzialac poprawnie dla x = 10^(-5.77) i dziala dobrze do konca"
void sinhlVersion()
{
    long double x;
    double power = -10;

    while (power <= 3) {
        x = powl(10L, power);
        long double result = powl(x, 3.0) / (6.0 * (sinhl(x) - x));
        printf("power = %lf x = %.20Le result = %Le\n",power, x, result);
        power += 0.01;
    }
}
//"Zaczyna dzialac poprawnie dla x = 10^(-4.10) i konczy dla x = 10^2.85"
void sinhVersion() {
    long double x;
    double power = -10;

    while (power <= 3) {
        x = powl(10L, power);
        long double result = powl(x, 3.0) / (6.0 * (sinh(x) - x));
        printf("power = %lf x = %.20Le result = %Le\n",power, x, result);
        power += 0.01;
    }
}
