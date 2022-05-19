#!/bin/bash

usage()
{
 echo "howtocompile.sh: wrong number of input parameters. Exiting."
 echo -e "Usage: bash howtocompile.sh <supercomputer>"
 echo -e "  g.e: bash howtocompile.sh ogbon"
}

nowherman()
{
 nvcc -I/usr/include/openmpi -L/usr/lib/openmpi -lmpi -Xcompiler -fopenmp ping-pong-CUDA-AWARE-MPI.cu -o ping-pong-CUDA-AWARE-MPI
}


ogbon()
{
 module load openmpi/4.1.1-cuda-11.6-ofed-5.4
 nvcc -I/opt/share/openmpi/4.1.1-cuda/include -L/opt/share/openmpi/4.1.1-cuda/lib64 -DprintLabel -lnccl -lmpi -Xcompiler -fopenmp ping-pong-CUDA-AWARE-MPI.cu -o ping-pong-CUDA-AWARE-MPI
}

#args in comand line
if [ "$#" ==  0 ]; then
 usage
 exit
fi

#nowherman
if [[ $1 == "nowherman" ]];then
 nowherman
fi


#ogbon
if [[ $1 == "ogbon" ]];then
 ogbon
fi