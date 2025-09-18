#!/bin/bash
#SBATCH --job-name=simple_test            # Job name
#SBATCH --nodes=1                         # Number of nodes (do not change)
#SBATCH --partition=debug                 # Partition or queue name (do not change)
#SBATCH --nodelist=node1                  # Use the node assigned to you (change this if needed)
#SBATCH --ntasks=1                        # Number of tasks per node
#SBATCH --time=00:05:00                   # Maximum runtime (HH:MM:SS)
#SBATCH --output=out_%j.txt               # standard output file
#SBATCH --error=err_%j.txt                # standard error file

# -------------------------

# Your commands to run your code should be here

echo "Hello from Slurm job on $(hostname)"
date
