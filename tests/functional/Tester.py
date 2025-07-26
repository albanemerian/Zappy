##
## EPITECH PROJECT, 2025
## tekspice
## File description:
## tester
##

from Parser import *
from Colors import *
from Utils import *
from TestCase import *

import time

CONFIG_FILE = "tests/functional/config.json"

def main():
    parser = Parser()
    tests = parser.getTests()

    total = len(tests)
    passed = 0
    count = 0
    start_time = time.time()

    for test in tests:
        test.execute()
        if test.check():
            test.displayPassed(count)
            passed += 1
        else:
            test.displayFailed(count)
        count += 1

    end_time = time.time()
    print(f"\n{YELLOW}Synthesis: Tested: {BLUE}{total}{YELLOW}, Passed: {GREEN}{passed}{YELLOW}, Failed: {RED}{total - passed}{END}")
    print(f"{YELLOW}Execution time: {BLUE}{end_time - start_time:.2f}{YELLOW} seconds{END}")

    if passed == total:
        return 0
    return 1

if __name__ == "__main__":
    exit(main())
