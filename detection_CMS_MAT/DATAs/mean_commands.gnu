set title "efficacite en fonction de c*tau"
set xlabel "c*tau (m)"
set ylabel "efficacite"

set logscale y
set yrange [1e-10:]

plot "result_mean.dat" using 1:4:5 with errorbars lc rgb "black" title "CMS err std"
replot "result_mean.dat" using 1:4 with points pointsize 0.5 pointtype 5 lc rgb "red" title "CMS moyenne"

replot "result_mean.dat" using 1:2:3 with errorbars lc rgb "black" title "MATHUSLA err std"
replot "result_mean.dat" using 1:2 with points pointsize 0.5 pointtype 5 lc rgb "blue" title "MATHUSLA moyenne"

pause -1
set terminal png
set output "efficacite_CMS_MAT.png"
replot
set terminal x11
