#!/bin/bash

# Epitech C Coding Style Checker - Simplified version
# This script implements basic checks for Epitech C coding style

REPORT_FILE="coding-style-reports.log"
> "$REPORT_FILE"  # Clear the log file

# Check for trailing spaces
check_trailing_spaces() {
    local file=$1
    local line_num=1

    while IFS= read -r line; do
        if echo "$line" | grep -q "[[:space:]]$"; then
            echo "$file:$line_num: trailing space" >> "$REPORT_FILE"
        fi
        ((line_num++))
    done < "$file"
}

# Check for tabs
check_tabs() {
    local file=$1
    local line_num=1

    while IFS= read -r line; do
        if echo "$line" | grep -q $'\t'; then
            echo "$file:$line_num: tab character(s)" >> "$REPORT_FILE"
        fi
        ((line_num++))
    done < "$file"
}

# Check for long lines (>80 characters)
check_line_length() {
    local file=$1
    local line_num=1

    while IFS= read -r line; do
        local line_length=${#line}
        if [ "$line_length" -gt 80 ]; then
            echo "$file:$line_num: line too long ($line_length > 80 characters)" >> "$REPORT_FILE"
        fi
        ((line_num++))
    done < "$file"
}

# Check function count (max 5 functions per file)
check_function_count() {
    local file=$1

    local func_count=$(grep -c "^[a-zA-Z0-9_]\+[[:space:]]*(" "$file")
    if [ "$func_count" -gt 5 ]; then
        echo "$file:1: too many functions ($func_count > 5)" >> "$REPORT_FILE"
    fi
}

# Check function length (max 25 lines)
check_function_length() {
    local file=$1

    # Extract function definitions with line numbers
    grep -n '^[a-zA-Z0-9_]\+[[:space:]]*(' "$file" | while read -r func_line; do
        # Get function name and line number
        local line_num=$(echo "$func_line" | cut -d: -f1)
        local func_name=$(echo "$func_line" | cut -d: -f2 | sed -E 's/^([a-zA-Z0-9_]+).*/\1/')

        # Find opening brace line
        local brace_line=$(tail -n +$line_num "$file" | grep -n "{" | head -1 | cut -d: -f1)
        brace_line=$((line_num + brace_line - 1))

        # Find matching closing brace
        local end_line=0
        local brace_count=1
        local cur_line=$brace_line

        while read -r line; do
            ((cur_line++))
            if [[ "$line" == *"{"* ]]; then
                ((brace_count++))
            fi
            if [[ "$line" == *"}"* ]]; then
                ((brace_count--))
                if [[ $brace_count -eq 0 ]]; then
                    end_line=$cur_line
                    break
                fi
            fi
        done < <(tail -n +$((brace_line + 1)) "$file")

        if [[ $end_line -gt 0 ]]; then
            local func_length=$((end_line - line_num + 1))
            if [[ $func_length -gt 25 ]]; then
                echo "$file:$line_num: function $func_name too long ($func_length > 25 lines)" >> "$REPORT_FILE"
            fi
        fi
    done
}

# Main check function
check_file() {
    local file=$1

    echo "Checking $file..."
    check_trailing_spaces "$file"
    check_tabs "$file"
    check_line_length "$file"
    check_function_count "$file"
    check_function_length "$file"
}

# Find all C files and check them
find . -type f \( -name "*.c" -o -name "*.h" \) ! -path "*/tests/*" ! -path "*/bonus/*" ! -path "*/documentation/*" ! -path "*/lib/*" | while read -r file; do
    if [ -f "$file" ]; then
        check_file "$file"
    fi
done

# Filter out documentation directory
grep -v "./documentation" "$REPORT_FILE" > new-coding.log
cat new-coding.log

# Count the number of errors
ERROR_COUNT=$(wc -l < new-coding.log)

# Inform user of completion
if [ "$ERROR_COUNT" -eq 0 ]; then
    echo "C coding style check completed. No errors found!"
else
    echo "C coding style check completed. Found $ERROR_COUNT error(s)."
fi
