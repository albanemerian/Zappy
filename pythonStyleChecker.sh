#!/bin/bash

# Define the log file
LOG_FILE="coding-style-python-reports.log"
> "$LOG_FILE"  # Clear the log file before writing

# Check if pycodestyle is installed
if ! command -v pycodestyle &> /dev/null; then
    echo "pycodestyle is not installed. Please install it using 'pip install pycodestyle'."
    exit 1
fi

# Run pycodestyle on all .py files in the ai directory, excluding tests/ and bonus/
find ./ai -type f -name "*.py" ! -path "*/tests/*" ! -path "*/bonus/*" ! -path "*/__pycache__/*" ! -path "*venv*" | while read -r file; do
    echo "Checking $file..."
    # Using --ignore=E266 to ignore the Epitech header style (multiple # characters at start of comments)
    # Setting max-line-length to 95 characters
    pycodestyle --ignore=E266,W504 --max-line-length=95 "$file" >> "$LOG_FILE" 2>&1
done

# Count the number of errors
ERROR_COUNT=$(wc -l < "$LOG_FILE")

# Inform user of completion
if [ "$ERROR_COUNT" -eq 0 ]; then
    echo "Coding style check completed. No errors found!"
    rm "$LOG_FILE"  # Remove the log file if empty
else
    echo "Coding style check completed. Found $ERROR_COUNT error(s). See $LOG_FILE for details."
fi
