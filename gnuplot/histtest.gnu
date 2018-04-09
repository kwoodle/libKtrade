#!/usr/bin/gnuplot

#col = 1
col = ARG1
#nbins = 100
nbins = ARG2
#datafile = '../gnuplot/votedata'
print 'ARG3 = ', ARG3
#print ' @ARG3 = ', @ARG3
datafile = ARG3
stats 'ARG3' using col nooutput
#N = STATS_records
min = STATS_min
max = STATS_max
delta = max-min

binwidth = delta/nbins
set boxwidth binwidth absolute
set style fill pattern 4
set yrange [0:*]
bin(val) = binwidth * floor(val/binwidth)
set ylabel 'counts'

unset key
#set autoscale xfixmax

plot datafile using (bin(column(col))):(1.0) smooth frequency with boxes

