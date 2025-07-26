# The Zappy - Server

## 🎯 Overview

The Zappy server is the core component of the multiplayer strategy game, managing all game logic, network communications, and state synchronization between AI clients and GUI viewers. It handles real-time gameplay where teams compete to elevate players to the maximum level.

## 🏗️ Architecture Overview

The server follows a modular architecture with clear separation of concerns:

```
📦 Zappy Server
├── 🌐 Network Layer     - Client connections and messaging
├── 🎮 Game Engine       - Core game logic and state management
├── 🗺️  Map System       - Tile-based world with resources
├── 👥 Team Management   - Player teams and coordination
├── 🥚 Egg System        - Player spawning mechanism
└── 📊 GUI Communication - Real-time game state broadcasting
```

## 🚀 Server Initialization

### Main Entry Point
The server starts in [`main.c`](server/src/main.c):

```c
int main(int argc, char **argv)
{
    zappy_t *zappy = init_server(argc, argv);  // Initialize server
    init_game(zappy);                          // Setup game world
    start_protocol(zappy);                     // Begin game loop
    free_zappy(zappy);                         // Cleanup
}
```

### Server Setup Process

1. **Parameter Parsing** - [`params.c`](server/src/params.c)
   - Validates command line arguments
   - Sets up port, map dimensions, teams, client limits, and frequency

2. **Network Initialization** - [`server.c`](server/src/server.c)
   - Creates socket and binds to specified port
   - Initializes unified polling system for all client connections
   - Prepares for multiple client handling

3. **Game World Creation** - [`game/game.c`](server/src/game/game.c)
   - Initializes map with specified dimensions
   - Distributes resources across tiles using density algorithms
   - Creates team structures
   - Spawns initial eggs for players

## 🌐 Network Architecture

### Connection Management
The server handles multiple connection types:

- **AI Clients**: Game players that send commands
- **GUI Clients**: Observers that receive game state updates

### Client Connection Flow
From [`accept.c`](server/src/accept.c):

1. Server accepts new connection
2. Sends "WELCOME" message
3. Reads team name or "GRAPHIC" identifier
4. Validates team name and availability
5. Adds client to appropriate team or GUI list
6. Sends connection confirmation with game parameters

```c
int accept_client(zappy_t *zappy)
{
    int new_sockfd = accept_connection(zappy->network->sockfd);
    char *message = check_team_name(zappy, new_sockfd);
    
    if (strcmp(message, "GRAPHIC") == 0) {
        // Add to GUI clients
    } else {
        // Add to team
    }
}
```

## 🗺️ Map System

### Map Structure
The game world consists of a 2D grid of tiles, each containing:

- **Resources**: Food, Linemate, Deraumere, Sibur, Mendiane, Phiras, Thystame
- **Players**: Multiple players can occupy the same tile
- **Eggs**: Spawning points for new players

### Resource Distribution
From [`game/game.c`](server/src/game/game.c):

```c
static void distribute_resources(zappy_t *server)
{
    float density[7] = {0.5, 0.3, 0.15, 0.1, 0.1, 0.08, 0.05};
    // Distributes resources based on map size and density ratios
}
```

Resources are distributed using:
- **Fisher-Yates shuffle** for random tile selection
- **Density-based allocation** ensuring balanced resource availability
- **Wraparound coordinates** for seamless world boundaries

## 👥 Team and Player Management

### Team Structure
Each team contains:
- **Name**: Unique identifier
- **Player List**: Linked list of active players
- **Player Count**: Current and maximum allowed players
- **Alive Players**: Track active participants

### Player Lifecycle
From [`game/player.c`](server/src/game/player.c):

1. **Connection**: Player joins team through egg hatching
2. **ID Assignment**: Unique ID generation across all teams
3. **Positioning**: Random spawn location assignment
4. **Inventory**: Initial resource allocation
5. **Status Tracking**: Level, direction, and alive state

## 🥚 Egg System

### Purpose
Eggs serve as spawn points for new players, distributed randomly across the map at server startup.

### Egg Lifecycle
From [`game/init_egg.c`](server/src/game/init_egg.c):

1. **Creation**: Generated during server initialization
2. **Positioning**: Random placement using shuffled coordinates
3. **Assignment**: Connected to players upon team joining
4. **Hatching**: Converts to active player
5. **Cleanup**: Removed after player spawning

## 🎮 Game Loop and Protocol

### Main Game Loop
From [`protocol.c`](server/src/protocol.c):

```c
int start_protocol(zappy_t *zappy)
{
    double respawn_interval = 0;
    double time_since_last_spawn = 0.0;
    struct timespec last_time;

    clock_gettime(CLOCK_MONOTONIC, &last_time);
    setup_signal();
    
    while (*get_running_state()) {
        respawn_interval = 20.0 / zappy->params->freq;
        poll_all_clients(zappy);              // Unified polling system
        handle_time_and_respawn(zappy, &last_time, &time_since_last_spawn,
            respawn_interval);
        handle_clients_and_game(zappy);       // Process game logic
    }
}
```

### Unified Polling System
The server now uses a unified polling architecture that efficiently manages all client connections in a single poll operation:

#### Unified Poll Structure
From [`zappy.h`](server/include/zappy.h):

```c
typedef struct unified_poll_s {
    struct pollfd *fds;     // Array of file descriptors to poll
    int count;              // Current number of active FDs
    int capacity;           // Maximum capacity of the array
} unified_poll_t;
```

#### Poll Management Functions
From [`unified_poll.c`](server/src/unified_poll.c):

- **`init_unified_poll()`**: Initializes the polling structure
- **`add_fd_to_poll()`**: Adds a new file descriptor to the poll array
- **`remove_fd_from_poll()`**: Removes a file descriptor from polling
- **`rebuild_poll_fds()`**: Reconstructs the poll array for efficiency
- **`poll_all_clients()`**: Main polling function that handles all connections

#### Client Processing Flow
```c
void poll_all_clients(zappy_t *zappy)
{
    // 1. Poll all file descriptors simultaneously
    int poll_result = poll(zappy->unified_poll->fds, 
                          zappy->unified_poll->count, 0);
    
    // 2. Check server socket for new connections
    if (server_socket_ready)
        accept_client(zappy);
    
    // 3. Process AI client messages
    for each AI client with POLLIN:
        queue_action(player, command, zappy);
    
    // 4. Handle GUI client requests
    for each GUI client with POLLIN:
        poll_graphic_commands(zappy, current, buffer);
    
    // 5. Detect disconnections (POLLHUP/POLLERR)
    remove_disconnected_clients();
}
```

### Connection Processing Details

#### Server Socket Monitoring
The unified polling system monitors the server socket for incoming connections:

```c
// Check if server socket has incoming connections
if (zappy->unified_poll->fds[0].revents & POLLIN) {
    accept_client(zappy);  // Handle new connection
}
```

#### AI Client Message Processing
For each AI client with available data (`POLLIN` event):

1. **Message Reception**: Read command from client socket
2. **Command Parsing**: Extract and validate command format
3. **Action Queuing**: Add command to player's action queue with priority
4. **Response Handling**: Queue appropriate response for later transmission

```c
// From accept.c and unified polling
char *message = get_message(fd, timeout);
if (message) {
    queue_action(player, message, zappy);
}
```

#### GUI Client Request Handling
GUI clients send specific query commands that are processed immediately:

```c
// From graphic_client.c
for (int i = 0; GRAPHIC_COMMAND[i].command != NULL; i++) {
    if (strncmp(GRAPHIC_COMMAND[i].command, buffer,
        strlen(GRAPHIC_COMMAND[i].command)) == 0)
        return GRAPHIC_COMMAND[i].handler(zappy, current, buffer);
}
```

#### Connection State Management
The polling system efficiently handles connection lifecycle:

- **New Connections**: Added to unified poll array via `add_fd_to_poll()`
- **Active Monitoring**: All connections polled simultaneously
- **Disconnection Detection**: `POLLHUP` and `POLLERR` events trigger cleanup
- **Resource Cleanup**: File descriptors removed and closed properly

#### Performance Benefits
The unified polling approach provides several advantages:

1. **Single System Call**: All file descriptors polled in one `poll()` call
2. **Scalable Architecture**: Handles multiple client types efficiently
3. **Event-Driven Processing**: Only processes clients with available data
4. **Reduced CPU Usage**: No busy-waiting or individual socket polling
5. **Centralized Management**: Single point of control for all network I/O

## 🔄 Unified Polling Implementation

### Overview
The Zappy server implements a sophisticated unified polling system that handles all network I/O operations through a single, efficient polling mechanism. This system manages three types of connections simultaneously:

1. **Server Socket**: Listening for new incoming connections
2. **AI Clients**: Game players sending commands and receiving responses
3. **GUI Clients**: Observers requesting game state information

### Implementation Details

#### Polling Structure Management
From [`unified_poll.c`](server/src/unified_poll.c):

```c
unified_poll_t *init_unified_poll(void)
{
    unified_poll_t *poll_struct = malloc(sizeof(unified_poll_t));
    poll_struct->fds = malloc(sizeof(struct pollfd) * INITIAL_CAPACITY);
    poll_struct->count = 0;
    poll_struct->capacity = INITIAL_CAPACITY;
    return poll_struct;
}
```

#### Dynamic File Descriptor Management
The system dynamically manages file descriptors with automatic capacity expansion:

- **Adding FDs**: `add_fd_to_poll()` adds new connections to the polling array
- **Removing FDs**: `remove_fd_from_poll()` removes disconnected clients
- **Rebuilding**: `rebuild_poll_fds()` optimizes the array structure

#### Event Processing Flow
From [`unified_poll.c`](server/src/unified_poll.c):

```c
void poll_all_clients(zappy_t *zappy)
{
    // Poll all file descriptors with timeout of 0 (non-blocking)
    int poll_result = poll(zappy->unified_poll->fds, 
                          zappy->unified_poll->count, 0);
    
    if (poll_result > 0) {
        // Process server socket for new connections
        check_server_socket(zappy);
        
        // Process existing client connections
        process_client_events(zappy);
        
        // Handle disconnections and errors
        cleanup_disconnected_clients(zappy);
    }
}
```

### Client Type Handling

#### AI Client Processing
AI clients are processed through the action queue system:

1. **Message Reading**: Commands are read from client sockets
2. **Command Validation**: Messages are parsed and validated
3. **Action Queuing**: Valid commands are added to player action queues
4. **Priority Handling**: Actions are executed based on priority levels

#### GUI Client Processing
GUI clients receive immediate responses to their queries:

1. **Command Parsing**: GUI commands are parsed immediately
2. **Handler Dispatch**: Appropriate handler functions are called
3. **Response Generation**: Game state data is formatted and sent
4. **Real-time Updates**: Continuous broadcasting of game events

### Error Handling and Robustness

#### Connection Error Detection
The polling system detects various connection states:

- **POLLIN**: Data available for reading
- **POLLOUT**: Socket ready for writing
- **POLLHUP**: Client disconnected (hang up)
- **POLLERR**: Socket error occurred
- **POLLNVAL**: Invalid file descriptor

#### Graceful Disconnection Handling
```c
// From unified polling implementation
if (poll_fd.revents & (POLLHUP | POLLERR)) {
    remove_client_from_game(fd);
    remove_fd_from_poll(zappy->unified_poll, fd);
    close(fd);
}
```

### Performance Characteristics

#### Efficiency Gains
- **O(1) Addition/Removal**: File descriptor management is constant time
- **Single System Call**: All network I/O handled in one poll() call
- **Event-Driven**: Only processes clients with pending data
- **No Busy Waiting**: Non-blocking operation with efficient timeout handling

#### Scalability Features
- **Dynamic Capacity**: Automatic expansion of polling array
- **Memory Efficient**: Compact data structures minimize overhead
- **Low Latency**: Direct event processing without intermediate queues
- **Multi-Client Support**: Handles hundreds of simultaneous connections

### Integration with Game Logic

#### Action Queue System
The polling system integrates seamlessly with the action queue mechanism:

```c
// From pollin_handler.c
void process_player_actions_tick(zappy_t *zappy)
{
    team_t *team = zappy->game->teams;
    while (team) {
        player_t *player = team->players;
        while (player) {
            process_player_actions(player, zappy);
            player = player->next;
        }
        team = team->next;
    }
}
```

#### Real-time Game Updates
The system ensures real-time synchronization between AI actions and GUI updates:

1. **Command Reception**: AI commands received via polling
2. **Action Execution**: Commands processed through action queue
3. **State Changes**: Game state updated accordingly
4. **GUI Notification**: Changes broadcast to all GUI clients

This unified polling architecture provides the foundation for Zappy's efficient, scalable multiplayer networking system.
