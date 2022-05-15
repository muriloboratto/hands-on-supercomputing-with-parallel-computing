#!/bin/sh

#SBATCH --job-name=CUDA-AWARE-MPI              # Job name
#SBATCH --nodes=2                              # Run all processes on 2 nodes  
#SBATCH --partition=GPUlongB                   # Partition OGBON
#SBATCH --output=out_%j.log                    # Standard output and error log
#SBATCH --ntasks-per-node=1                    # 1 job per node
#SBATCH --account=cenpes-lde                   # Account of the group 

module load openmpi/4.1.1-cuda

mpirun -np 2 -x UCX_MEMTYPE_CACHE=n  -mca pml ucx -mca btl ^vader,tcp,openib,smcuda -x UCX_NET_DEVICES=mlx5_0:1  ./ping-pong-CUDA-AWARE-MPI