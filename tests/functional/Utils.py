##
## EPITECH PROJECT, 2025
## B-NWP-400-NAN-4-1-myftp-eliott.tesnier
## File description:
## Utils
##

import re

def strip_ansi_codes(text):
    ansi_escape_pattern = re.compile(r'\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])')
    return ansi_escape_pattern.sub('', text).replace("\r", "")
