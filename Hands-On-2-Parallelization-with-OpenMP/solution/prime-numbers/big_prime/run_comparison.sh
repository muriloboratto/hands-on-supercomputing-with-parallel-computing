#!/bin/bash

export LC_NUMERIC="en_US.UTF-8"

set -euo pipefail

create_plot_script() {
cat <<EOF >plot.gp
set xlabel "Threads"
set terminal postscript eps enhanced color
set key top left
set key box
set style data lines
set title "Execution Time"
set ylabel "Time (Seconds)"
set output "time.eps"
plot "result" using 1:2 title "Execution Time" with linespoints
set title "Speedup"
set ylabel "Speedup"
set output "speedup.eps"
plot "result" using 1:3 title "Speedup" with linespoints
EOF
}

printf 'Compiling and setting permissions\n\n'
module load gcc/11.1.0
gcc big_prime.c -o big_prime -fopenmp -lm -O3
chmod +x big_prime

printf 'Running tests\n\n'
printf 'Threads\tTime\t\tSpeedup\n'

reference=$(OMP_NUM_THREADS=1 ./big_prime | awk '{print $1}')
printf '1\t%s\t1\n' $reference >>result
printf "1\t%s\t1\n" $reference

for num_threads in {2..16}; do
    duration=$(OMP_NUM_THREADS=$num_threads ./big_prime | awk '{print $1}')
    speedup=$(echo "scale=6;$reference/$duration" | bc)

    printf '%d\t%f\t%.5f\n' $num_threads $duration $speedup >>result
    printf '%d\t%f\t%.5f\n' $num_threads $duration $speedup
done

printf '\nDo you want to plot the graphs? (y/n)\n'
read choice

if [[ $choice == "y" ]]; then
    create_plot_script
    gnuplot plot.gp
    filename=$(whoami)@$(hostname)-$(date +%F-%T)
    mv time.eps time.$filename.eps
    mv speedup.eps speedup.$filename.eps
    printf '\nOutput at:\ntime.%s.eps\nspeedup.%s.eps' $filename $filename
fi

printf '\n\nClearing temporary files\n'
rm -f big_prime result plot.gp

printf '\nFinished\n'
