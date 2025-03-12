set terminal pngcairo enhanced size 800,600
set output 'blad_wzgledny.png'

set title "Błąd względny dla f(x)"
set xlabel "log(x)"
set ylabel "log10(błąd względny)"
set grid

plot "data.txt" using 1:2 with linespoints title "Błąd względny" lw 2 pt 7