# The Zappy - AI

## 🎯 Overview

The Zappy AI client is an intelligent automated player system that connects to the Zappy server to compete in the strategic multiplayer game. Built in Python, it implements advanced gameplay strategies including resource management, team coordination, and automatic player spawning to achieve the victory condition of elevating 6 players to maximum level.

## 🏗️ Architecture Overview

The AI client follows a modular, multi-process architecture with sophisticated state management:

```
📦 Zappy AI
├── 🚀 App Management       - Process orchestration and lifecycle
├── 🤖 Player Intelligence  - Advanced game logic and decision making  
├── 🌐 Communication        - Threaded server protocol and messaging
├── 📡 Broadcasting         - E## 🎯 Victory Strategy
```

The AI achieves victory through coordinated multi-phase strategy:

### Core Strategies

#### 1. **Resource Collection**
- **Smart Prioritization**: Focuses on stones needed for current level
- **Team Coordination**: Real-time inventory sharing across all players
- **Food Management**: Ensures survival during long incantations (300+ time units)

#### 2. **Level Progression** 
- **Coordinated Incantations**: Multi-player synchronized advancement
- **Leadership System**: Dynamic leader election for incantation coordination
- **Resource Preparation**: Pre-positioning stones and players efficiently

#### 3. **Team Management**
- **Encrypted Communication**: Secure messages prevent enemy interference
- **Load Balancing**: Dynamic task distribution across team members
- **Process Scaling**: Automatic player spawning for all available slots

#### 4. **Victory Condition**
- **Goal**: Elevate 6 players to maximum level (8)
- **Progressive Strategy**: Systematic advancement through levels 1-8
- **Competitive Edge**: Superior coordination outperforms enemy teams

### Tactical Advantages
- **Map Awareness**: Adapts exploration to map size and boundaries
- **Enemy Detection**: Recognizes and responds to competing teams
- **Resource Denial**: Strategic collection to limit enemy progress
- **Failure Recovery**: Robust fallback strategies maintain team momentum coordination and messaging
├── 🔐 Hash System          - Secure team communication with replay protection
├── 🖥️  CLI Parser          - Robust command line argument handling
├── 📊 Logger System        - Comprehensive logging and debugging
├── ⚙️  Config System       - Game configuration and constants
└── 🔧 Exception Handling   - Comprehensive error management

## 🚀 Entry Point and Application Management

### Main Entry Point
The AI starts from [`main.py`](ai/src/main.py):

```python
def main():
    logger = Logger()

    if len(argv) == 2 and argv[1] == "-help":
        logger.help(USAGE_STRING)
        return SUCCESS

    try:
        cli = CLI()
        config = cli.parse_args(argv)

        logger.success(
            "AI initialized with configuration: "
            f"Port: {config['port']}, "
            f"Team name: {config['name']}, "
            f"Machine: {config['machine']}"
        )

        app = App(config)
        return app.run()

    except CLIParsingException as e:
        logger.error(f"CLI Parsing Error: {e}")
        logger.help(USAGE_STRING)
        return FAILURE

    except KeyboardInterrupt:
        logger.info(f"\nReceived keyboard interrupt, shutting down...")
        return SUCCESS
```

### Application Orchestration
From [`App.py`](ai/src/App/App.py):

The App class manages the entire AI team lifecycle:

```python
class App:
    def __init__(self, config: dict[str]):
        self.port = config["port"]
        self.name = config["name"]
        self.ip = config["machine"]
        self.childs: list[int] = []
        self.running = True
        self.is_main_process = True
        self.logger = Logger()

        if self.is_main_process:
            signal.signal(signal.SIGINT, self._signal_handler)
            signal.signal(signal.SIGTERM, self._signal_handler)
```

#### Multi-Process Architecture
The AI uses a sophisticated fork-based multi-process system with advanced process management:

1. **Main Process**: Coordinates team, connects to server, and spawns child processes
2. **Child Processes**: Individual AI players with independent logic and communication
3. **Signal Handling**: Graceful shutdown with proper cleanup of all processes
4. **Process Monitoring**: Tracks child process status and ensures proper termination

```python
def create_new_player(self) -> int:
    pid: int = os.fork()
    if pid < 0:
        return -1
    if pid == 0:  # Child process
        self.is_main_process = False
        signal.signal(signal.SIGINT, self._child_signal_handler)
        signal.signal(signal.SIGTERM, self._child_signal_handler)

        try:
            p = Player(self.name, self.ip, self.port)
            p.is_child_process = True
            _, x, y = p.communication.connectToServer()
            p.setMapSize(x, y)
            p.startComThread()
            p.loop()
        except (CommunicationException, SocketException):
            exit(FAILURE)
        except KeyboardInterrupt:
            exit(SUCCESS)
        except Exception:
            exit(FAILURE)
        exit(SUCCESS)
    return pid  # Parent receives child PID
```

#### Advanced Process Management
The App class includes sophisticated process cleanup and monitoring:

```python
def run(self):
    self.logger.success(f"Starting Zappy AI for team: {self.name}...")
    player = Player(self.name, self.ip, self.port)
    slots, x, y = 0, 0, 0

    try:
        slots, x, y = player.communication.connectToServer()
    except (CommunicationException, SocketException) as e:
        self.logger.error(f"Failed to connect to server: {e}")
        return FAILURE

    player.setMapSize(x, y)
    player.setNbSlots(slots + 1)

    # Spawn child processes for available slots
    for _ in range(slots):
        if not self.running:
            break
        child_pid = self.create_new_player()
        if child_pid > 0:
            self.childs.append(child_pid)

    # Main player execution with proper cleanup
    try:
        player.startComThread()
        while self.running:
            try:
                player.loop()
                break
            except KeyboardInterrupt:
                break
    except (CommunicationException, SocketException):
        self.logger.error(f"Server connection lost for team {self.name}")
        self._cleanup_children()
        return FAILURE
    finally:
        if self.is_main_process:
            self._wait_for_children()
            self.logger.info(f"AI team {self.name} finished")

    return SUCCESS
```

## 🤖 Player Intelligence System

### Player Architecture
From [`Player.py`](ai/src/Player/Player.py):

Each AI player is an autonomous agent with advanced state management:

```python
class Player:
    def __init__(self, name: str, ip: str, port: int = 4242) -> None:
        self.communication: Communication = Communication(name, ip, port)
        self.commThread: Thread = Thread(
            target=self.communication.loop,
            name=f"CommunicationThread-{name}"
        )
        
        self.logger = Logger()
        self.broadcaster: Broadcaster = Broadcaster(self.communication, name)
        
        self.level: int = 1
        self.inventory: dict[str, int] = {
            "food": 10, "linemate": 0, "deraumere": 0,
            "sibur": 0, "mendiane": 0, "phiras": 0, "thystame": 0
        }
        
        # Advanced state management
        self.roombaState: dict = {
            "forwardCount": 0, "targetForward": 10,
            "phase": "lookAround", "lastCommand": None,
            "lastPhase": None, "commandSentTime": 0,
            "highestPidSeen": 0
        }
        
        self.incantationState: dict = {
            "status": False, "phase": "sendComeIncant",
            "lastCommand": None, "playerResponses": []
        }
        
        self.goToIncantationState: dict = {
            "status": False, "steps": [], "lastCommand": None,
            "direction": 0, "arrived": False, "movementStarted": False,
            "droppingStones": False, "needToWait": False
        }
```

### Level Progression System
The AI implements a sophisticated leveling system based on resource requirements from [`GameConfig.py`](ai/src/Config/GameConfig.py):

```python
LVL_UPGRADES = {
    1: {
        "players": 1,
        "stones": {
            "linemate": 1, "deraumere": 0, "sibur": 0,
            "mendiane": 0, "phiras": 0, "thystame": 0
        }
    },
    2: {
        "players": 2,
        "stones": {
            "linemate": 1, "deraumere": 1, "sibur": 1,
            "mendiane": 0, "phiras": 0, "thystame": 0
        }
    },
    3: {
        "players": 2,
        "stones": {
            "linemate": 2, "deraumere": 0, "sibur": 1,
            "mendiane": 0, "phiras": 2, "thystame": 0
        }
    },
    # ... continues for levels 4-7
}

TOTAL_NEEDED_STONES = {
    "linemate": 9, "deraumere": 8, "sibur": 10,
    "mendiane": 5, "phiras": 6, "thystame": 1
}

MAX_LEVEL = 8
ELEVATION_COST = 300  # Time units for incantation
FOOD_VALUE = 126      # Time units of life per food
```

### Advanced Roomba Strategy
The AI implements a sophisticated "Roomba" exploration strategy with multiple phases:

- **Look Around**: Scans the current tile for resources
- **Vacuum**: Collects food and needed stones, prioritizing survival
- **Forward Movement**: Moves in straight lines until hitting boundaries
- **Turn**: Changes direction when reaching map edges
- **Team Coordination**: Checks teammate status for incantation readiness

The strategy intelligently adapts based on team needs and available resources.

### Resource Prioritization and Team Strategy
The AI implements intelligent resource management with team coordination:

- **Priority Collection**: Focuses on stones needed for current level progression
- **Team Inventory Tracking**: Aggregates resources across all team members
- **Food Management**: Ensures sufficient food for incantation duration (300 time units)
- **Resource Sharing**: Coordinates stone distribution for optimal team advancement

Key features include level-specific stone requirements and team-wide resource planning to ensure successful incantations.

## 🌐 Communication System

### Threaded Communication System
From [`Communication.py`](ai/src/Communication/Communication.py):

The communication system handles all server interactions with threaded processing for optimal performance:

- **Asynchronous Processing**: Separate thread handles all network I/O
- **Queue Management**: Request, response, and message queues for organized communication
- **Thread Safety**: Mutex-protected operations for concurrent access
- **Connection Management**: Automatic connection handling and error recovery

### Command System
The AI supports all server commands with sophisticated response handling:

- **Movement Commands**: `sendForward()`, `sendLeft()`, `sendRight()`
- **Resource Commands**: `sendTakeObject(obj)`, `sendSetObject(obj)`
- **Information Commands**: `sendLook()`, `sendInventory()`, `sendGetConnectNbr()`
- **Advanced Commands**: `sendIncantation()`, `sendFork()`, `sendEject()`
- **Communication**: `sendBroadcast(message)`

### Advanced Response Processing
The AI processes server responses with intelligent command-response mapping:

- **Response Classification**: Automatically categorizes different server response types
- **Context-Aware Handling**: Maintains command context for error reporting
- **State Updates**: Updates player state based on server responses
- **Error Recovery**: Handles failures gracefully with fallback strategies

Supported responses include inventory updates, look results, movement confirmations, incantation status, and level progression notifications.

## 📡 Broadcasting and Team Coordination

### Advanced Team Communication
From [`Broadcaster.py`](ai/src/Broadcaster/Broadcaster.py):

The broadcasting system enables secure team coordination:

- **Message Encryption**: Team-specific hash algorithms prevent enemy interception
- **Replay Protection**: Message indexing prevents replay attacks
- **Protocol System**: Structured message types for different coordination needs
- **Direction-Based Communication**: Uses broadcast direction for position coordination

### Message Protocol System
The AI supports various message types for team coordination:

- `teamslots`: Share available team slots information
- `sendInventory` / `inventory`: Request and share inventory status
- `comeIncant` / `whereAreYou` / `here`: Incantation coordination messages
- `dropStones` / `leadIncantation`: Resource and leadership coordination
- `goRoombas`: Emergency fallback to exploration mode

### Message Encryption
From [`Hash.py`](ai/src/Hash/Hash.py):

Team messages are encrypted to prevent enemy teams from intercepting communications:

- **Team-specific encryption**: Each team has unique hash algorithms
- **Message indexing**: Prevents replay attacks and ensures message ordering
- **Secure broadcasting**: Only team members can decrypt messages

## 🖥️ Command Line Interface

### Robust Argument Parsing
From [`CLI.py`](ai/src/CLI/CLI.py):

The CLI system provides comprehensive command line argument validation:

- **Required Parameters**: Port (`-p`) and team name (`-n`) are mandatory
- **Optional Hostname**: Machine address (`-h`) defaults to localhost
- **Validation**: Port range (1-65535), non-empty team names, valid hostnames
- **Error Handling**: Detailed error messages for invalid arguments
- **Help System**: Built-in help with `-help` option

### Exception System
The AI includes comprehensive exception handling:

- **CLI Exceptions**: Invalid arguments, missing parameters, port/hostname errors
- **Communication Exceptions**: Network errors, handshake failures, socket issues
- **Game Exceptions**: Player death, incantation failures, resource conflicts
- **Recovery Strategies**: Graceful fallbacks and error reporting for all scenarios

## 📊 Logging and Debugging

### Comprehensive Logging System
From [`Logger.py`](ai/src/Logger/Logger.py):

The AI includes a sophisticated logging system with:

- **Colored output**: Different colors for different log levels
- **Timestamping**: Precise timing information for debugging
- **Multi-level logging**: Success, error, info, warning, and debug messages
- **Process identification**: Distinguishes between main and child process logs

## 🎮 Game Loop and Strategy

### Main Game Loop
Each AI player runs a sophisticated continuous game loop with three main operational modes:

1. **Message Processing**: Handles team coordination messages and server responses
2. **State Management**: Tracks team slots, player connections, and game status  
3. **Behavior Execution**: Executes appropriate strategy based on current state

The AI switches between three primary behaviors:
- **Roomba Mode**: Resource collection and map exploration
- **Incantation Mode**: Leading team incantations with player coordination
- **Go-To-Incantation Mode**: Assisting other players' incantations

### Incantation Strategy
The AI coordinates complex multi-player incantations through:

- **Leadership Election**: Highest PID player becomes incantation leader
- **Player Gathering**: Coordinates teammate movement to incantation location
- **Resource Coordination**: Ensures all required stones are present
- **Timing Management**: Synchronizes incantation start across all players
- **Fallback Handling**: Returns to exploration if incantation fails

## 🚀 Advanced Features

### Multi-State Intelligence System
The AI implements a sophisticated three-state system for optimal gameplay:

1. **Roomba Mode**: Autonomous resource collection with intelligent pathfinding
2. **Incantation Mode**: Leading coordinated team incantations
3. **Go-To-Incantation Mode**: Supporting teammate incantations

### Key Features

#### Dynamic Navigation
- **Direction-Based Movement**: 8-directional movement calculations from broadcast signals
- **Optimal Pathfinding**: Shortest path calculation to teammate locations
- **Boundary Awareness**: Intelligent map edge detection and turning

#### Process Management
- **Multi-Process Architecture**: Spawns players for all available server slots
- **Signal Handling**: Graceful shutdown with SIGINT/SIGTERM support
- **Resource Cleanup**: Automatic process termination and resource management
- **Exception Recovery**: Robust error handling across all process levels

#### Team Coordination Features
- **Leadership Election**: Automatic leader selection based on player priority
- **Real-Time Inventory Sharing**: Continuous resource status updates
- **Position Tracking**: Direction-based teammate location coordination
- **Emergency Protocols**: Fallback strategies for failed operations

## 🔧 Configuration and Usage

### Running the AI
```bash
./zappy_ai -p <port> -n <team_name> [-h <hostname>]
```

### Parameters
- **`-p port`**: Server port number (required, 1-65535)
- **`-n team_name`**: Team name for identification (required, non-empty)
- **`-h hostname`**: Server hostname (optional, defaults to "127.0.0.1")

### Help and Usage
```bash
./zappy_ai -help
```

### Team Coordination
Multiple AI instances can run for the same team with advanced coordination:
- Each instance automatically spawns players for all available server slots
- Players coordinate through encrypted broadcasting with replay protection
- Shared strategy with real-time resource distribution and planning
- Leadership election system for incantation coordination
- Dynamic load balancing based on player performance and resources

## 🎯 Victory Strategy

The AI's ultimate goal is achieving victory through:

1. **Resource Collection**: Efficient gathering of required stones
2. **Level Progression**: Strategic incantations to advance levels
3. **Team Coordination**: Synchronized efforts for maximum efficiency
4. **Survival Management**: Maintaining food supply for all players
5. **Player Spawning**: Creating new players through forking
6. **Victory Condition**: Getting 6 players to maximum level (8)

## 🧪 Testing and Quality Assurance

### Testing Infrastructure
The AI includes comprehensive testing coverage:

- **Unit Tests**: CLI parsing, communication protocols, player behavior validation
- **Integration Tests**: Multi-process coordination, server interaction, team coordination
- **Performance Tests**: Load testing, memory profiling, latency optimization
- **Quality Metrics**: Code coverage analysis, static analysis, continuous integration

### Quality Features
- **Comprehensive Logging**: Detailed debugging and monitoring capabilities
- **Error Handling**: Robust exception management throughout all components
- **State Validation**: Consistency checks across all game states and operations
- **Resource Management**: Proper cleanup and memory management validation

The Zappy AI demonstrates advanced software engineering principles with sophisticated strategies for competitive automated gameplay. Its modular architecture enables easy maintenance and future enhancements while providing superior performance in tournament environments.
