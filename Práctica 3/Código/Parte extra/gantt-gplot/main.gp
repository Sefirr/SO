set terminal postscript eps color solid size 10,5
#set terminal pdf color solid size 10,5
if (!exists("myfont")) myfont="Helvetica,18"
if (!exists("outfile")) outfile='graph.eps'
if (!exists("script")) script='out.gp'
set output outfile
set key font myfont
set xtics font myfont
set ytics font myfont
set xlabel font myfont
set key font ",20"
load script
#unset output 
