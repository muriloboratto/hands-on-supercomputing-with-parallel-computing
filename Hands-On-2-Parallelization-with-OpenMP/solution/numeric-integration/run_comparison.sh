#!/bin/bash

export LC_NUMERIC="en_US.UTF-8"

set -euo pipefail

create_plot_script() {
cat <<EOF >plot.gp
set xlabel "Size"
set terminal postscript eps enhanced color
set key top left
set key box
set style data lines
set title "Execution Time"
set ylabel "Time (Seconds)"
set output "time.eps"
plot "time" using 1:2 title "1T" with linespoints,\
     "time" using 1:3 title "2T" with linespoints,\
     "time" using 1:4 title "4T" with linespoints,\
     "time" using 1:5 title "8T" with linespoints,\
     "time" using 1:6 title "16T" with linespoints
set title "Speedup Variant 1"
set ylabel "Speedup"
set output "speedup.eps"
plot "speedup" using 1:2 title "2T" with linespoints,\
     "speedup" using 1:3 title "4T" with linespoints,\
     "speedup" using 1:4 title "8T" with linespoints,\
     "speedup" using 1:5 title "16T" with linespoints
EOF
}

printf 'Compiling and setting permissions\n\n'
module load gcc/11.1.0
gcc integral-omp.c -o integral-omp -fopenmp -lm -O3
chmod +x integral-omp

for i in {0..7}; do
    let steps=25000000+$i*25000000
    printf 'Running test with size problem %d\n' $steps

    line=$steps
    for num_threads in 1 2 4 8 16; do
        line="${line}"$'\t'"$(OMP_NUM_THREADS=$num_threads ./integral-omp 1 ${steps} | awk '{print $1}')"
    done
    printf "$line\n" >>time
done

awk '{printf("%d\t%f\t%f\t%f\t%f\n", $1, (($2*1000)/($3*1000)), (($2*1000)/($4*1000)), (($2*1000)/($5*1000)), (($2*1000)/($6*1000)) )}' time >>speedup

printf '\nPrinting results:\n'

printf 'Execution Time\n'
printf "Size\t\t1T\t\t2T\t\t4T\t\t8T\t\t16T\n"
cat time

printf "\nSpeedup\n"
printf "Size\t\t2T\t\t4T\t\t8T\t\t16T\n"
cat speedup

printf "\nDo you want to plot the graphs? (y/n)\n"
read choice

if [[ $choice == "y" ]]; then
    create_plot_script
    gnuplot plot.gp
    filename=$(whoami)@$(hostname)-$(date +%F-%T)
    mv time.eps time.$filename.eps
    mv speedup.eps speedup.$filename.eps
    printf '\nOutput at:\ntime.%s.eps\nspeedup.%s.eps' $filename $filename
fi

printf "\n\nClearing temporary files\n"
rm -f integral-omp time speedup plot.gp

printf "\nFinished\n"
