#!/bin/bash

# This script ensures the Docker socket has correct permissions
# and that the jenkins user can access it

# Check if docker socket exists
if [ ! -S /var/run/docker.sock ]; then
    echo "Error: Docker socket not found at /var/run/docker.sock"
    exit 1
fi

# Get the group ID of the docker socket
DOCKER_GID=$(stat -c '%g' /var/run/docker.sock)

# Check if the docker group exists already
if ! getent group docker > /dev/null; then
    # Create the docker group with the correct GID
    groupadd -g $DOCKER_GID docker
else
    # Update the docker group with the correct GID
    groupmod -g $DOCKER_GID docker
fi

# Add jenkins user to the docker group
usermod -aG docker jenkins

# Test Docker access
su - jenkins -c "docker ps" || {
    echo "Error: Failed to run Docker commands as jenkins user"
    exit 1
}

echo "Docker setup completed successfully"
