set xlabel "masse"
set ylabel "effectif"

set boxwidth 2
plot "histogramme_mass.out" using 1:2
pause -1
set terminal postscript
set output "histo_mass.ps"
replot
set terminal x11
