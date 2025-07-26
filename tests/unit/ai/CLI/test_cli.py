##
## EPITECH PROJECT, 2025
## B-YEP-400-NAN-4-1-zappy-albane.merian
## File description:
## test_cli
##

import pytest
import sys
import os

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../../ai/')))

from src.CLI.CLI import CLI
from src.Exceptions.Exceptions import (
    CLIPortException,
    CLINameException,
    CLIMachineException,
    CLIMissingArgumentException,
    CLIInvalidArgumentException
)

class TestCLI:
    def test_parse_args_valid(self):
        """Test parsing valid command line arguments"""
        cli = CLI()
        args = ["zappy_ai", "-p", "4242", "-n", "team1", "-h", "localhost"]
        result = cli.parse_args(args)

        assert result["port"] == 4242
        assert result["name"] == "team1"
        assert result["machine"] == "127.0.0.1"

    def test_parse_args_valid_ip(self):
        """Test parsing valid IP address"""
        cli = CLI()
        args = ["zappy_ai", "-p", "4242", "-n", "team1", "-h", "192.168.1.1"]
        result = cli.parse_args(args)

        assert result["port"] == 4242
        assert result["name"] == "team1"
        assert result["machine"] == "192.168.1.1"

    def test_parse_args_invalid_option(self):
        """Test parsing invalid option"""
        cli = CLI()
        args = ["zappy_ai", "-x", "4242", "-n", "team1", "-h", "localhost"]

        with pytest.raises(CLIInvalidArgumentException) as excinfo:
            cli.parse_args(args)
        assert "Unknown option: -x" in str(excinfo.value)

    def test_parse_args_missing_value(self):
        """Test parsing missing value for option"""
        cli = CLI()
        args = ["zappy_ai", "-p", "4242", "-n", "team1", "-h"]

        with pytest.raises(CLIMissingArgumentException) as excinfo:
            cli.parse_args(args)
        assert "Missing value for option: -h" in str(excinfo.value)

    def test_parse_args_not_enough_args(self):
        """Test parsing not enough arguments"""
        cli = CLI()
        args = ["zappy_ai", "-p", "4242"]

        with pytest.raises(CLIInvalidArgumentException) as excinfo:
            cli.parse_args(args)
        assert "Not enough arguments provided" in str(excinfo.value)

    def test_parse_port_invalid(self):
        """Test parsing invalid port"""
        cli = CLI()

        with pytest.raises(CLIPortException) as excinfo:
            cli.parse_port("abc")
        assert "Port must be a valid number" in str(excinfo.value)

    def test_parse_port_negative(self):
        """Test parsing negative port"""
        cli = CLI()

        with pytest.raises(CLIPortException) as excinfo:
            cli.parse_port("-1")
        assert "Port must be a positive integer" in str(excinfo.value)

    def test_parse_port_too_large(self):
        """Test parsing port that is too large"""
        cli = CLI()

        with pytest.raises(CLIPortException) as excinfo:
            cli.parse_port("65536")
        assert "Port must be less than or equal to 65535" in str(excinfo.value)

    def test_parse_name_empty(self):
        """Test parsing empty team name"""
        cli = CLI()

        with pytest.raises(CLINameException) as excinfo:
            cli.parse_name("")
        assert "Team name cannot be empty" in str(excinfo.value)

    def test_parse_name_whitespace(self):
        """Test parsing whitespace team name"""
        cli = CLI()

        with pytest.raises(CLINameException) as excinfo:
            cli.parse_name("   ")
        assert "Team name cannot be empty" in str(excinfo.value)

    def test_parse_machine_empty(self):
        """Test parsing empty machine name"""
        cli = CLI()

        with pytest.raises(CLIMachineException) as excinfo:
            cli.parse_machine("")
        assert "Machine name cannot be empty" in str(excinfo.value)

    def test_parse_machine_invalid_ip_format(self):
        """Test parsing invalid IP format"""
        cli = CLI()

        with pytest.raises(CLIMachineException) as excinfo:
            cli.parse_machine("192.168.1")
        assert "IP address must have exactly 4 segments" in str(excinfo.value)

    def test_parse_machine_invalid_ip_value(self):
        """Test parsing invalid IP value"""
        cli = CLI()

        with pytest.raises(CLIMachineException) as excinfo:
            cli.parse_machine("192.168.1.256")
        assert "IP address segments must be between 0 and 255" in str(excinfo.value)

    def test_parse_machine_invalid_ip_chars(self):
        """Test parsing IP with invalid characters"""
        cli = CLI()

        with pytest.raises(CLIMachineException) as excinfo:
            cli.parse_machine("192.168.1.abc")
        assert "Invalid machine name or IP address" in str(excinfo.value)

    def test_validate_config_missing_port(self):
        """Test validating config with missing port"""
        cli = CLI()

        with pytest.raises(CLIMissingArgumentException) as excinfo:
            cli.validate_config(False, True)
        assert "Missing required option: -p port" in str(excinfo.value)

    def test_validate_config_missing_name(self):
        """Test validating config with missing name"""
        cli = CLI()

        with pytest.raises(CLIMissingArgumentException) as excinfo:
            cli.validate_config(True, False)
        assert "Missing required option: -n name" in str(excinfo.value)
