#!/bin/bash

# Set the number of iterations
iterations=10

# Set the program to run
program="./RogueGame"

# Loop through the specified number of iterations
for i in $(seq 1 $iterations); do
  echo "Running iteration $i..."
  
  # Run the program and check the exit code
  $program
  status=$?
  
  # If the program exited with a non-zero status, print an error message and exit
  if [ $status -ne 0 ]; then
    echo "Error: Program exited with status $status on iteration $i."
    exit 1
  fi
  
  echo "Iteration $i completed successfully."
done

echo "All $iterations iterations completed successfully."
exit 0
