<div align="center">

# 🎮 ZAPPY

![Zappy Logo](assets/zappy.png)

**A multiplayer network strategy game where teams compete for supremacy!**

[![License](https://img.shields.io/badge/License-EPITECH-blue?style=for-the-badge)](LICENSE)
[![Languages](https://img.shields.io/badge/Languages-C%2B%2B%20%7C%20C%20%7C%20Python-orange?style=for-the-badge)](#)

</div>

---

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

### 🛠️ Technologies Used

| Component | Language | Framework/Libraries |
|-----------|----------|-------------------|
| Server    | C        | Custom networking |
| GUI       | C++      | Graphics libraries |
| AI Bot    | Python   | Socket programming |

---

## 🚀 Quick Start

### Prerequisites

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

5. **Run the game**
   
   **Start the server:**
   ```bash
   ./zappy_server -p <port> -x <width> -y <height> -n <team1> <team2> ... -c <nb_clients> -f <freq>
   ```
   
   **Launch the GUI:**
   ```bash
   ./zappy_gui -p <port> -h <hostname>
   ```
   
   **Deploy AI team:**
   ```bash
   ./zappy_ai -p <port> -n <team_name> -h <hostname>
   ```

---

## 📚 Documentation

### 📖 Docusaurus Documentation

Start the interactive documentation:

```bash
cd documentation/my-zappy-doc
npx docusaurus start
```

> **💡 Troubleshooting:** If you encounter `npm error could not determine executable to run`, run:
> ```bash
> npm install --save-dev @docusaurus/types
> ```

### 📄 PDF Documentation (Doxygen)

Generate comprehensive PDF documentation:

> **⚠️ Important:** Move the `my-zappy-doc` folder out of the repository before generation due to Unicode emoji conflicts.

```bash
./generateDoc.sh
```

**Requirements:** Ensure `pdf-latex` library is installed on your system.

---

## 🤝 Contributing

We follow a structured commit convention to maintain code quality and project organization.

### 📝 Commit Convention

**Format:** `[Gitmoji] : [Element/Module] : [MESSAGE]`

- **Gitmoji**: Appropriate emoji for the modification type
- **Element/Module**: The component you modified
- **MESSAGE**: Detailed description of changes

### 🎨 Gitmoji Reference

#### 💻 Code Features
| Emoji | Code | Usage |
|-------|------|-------|
| ✨ | `:sparkles:` | Introduce new features |
| ♻️ | `:recycle:` | Refactor/update code |
| 🐛 | `:bug:` | Fix a bug |
| 💩 | `:poop:` | Remove coding style errors or temporary fix |
| 🚨 | `:rotating_light:` | Fix compiling warnings |
| 🔥 | `:fire:` | Remove code or files |

#### 🧪 Testing
| Emoji | Code | Usage |
|-------|------|-------|
| ✅ | `:white_check_mark:` | Add, update, or pass tests |

#### 🏗️ Architecture
| Emoji | Code | Usage |
|-------|------|-------|
| 🙈 | `:see_no_evil:` | Add or update .gitignore files |
| 👷 | `:construction_worker:` | Add or update CI build system |
| 🏗️ | `:building_construction:` | Make architectural changes |
| 📝 | `:memo:` | Add or update documentation |

#### 🔄 Pull Requests
| Emoji | Code | Usage |
|-------|------|-------|
| 🎉 | `:tada:` | **Must be used for each PR created!** |
| 💄 | `:lipstick:` | **Must be used for each PR merged!** |
| ⏪️ | `:rewind:` | **Must be used for each revert done!** |

---

## 🛠️ Git Commands Reference

### 🔄 Commit Management

**Modify commit message (before push):**
```bash
git commit --amend -m "New commit message"
```

**Modify commit message (after push):**
```bash
git commit --amend -m "New commit message"
git push --force
```

### 📂 File Management

**Unstage accidentally added file (not yet pushed):**
```bash
git restore --staged <file>
```

**Remove file from commit (after commit):**
```bash
git reset --soft HEAD~1
git restore --staged file-to-remove.txt
git commit -m "New commit message (without the file)"
```

---

## 🧪 Testing

Run the comprehensive test suite:

```bash
# Unit tests
make tests_run

# Functional tests
cd tests/functional
python3 Tester.py
```

**Coverage reports** are automatically generated in `coverage_report/`.

---

## 🤖 Jenkins CI/CD

This project includes a fully configured Jenkins pipeline to automate building, testing, and quality checking.

### 🚀 Getting Started with Jenkins

1. **Start the Jenkins container**:
   ```bash
   make jenkins
   ```

2. **Access the Jenkins interface**:
   - Open http://localhost:8080 in your browser
   - Login with credentials (check the `.env` file or ask a team member)

3. **Run the pipeline**:
   - Navigate to the "Pull Branch from Zappy" job
   - Enter your branch name (default is "main")
   - Click "Build"

4. **Stop the Jenkins container**:
   ```bash
   make jenkins_stop
   docker-compose -f jenkins/docker-compose.yml down -v // to remove volumes
   ```

### 📋 Pipeline Jobs

The CI/CD pipeline consists of the following sequential jobs:

#### 1️⃣ Coding Style Check
- **Purpose**: Verifies adherence to Epitech coding standards
- **Components Checked**:
  - C coding style (`cStyleChecker.sh`)
  - C++ coding style (`cppStyleChecker.sh`)
  - Python coding style (`pythonStyleChecker.sh`)
- **Trigger**: Automatic on each commit or manual execution

#### 2️⃣ Build
- **Purpose**: Compiles all project components
- **Steps**:
  - Build server (`make zappy_server`)
  - Build GUI (`make zappy_gui`)
  - Build AI (`make zappy_ai`)
  - Verify clean/rebuild works (`make clean`, `make fclean`, `make re`)
- **Trigger**: Automatic after successful style check

#### 3️⃣ Tests
- **Purpose**: Runs comprehensive test suite with coverage
- **Features**:
  - Sets up testing environment (GUI tests, audio configuration)
  - Runs unit tests with coverage reporting
  - Fixes common testing issues automatically
- **Trigger**: Automatic after successful build

#### 4️⃣ Dashboard
- **Purpose**: Generates reports and visualizations
- **Outputs**:
  - Pipeline summary report
  - Visual pipeline progress representation
  - Test results and coverage statistics
- **Trigger**: Automatic after tests (even on failure)

### 🔧 Troubleshooting

If the pipeline fails:

1. **Check the console output** for the failing job
2. **View artifact reports** for detailed error information
3. **Run specific diagnostic scripts**:
   ```bash
   # From host machine
   docker exec zappipicaca /var/jenkins_home/plugin_diagnostics.sh
   docker exec zappipicaca /var/jenkins_home/memory_diagnostics.sh
   ```

### 📊 Viewing Reports

- Access coverage reports and artifacts from the Jenkins job page
- Click on "Artifacts" in the left sidebar of a completed job
- Download and view generated reports locally

---

## 👥 Team

**Project developed by EPITECH students**

- [Eliott Tesnier](https://github.com/eliotttesnier)
- [Albane Merian](https://github.com/albanemerian)
- [Nolan Papa](https://github.com/Nolfews)
- [Matisse Marsac](https://github.com/Matisse-M)
- [Alban Roussée](https://github.com/albanrss)
- [Noa Roussière](https://github.com/nduboi)
