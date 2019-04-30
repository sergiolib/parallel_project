#!/bin/bash -l

# job name
#SBATCH -J Project
# account
#SBATCH -A edu19.SF2568
# email notification
#SBATCH --mail-type=BEGIN,END
# 10 minutes wall-clock time will be given to this job
#SBATCH -t 01:00:00
# Number of nodes
#SBATCH --nodes=2
# set tasks per node to 24 in order to disablr hyperthreading
#SBATCH --ntasks-per-node=48

module add i-compilers intelmpi opencv

mpirun -np 96 ./build/ParallelGenetic -i data/kenny.png -e 1000000

