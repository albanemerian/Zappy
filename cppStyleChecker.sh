#!/bin/bash

LOG_FILE="coding-style-cpp-reports.log"
> "$LOG_FILE"

FILTERS="+whitespace/line_length,+whitespace/operators,+whitespace/tab,+whitespace/empty_if_body,-readability/braces,+readability/casting,+build/include_order"

NOFILTERS="-readability/namespace,-legal/copyright,-build/include_subdir,-whitespace/braces,-runtime/string,-whitespace/parens,-whitespace/indent_namespace,-build/header_guard,-whitespace/indent,-build/c++17,-runtime/explicit"

LINELENGTH=95

find . -type f \( -name "*.cpp" -o -name "*.hpp" \) ! -path "*/tests/*" ! -path "*/bonus/*" ! -path "*/temp/*" | while read -r file; do
    cpplint --filter="$FILTERS,$NOFILTERS" --linelength=$LINELENGTH "$file" 2>> "$LOG_FILE"
done

echo "Coding style check completed. See $LOG_FILE for details."
