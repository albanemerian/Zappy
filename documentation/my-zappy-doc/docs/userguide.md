# The Zappy - User Guide

## 🎯 About The Project

Zappy is an exciting network-based strategy game where multiple teams compete on a tile-based map filled with resources. The objective is strategic: be the first team to get **at least 6 players** to reach the **maximum elevation level**.

### ✨ Key Features

- 🌐 **Multiplayer Network Game** - Real-time competition between teams
- 🗺️ **Dynamic Tile Map** - Resource-rich environment for strategic gameplay
- 👥 **Team-Based Strategy** - Collaborate with teammates to achieve victory
- 🎮 **Multiple Interfaces** - Server, GUI client, and AI bot components
- 📊 **Real-time Visualization** - Watch the action unfold with the GUI
- 🤖 **AI Integration** - Develop and deploy intelligent bots


## 🏗️ Architecture

The project consists of three main components:

```
📦 Zappy
├── 🖥️  Server      - Core game engine and network management
├── 🎮  GUI Client  - Real-time game visualization interface
└── 🤖  AI Bot      - Intelligent automated players
```

## Prerequisites

Before running Zappy, ensure you have:

- **C/C++ Compiler** (gcc/g++)
- **Python 3.x**
- **Make** build system
- **PDF-LaTeX** (for documentation generation)
- **Raylib** graphics library
- **SFML** audio library

### 🔧 Installation


1. **Install RayLib** (only if you don't already have the lib)
   **Ubuntu**:
    ```bash 
    apt install libraylib-dev
   ```  
   **Other distributions**:
   ```bash
    git clone https://github.com/raysan5/raylib.git raylib
    cd raylib
    mkdir build && cd build
    cmake -DSHARED=ON -DBUILD_EXAMPLES=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=ON ..
    make -j$(nproc)
    make install
    ldconfig
    cd ../..
    ```

2. **Install SFML** (only if you don't already have the lib)
   **Ubuntu**:
    ```bash 
    apt install libsfml-dev
   ``` 
   **Other distributions**:
   ```bash
    wget https://github.com/SFML/SFML/archive/refs/heads/2.3.x.zip
    unzip 2.3.x.zip && cd 2.3.x
    mkdir build && cd build 
    cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON
    make -j$(nproc)
    make install
    ```

3. **Clone the repository**
   ```bash
   git clone <repository-url>
   cd zappy
   ```

4. **Build all components**
   ```bash
   make
   ```
   This will compile:
    - `zappy_server` - The game server
    - `zappy_gui` - The graphical interface
    - `zappy_ai` - The AI bot

## How to run the project

To run the project you need to start multiple elements

1 - You need to start the server the servr will handle the messaages from the ai client and the gui. Here is how you start it :
    ```
    ./zappy_server -p <port> -x <width> -y <height> -n <team1> <team2> ... -c <nb_clients> -f <freq>
    ```
    `<port>` = The port on wich to start the server

    `<width>` = The width of the map

    `<height>` = The height of the map

    `<team>` = Here you specify the name of all the team you want to run

    `<nb_clients>` = The number of client each team will hold

    `<freq>` = The frequence by wich the action will be divided, this parameter is not mandatory and will be set at 100 if not specified

2 - If you want a graphical display of what is happening you will need to start the gui :

    ```
    ./zappy_gui -p <port> -h <hostname>
    ```
    `<port>` = The port that the server was started on

    `<hostname>` = The IP on wich the server was started if it was one the same machine `localhost` works

3 - Then you wiil need to start ai clients, one for each team you defined in the server CLI :

    ```
    ./zappy_ai -p <port> -n <team_name> -h <hostname>
    ```
    `<port>` = The port that the server was started on

    `<n>` = The name the ai will joins

    `<hostname>` = The IP on wich the server was started if it was one the same machine `localhost` works
