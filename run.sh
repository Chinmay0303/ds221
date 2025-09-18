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
g++ sample_tests/question1/gen_inp.cpp -o gen_inp
# run the code 
./gen_inp



# make a loop to run main.cpp with n different input and output files
#!/bin/bash

g++ main.cpp -o main 
# ./main "./sample_tests/question1/test_case_1.txt" "output_1.txt"
for i in $(seq 1 $1)
do
  echo "input_$i.txt"
  echo "output_$i.txt"
  ./main "./sample_tests/question1/test_case_$i.txt" "./sample_tests/question1/output_$i.txt"
done

# also i want to generate a csv file with the time taken for each input file
# first calculate size of each input file from the number of lines in the file
#  main function gives the time taken in microseconds, save it in a csv file with two columns: input size, time taken
# input is given like this as output of main function: Total time taken: 21 microseconds in the terminal
# how to capture this output in a variable in bash from terminal
# time_taken=$(./main "./sample_tests/question1/test_case_1.txt" "output_1.txt" | grep -oP 'Total time taken: \K[0-9]+')
# echo $time_taken
echo "Input Size,Time Taken (microseconds)" > time_taken.csv

# correct this loop


for i in $(seq 1 $1)
do
  input_size=$(wc -l < "./sample_tests/question1/test_case_$i.txt")
  input_size=$((input_size - 1)) # subtract 1 for the first line, if needed
  time_taken=$(./main "./sample_tests/question1/test_case_$i.txt" "./sample_tests/question1/output_$i.txt" 2>&1 | grep -oP 'Total time taken: \K[0-9]+')
  echo "$input_size,$time_taken" >> time_taken.csv
done




