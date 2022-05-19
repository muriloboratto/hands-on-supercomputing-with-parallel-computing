#!/bin/bash

usage()
{
 echo "howtocompile.sh: wrong number of input parameters. Exiting."
 echo -e "Usage: bash howtocompile.sh <supercomputer>"
 echo -e "  g.e: bash howtocompile.sh ogbon"
}

nowherman()
{
 mpicc ping-pong-MPI.c -o ping-pong-MPI
}


ogbon()
{
 module load openmpi/4.1.1-cuda-11.6-ofed-5.4
 mpicc ping-pong-MPI.c -o ping-pong-MPI
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
