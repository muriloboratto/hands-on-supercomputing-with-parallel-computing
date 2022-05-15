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
     "file_comparison.data" using 1:3 title "T=2"                     ls 2 with linespoints,\
     "file_comparison.data" using 1:4 title "T=3"                     ls 3 with linespoints,\
     "file_comparison.data" using 1:5 title "T=4"                     ls 4 with linespoints
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

plot "file_speedup.data" using 1:2 title "T=2"    ls 1 with linespoints,\
     "file_speedup.data" using 1:3 title "T=3"    ls 2 with linespoints,\
     "file_speedup.data" using 1:4 title "T=4"    ls 3 with linespoints
EOF
}

################################################
# 0. COMPILATION + PERMISSIONS  TO EXECUTE     #
################################################

gcc mm.c -o mm -fopenmp -O3
chmod +x mm

###################################
# Experimental Times              #
###################################

sleep 5 > /dev/null 2>&1 &

printf "Loading...\040\040" ; showPropeller
echo " "

for i in 100 200 300 400 500 600 700 800 900 1000
do
printf "\033[1D$i :" 
OMP_NUM_THREADS=1   ./mm               "$i"    >> file1
OMP_NUM_THREADS=2   ./mm               "$i"    >> file2
OMP_NUM_THREADS=3   ./mm               "$i"    >> file3
OMP_NUM_THREADS=4   ./mm               "$i"    >> file4
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
echo "    [#][size]       [S]	           [T02]	   [T03]	  [T04]"
cat -n  file_comparison.data

sleep 3

#####################
# SPEEDUP           #
#####################

awk '{print $1, " ",(($2*1000)/($3*1000))}' file_comparison.data >> fspeed0 #OMP T=02 
awk '{print $1, " ",(($2*1000)/($4*1000))}' file_comparison.data >> fspeed1 #OMP T=03
awk '{print $1, " ",(($2*1000)/($5*1000))}' file_comparison.data >> fspeed2 #OMP T=04


pr -m -t -s\  fspeed0 fspeed1 fspeed2 | awk '{print $1,"\t",$2,"\t",$4,"\t",$6}' >> file_speedup.data

echo " "
echo " "
echo "    ********************************"
echo "    * Speedup  Rate                *"
echo "    ********************************"
echo " "
echo "    [#][size]    [ST02]	          [ST03]	  [ST04]"
cat -n file_speedup.data

sleep 3

#####################
# PLOTING           #
#####################
echo " "
echo "Do you want to plot a graphic (y/n)?"
read resp

if [[ $resp = "y" ]];then
         echo "ploting eps graphic with gnuplot..."
         create_plot_script_time
         create_plot_script_speedup
         gnuplot "time.plt"
         gnuplot "speedup.plt"
#rename the files
  mv time.eps  time.$(whoami)@$(hostname)-$(date +%F%T).eps
  mv speedup.eps  speedup.$(whoami)@$(hostname)-$(date +%F%T).eps

fi

sleep 1

echo " "
echo "[Remove unnecessary files] "
rm -f *.txt file* fspeed* *.data mm *.plt
echo " "

sleep 7 > /dev/null 2>&1 &

printf "Loading...\040\040" ; showPropeller
echo " "
echo "[END] " 
echo " "

