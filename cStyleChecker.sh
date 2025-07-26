#!/bin/bash

make fclean
coding-style . .
grep -v "./documentation" coding-style-reports.log > new-coding.log
cat new-coding.log
