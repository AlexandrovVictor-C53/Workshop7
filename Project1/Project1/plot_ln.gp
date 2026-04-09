set terminal pngcairo size 800,600 enhanced font 'Arial,12'
set output 'plot_ln.png'
set title 'Зависимость ln I_k от U_эб'
set xlabel 'U_эб, В'
set ylabel 'ln I_k (I_k в мА)'
set grid

# Определяем функцию прямой с вашими числами
f(x) = 29.888040 * x - 16.889447

# Рисуем точки и прямую
plot 'data1.txt' using 1:(log($2/12)) with points pt 7 title 'Эксперимент', \
     f(x) with lines title 'Аппроксимация'
