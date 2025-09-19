#!/bin/bash
#SBATCH --job-name=simple_test            # Job name
#SBATCH --nodes=1                         # Number of nodes (do not change)
#SBATCH --partition=debug                 # Partition or queue name (do not change)
#SBATCH --nodelist=node3                  # Use the node assigned to you (change this if needed)
#SBATCH --ntasks=1                        # Number of tasks per node
#SBATCH --time=00:40:00                   # Maximum runtime (HH:MM:SS)
#SBATCH --output=out_%j.txt               # standard output file
#SBATCH --error=err_%j.txt                # standard error file

# -------------------------

# Your commands to run your code should be here

# write the bash script to compile and run the code gen_inp.cpp
g++ gen_inp.cpp -o gen_inp -std=c++17
# run the code to generate test cases
./gen_inp

# compile the main.cpp file
g++ ../../main.cpp -o main -std=c++17 

# echo "Input Size,Time Taken (microseconds),Memory usage(kB)" > time_taken.csv
echo "Test_Case,Nodes(n),Edges(m),Metro_Cities(k),Time_Taken(microseconds),Memory_Usage(kB)" > time_taken.csv

## run for n test cases

for i in $(seq 1 $1)
do
  echo "Running test case $i"
  
  # Extract input characteristics from test case file
  test_file="./test_case_$i.txt"
  
  # Count edges (lines between "edges:" and "metro_cities:")
  edges_count=$(sed -n '/^edges:/,/^metro_cities:/p' "$test_file" | grep -E '^[0-9]' | wc -l)
  
  # Count metro cities (lines after "metro_cities:")
  metro_count=$(sed -n '/^metro_cities:/,$p' "$test_file" | grep -E '^[0-9]' | wc -l)
  
  # Find maximum node number to determine n
  max_node=$(sed -n '/^edges:/,/^metro_cities:/p' "$test_file" | grep -E '^[0-9]' | awk '{print $1, $2}' | tr ' ' '\n' | sort -n | tail -1)
  
  # If max_node is empty, check metro_cities section too
  if [ -z "$max_node" ]; then
    max_node=$(grep -E '^[0-9]' "$test_file" | awk '{print $1, $2}' | tr ' ' '\n' | sort -n | tail -1)
  fi
  
  # Set default if still empty
  if [ -z "$max_node" ]; then
    max_node=0
  fi
  
  output=$(./main "$test_file" "./output_$i.txt")
  echo "$output"

  total_time=$(echo "$output" | grep -oP 'Total time taken: \K[0-9]+')
  memory_used=$(echo "$output" | grep -oP 'Memory usage \(VmRSS\):\s*\K[0-9]+')

  echo "$i,$max_node,$edges_count,$metro_count,$total_time,$memory_used" >> time_taken.csv
done
# i=1
# for file in test_*.txt; do
#     # start i from 1 and increment for each file
#     # Extract the preorder line, remove 'preorder:', count the numbers
#     ./main "$file" "./output_$i.txt"
#     # size=$(grep '^preorder:' "$file" | sed 's/preorder://g' | wc -w)
#     # echo "$file: $size"
#     i=$((i + 1))
# done



