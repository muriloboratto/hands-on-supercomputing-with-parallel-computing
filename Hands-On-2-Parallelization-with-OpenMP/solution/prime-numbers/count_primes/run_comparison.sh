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
plot "result" using 1:2 title "static" with linespoints,\
     "result" using 1:4 title "static,1" with linespoints,\
     "result" using 1:6 title "dynamic" with linespoints,\
     "result" using 1:8 title "dynamic,64" with linespoints
set title "Speedup"
set ylabel "Speedup"
set output "speedup.eps"
plot "result" using 1:3 title "static" with linespoints,\
     "result" using 1:5 title "static,1" with linespoints,\
     "result" using 1:7 title "dynamic" with linespoints,\
     "result" using 1:9 title "dynamic,64" with linespoints
EOF
}

printf 'Compiling and setting permissions\n\n'
gcc count_primes.c -o count_primes -fopenmp -lm -O3
chmod +x count_primes

printf 'Running tests\n\n'

printf 'Schedule\t\tstatic\t\t\t\tstatic,1\t\t\tdynamic\t\t\t\tdynamic,64\n'
printf 'Threads\tTime\t\tSpeedup\t\tTime\t\tSpeedup\t\tTime\t\tSpeedup\t\tTime\t\tSpeedup\n'

duration_1t_static=$(OMP_NUM_THREADS=1 OMP_SCHEDULE=static ./count_primes | awk '{print $1}')

for num_threads in {1..16}; do
    line=$num_threads

    for schedule in 'static' 'static,1' 'dynamic' 'dynamic,64'; do
        duration=$(OMP_SCHEDULE=$schedule OMP_NUM_THREADS=$num_threads ./count_primes | awk '{print $1}')
        speedup=$(echo "scale=6;$duration_1t_static/$duration" | bc)
        speedup=$(printf "%.6f" $speedup)
        line="$line"$'\t'"$duration"'\t'"$speedup"
    done

    printf "$line\n" >>result
    printf "$line\n"
done

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
rm -f count_primes result plot.gp

printf "\nFinished"
