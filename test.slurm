#!/bin/bash
#SBATCH --job-name=super_mario_name
#SBATCH --output=result.txt
#SBATCH --partition=nu-wide
#SBATCH --ntasks=16
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=4
#SBATCH --time=00:10:00

module load gcc/7.2.0
module load mpich /3.2.1-gcc-7.2.0

mpiexec -n 16 bin/super_mario_mpi 2 R cluster
