set terminal pngcairo size 800,600 enhanced font 'Arial,12'
set output 'plot_Ik.png'
set title 'Зависимость I_k от U_эб'
set xlabel 'U_эб, В'
set ylabel 'I_k, мА'
set grid

# Определяем I0 и функцию g(x) ДО команды plot
I0 = exp(-17.901705)
g(x) = I0 * exp(32.289788*x)

# Рисуем точки и кривую в одной команде plot
plot 'data2.txt' using 1:($2/12) with points pt 7 title 'Эксперимент', \
     g(x) with lines title 'Теоретическая кривая'
