#!/bin/bash
# SBATCH --job-name=simple_test            # Job name
# SBATCH --nodes=1                         # Number of nodes (do not change)
# SBATCH --partition=debug                 # Partition or queue name (do not change)
# SBATCH --nodelist=node3                  # Use the node assigned to you (change this if needed)
# SBATCH --ntasks=1                        # Number of tasks per node
# SBATCH --time=00:40:00                   # Maximum runtime (HH:MM:SS)
# SBATCH --output=out_%j.txt               # standard output file
# SBATCH --error=err_%j.txt                # standard error file

# -------------------------

# Your commands to run your code should be here

# write the bash script to compile and run the code gen_inp.cpp to generate test cases
g++ gen_inp.cpp -o gen_inp
# run the code 
./gen_inp

# compile the main.cpp file
g++ ../../main.cpp -o main 

echo "Input Size,Time Taken (microseconds),Memory usage(kB)" > time_taken.csv

## run for n test cases

for i in $(seq 1 $1)
do
  echo "Running test case $i"
  output=$(./main "./test_case_$i.txt" "./output_$i.txt")
  echo "$output"
  input_size=$(wc -l < "./test_case_$i.txt")
  input_size=$((input_size - 1)) # subtract 1 for the first line, if needed

  total_time=$(echo "$output" | grep -oP 'Total time taken: \K[0-9]+')
  memory_used=$(echo "$output" | grep -oP 'Memory usage \(VmRSS\):\s*\K[0-9]+')

  echo "$input_size,$total_time,$memory_used" >> time_taken.csv
done




