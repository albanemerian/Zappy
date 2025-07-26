# Jenkins CI/CD Pipeline for Zappy

This directory contains the configuration for a Jenkins CI/CD pipeline that automates building, testing, and reporting for the Zappy project.

## Features

- **Multi-Job Pipeline**: Sequential jobs organized in branch-specific folders
  - Coding Style Check
  - Build
  - Tests
  - Dashboard
- **Comprehensive Testing**: Unit tests with coverage reports for server, GUI, and AI components
- **Automated Reporting**: Visual dashboards and summary reports

## Setup Instructions

### Requirements

- Docker and Docker Compose
- Git access to the Zappy repository

### Getting Started

1. **Start Jenkins**:
   ```bash
   cd jenkins
   docker-compose up -d
   ```

2. **Access Jenkins**:
   - Open http://localhost:8080 in your browser
   - Jenkins is pre-configured with necessary plugins and jobs

3. **Run the Pipeline**:
   - Navigate to the "Pull Branch from Zappy" job
   - Provide the branch name (default is "main")
   - Click "Build"

## Troubleshooting

### Plugin Issues

If you encounter issues with plugins:

1. Run the plugin diagnostics script inside the container:
   ```bash
   docker exec zappipicaca /var/jenkins_home/plugin_diagnostics.sh
   ```

2. Verify the plugins are properly installed:
   ```bash
   docker exec zappipicaca ls -la /var/jenkins_home/plugins/
   ```

### Fork Test Issues

The pipeline automatically disables problematic fork tests that cause crashes in the Jenkins environment. This is handled by the `fix_fork_tests.sh` script.

### Coverage Issues

If tests fail but you still want coverage reports, the pipeline uses `run_coverage_with_workaround.sh` to ensure coverage reports are generated even if some tests fail.

## Custom Scripts

- **fix_fork_tests.sh**: Disables problematic fork tests in Jenkins
- **fix_audio_issues.sh**: Addresses audio driver issues in GUI tests
- **run_coverage_with_workaround.sh**: Generates coverage even when tests fail
- **pipeline_summary.sh**: Creates textual summary of pipeline execution
- **pipeline_visualization.sh**: Creates visual ASCII representation of pipeline
- **plugin_diagnostics.sh**: Diagnoses plugin installation issues
- **memory_diagnostics.sh**: Reports memory usage and leaks

## Maintenance

### Adding New Jobs

To add a new job to the pipeline:

1. Update the `job_dsl.groovy` file with your new job definition
2. Ensure proper downstream triggering for sequential execution
3. Rebuild the "Pull Branch from Zappy" job

### Updating Plugins

To update or add plugins:

1. Add the plugin to `plugins.txt`
2. Rebuild the Docker image:
   ```bash
   docker-compose down
   docker-compose build --no-cache
   docker-compose up -d
   ```
