##
## EPITECH PROJECT, 2025
## B-YEP-400-NAN-4-1-zappy-albane.merian
## File description:
## run_tests
##

import pytest
import sys
import os

if __name__ == "__main__":
    project_root = os.path.abspath(os.path.join(os.path.dirname(__file__), '../..'))
    sys.path.insert(0, project_root)

    sys.exit(pytest.main(['-v', '--cov=src', '--cov-report=term', '--cov-report=html:coverage_report']))