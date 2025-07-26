FROM jenkins/jenkins:lts-jdk17

ENV JAVA_OPTS="-Djenkins.install.runSetupWizard=false"

COPY plugins.txt /usr/share/jenkins/ref/plugins.txt

RUN jenkins-plugin-cli --plugin-file /usr/share/jenkins/ref/plugins.txt

USER root

RUN mkdir -p /usr/local/bin

RUN apt-get update && apt-get install -y \
    git \
    gcc \
    g++ \
    make \
    cmake \
    python3 \
    python3-pip \
    gcovr \
    libcriterion-dev \
    libgtest-dev \
    libgl1-mesa-dev \
    libx11-dev \
    libxcursor-dev \
    libxrandr-dev \
    libxi-dev \
    libxinerama-dev \
    libxxf86vm-dev \
    libwayland-dev \
    libxkbcommon-dev \
    wayland-protocols \
    xvfb \
    libasound2-dev \
    libopenal-dev \
    libopengl-dev \
    liballegro5-dev \
    valgrind \
    wget  \
    unzip \
    libsfml-dev  \
    alsa-utils  \
    python3-venv && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

RUN python3 -m venv /opt/venv
ENV PATH="/opt/venv/bin:$PATH"

RUN pip install --no-cache-dir --upgrade pip && \
    pip install --no-cache-dir pylint flake8 pycodestyle cpplint

RUN git clone https://github.com/raysan5/raylib.git /tmp/raylib && \
    cd /tmp/raylib && mkdir build && cd build && \
    cmake -DSHARED=ON -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DSUPPORT_FILEFORMAT_WAV=OFF -DSUPPORT_AUDIO=OFF .. && \
    make -j$(nproc) && \
    make install && \
    rm -rf /tmp/raylib

USER jenkins
