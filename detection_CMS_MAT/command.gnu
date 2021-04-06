set xlabel "masse"
set ylabel "effectif"

set boxwidth 2
plot "histogramme_mass.out" using 2  with histograms


pause -1
plot "histogramme_mass.out" using 3  with histogram
pause -1
set terminal postscript
set output "histo_mass.ps"
replot
set terminal x11
