
set boxwidth 2
set ylabel "effectif"
#set xlabel "masse"
#plot "histogramme_mass.out" using 1:2
set xlabel "ctau"
plot "part_decay.out" using 1:2
pause -1
set terminal postscript
set output "histo_mass.ps"
replot
set terminal x11
