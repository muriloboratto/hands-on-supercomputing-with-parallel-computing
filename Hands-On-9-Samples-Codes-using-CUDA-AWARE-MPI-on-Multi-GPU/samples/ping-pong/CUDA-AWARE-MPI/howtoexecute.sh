#!/bin/bash

usage()
{
 echo "howtoexecute.sh: wrong number of input parameters. Exiting."
 echo -e "Usage: bash howtoexecute.sh <supercomputer>"
 echo -e "  g.e: bash howtoexecute.sh ogbon"
}

nowherman()
{
 mpirun -np 2 ./ping-pong-CUDA-AWARE-MPI
}

ogbon()
{
 sbatch slurm-CUDA-AWARE-MPI.sh
}

nvlink()
{
 module load openmpi/4.1.1-cuda
 mpirun -np 2 -x UCX_MEMTYPE_CACHE=n  -mca pml ucx -mca btl ^vader,tcp,openib,smcuda -x UCX_NET_DEVICES=mlx5_0:1  ./ping-pong-CUDA-AWARE-MPI
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

#nvlink
if [[ $1 == "nvlink" ]];then
 nvlink
fi

