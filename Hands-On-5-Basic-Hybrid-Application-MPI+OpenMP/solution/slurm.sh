#!/bin/sh

#SBATCH --job-name=Hybrid
#SBATCH --nodes=1
#SBATCH --partition=cpulongb
#SBATCH --output=out_%j.log
#SBATCH --ntasks-per-node=36
#SBATCH --account=cenpes-lde

module load openmpi/4.1.1-cuda-11.6-ofed-5.4

bash START.sh
