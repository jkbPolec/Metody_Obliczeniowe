set terminal pngcairo font "Arial,12" enhanced size 1600,900
set output 'plot.png'

set title "Błąd względny dla f(x)"
set yrange [*:50]
set xlabel "x"
set ylabel "blad wzgledny"
set grid

plot "data.txt" with points pt 1 ps 1.5 lc rgb "purple"
