
set boxwidth 2
set ylabel "effectif"
#set xlabel "masse"
#plot "histogramme_mass.out" using 1:2
set xlabel "ctau"
plot "part_decay.out" using 1:2
replot "part_decay.out" using 1:3
replot "part_decay.out" using 1:4
pause -1
set terminal png
set output "histo_mass.png"
replot
set terminal x11
