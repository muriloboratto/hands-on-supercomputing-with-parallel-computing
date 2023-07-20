#!/bin/sh

export LC_NUMERIC="en_US.UTF-8"

set -euo pipefail

clear 

###################################
# FUNCTIONS                       #
###################################

showPropeller() {
   
   tput civis
   
   while [ -d /proc/$! ]
   do
      for i in / - \\ \|
      do
         printf "\033[1D$i"
         sleep .1
      done
   done
   
   tput cnorm
}


create_plot_script_time() {
cat <<EOF >time.plt
set title "Execution Time" 
set ylabel "Time (Seconds)"
set xlabel "Size"

set style line 1 lt 2 lc rgb "cyan"   lw 2 
set style line 2 lt 2 lc rgb "red"    lw 2
set style line 3 lt 2 lc rgb "gold"   lw 2
set style line 4 lt 2 lc rgb "green"  lw 2
set style line 5 lt 2 lc rgb "blue"   lw 2
set style line 6 lt 2 lc rgb "black"  lw 2
set terminal postscript eps enhanced color
set output 'time.eps'

set xtics nomirror
set ytics nomirror
set key top left
set key box
set style data lines


plot "file_comparison.data" using 1:2 title "Sequential"              ls 1 with linespoints,\
     "file_comparison.data" using 1:3 title "OMP(T=8)"                ls 2 with linespoints,\
     "file_comparison.data" using 1:4 title "MPI(P=6)"                ls 3 with linespoints,\
     "file_comparison.data" using 1:5 title "MPI+OMP(P=4, T=6)"       ls 4 with linespoints
EOF
}

create_plot_script_speedup() {
cat <<EOF >speedup.plt
set title "Speedup" 
set ylabel "Speedup"
set xlabel "Size"

set style line 1 lt 2 lc rgb "cyan"   lw 2 
set style line 2 lt 2 lc rgb "red"    lw 2
set style line 3 lt 2 lc rgb "gold"   lw 2
set style line 4 lt 2 lc rgb "green"  lw 2
set style line 5 lt 2 lc rgb "blue"   lw 2
set style line 6 lt 2 lc rgb "black"  lw 2
set terminal postscript eps enhanced color
set output 'speedup.eps'

set xtics nomirror
set ytics nomirror
set key top left
set key box
set style data lines

plot "file_speedup.data" using 1:2 title "OMP(T=16)"             ls 1 with linespoints,\
     "file_speedup.data" using 1:3 title "MPI(P=36)"             ls 2 with linespoints,\
     "file_speedup.data" using 1:4 title "MPI+OMP(P=32, T=12)"    ls 3 with linespoints
EOF
}

################################################
# 0. COMPILATION + PERMISSIONS  TO EXECUTE     #
################################################

gcc mm-openmp.c -o sequential -fopenmp
gcc mm-openmp.c -o mm-openmp  -fopenmp -O3
mpicc mm-mpi.c -o mm-mpi -O3
mpicc mm-mpi+openmp.c -o mm-mpi+openmp -fopenmp -O3

chmod +x sequential
chmod +x mm-openmp
chmod +x mm-mpi
chmod +x mm-mpi+openmp

###################################
# Experimental Times              #
###################################

sleep 5 > /dev/null 2>&1 &

printf "Loading...\040\040" ; showPropeller
echo " "

for i in 100 200 300 400 500 600 700 800 900 1000
do
printf "\033[1D$i :" 
OMP_NUM_THREADS=1       ./mm-openmp        "$i"     >> file1
OMP_NUM_THREADS=16      ./mm-openmp        "$i"     >> file2
      mpirun  -np 36    ./mm-mpi           "$i"     2>/dev/null   >> file3
      mpirun  -np 32    ./mm-mpi+openmp    "$i"  12 2>/dev/null   >> file4
done

clear 

echo " "
echo " "
echo "    ********************************"
echo "    * Experimental Time Comparison *"
echo "    ********************************"
echo " "

pr -m -t -s\  file1 file2 file3 file4  | awk '{print $1,"\t",$2,"\t",$4,"\t",$6,"\t",$8}' >> file_comparison.data

echo " "
echo "    [#][size]       [S]	           [OMP]	   [MPI]	 [MPI+OMP]"
cat -n  file_comparison.data

sleep 3

#####################
# SPEEDUP           #
#####################

awk '{print $1, " ",(($2*1000)/($3*1000))}' file_comparison.data >> fspeed0 #OMP T1 
awk '{print $1, " ",(($2*1000)/($4*1000))}' file_comparison.data >> fspeed1 #OMP T2
awk '{print $1, " ",(($2*1000)/($5*1000))}' file_comparison.data >> fspeed2 #OMP T3


pr -m -t -s\  fspeed0 fspeed1 fspeed2 | awk '{print $1,"\t",$2,"\t",$4,"\t",$6}' >> file_speedup.data

echo " "
echo " "
echo "    ********************************"
echo "    * Speedup  Rate                *"
echo "    ********************************"
echo " "
echo "    [#][size]    [SOMP]	          [SMPI]	 [SMPI+OMP]"
cat -n file_speedup.data

sleep 1

#####################
# PLOTING           #
#####################
echo " "
echo "Plot a graphic"

sleep 1

echo "ploting eps graphic with gnuplot..."
create_plot_script_time
create_plot_script_speedup
gnuplot "time.plt"
gnuplot "speedup.plt"

#rename the files
mv time.eps  time.$(whoami)@$(hostname)-$(date +%F%T).eps
mv speedup.eps  speedup.$(whoami)@$(hostname)-$(date +%F%T).eps

sleep 1

echo " "
echo "[Remove unnecessary files] "
#rm -f *.txt file* fspeed* *.data mpi mm-mpi mm-openmp mm-mpi+openmp sequential *.plt
echo " "

sleep 7 > /dev/null 2>&1 &

printf "Loading...\040\040" ; showPropeller
echo " "
echo "[END] " 
echo " "

