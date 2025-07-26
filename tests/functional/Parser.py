##
## EPITECH PROJECT, 2025
## B-NWP-400-NAN-4-1-myftp-eliott.tesnier
## File description:
## Parser
##

from Colors import *
from TestCase import *
import json

CONFIG_FILE = "tests/functional/config.json"

class Parser:
    def __init__(self):
        self.tests_folder = ""
        self.tests_files_names = []
        self.tests_files = []
        self.output_folder = ""
        self.testsObjects = []

    def run(self):
        self.parseConfig()
        self.parseJsons()

    def parseConfig(self):
        try:
            with open(CONFIG_FILE, "r") as file:
                try:
                    data = json.load(file)
                    self.tests_folder = data["tests_folder"]
                    self.tests_files_names = data["files"]
                    self.output_folder = data["output_folder"]

                    for f in self.tests_files_names:
                        f = self.tests_folder + f + ".json"
                        self.tests_files.append(f)

                except json.JSONDecodeError:
                    print(f"{RED}Error: Invalid JSON format{END}")
                except KeyError as e:
                    print(f"{RED}Error: Missing required field in JSON: {e}{END}")
                except Exception as e:
                    print(f"{RED}Error: {str(e)}{END}")
        except FileNotFoundError:
            print(f"{RED}Error: File not found: {CONFIG_FILE}{END}")

    def parseJsons(self):
        for f in self.tests_files:
            try:
                with open(f, "r") as file:
                    try:
                        data = json.load(file)
                        for test in data["tests"]:
                            name = test["name"]
                            desc = test["description"]
                            output = test["expected_output"]
                            value = test["expected_return"]

                            input_data = test["input"]
                            if isinstance(input_data, str):
                                input_data = input_data.split()
                            elif isinstance(input_data, list):
                                pass
                            elif isinstance(input_data, dict) and "tty_mode" in input_data:
                                pass

                            self.testsObjects.append(TestCase(name, desc, input_data, output, value, self.output_folder))
                    except json.JSONDecodeError:
                        print(f"{RED}Error: Invalid JSON format{END}")
                    except KeyError as e:
                        print(f"{RED}Error: Missing required field in JSON: {e}{END}")
                    except Exception as e:
                        print(f"{RED}Error: {str(e)}{END}")
            except FileNotFoundError:
                print(f"{RED}Error: File not found: {f}{END}")
                continue

    def getTests(self):
        self.run()
        return self.testsObjects
