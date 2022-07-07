#!/bin/bash

export LC_NUMERIC="en_US.UTF-8"

set -euo pipefail

create_plot_script() {
    cat <<EOF >plot.gp
set xlabel "Processes"
set terminal postscript eps enhanced color
set key top left
set key box
set style data lines
set title "Execution Time"
set ylabel "Time (Seconds)"
set output "time.eps"
plot "result" using 1:2 title "2048x2048" with linespoints
set title "Speedup"
set ylabel "Speedup"
set output "speedup.eps"
plot "result" using 1:3 title "2048x2028" with linespoints
EOF
}

printf "Compiling and setting permissions\n\n"
mpicc -O3 -o lu lu.c
mpicc -O3 -o lu_mpi lu_mpi.c
chmod +x lu lu_mpi

problem_size=2048

printf 'Running tests\n'
reference=$(./lu $problem_size | tail -n 1 | awk '{print $2}')
printf "Serial solution reference time: %s\n\n" $reference

printf 'Procs\tTime\t\tSpeedup\n'

for num_processes in {1..8}; do
    line=$num_processes

    duration=$(mpirun --oversubscribe -np $num_processes ./lu_mpi $problem_size | tail -n 1 | awk '{print $2}')
    speedup=$(echo "scale=6;$reference/$duration" | bc)
    speedup=$(printf "%.6f" $speedup)
    line=$(printf '%s\t%s\t%s' $line $duration $speedup)

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

printf '\n\nClearing temporary files\n'
rm -f lu lu_mpi result plot.gp

printf '\nFinished\n'
