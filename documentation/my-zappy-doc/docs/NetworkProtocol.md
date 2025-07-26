# Network Protocol

## 🌐 Overview

<!-- Describe the network protocol overview here -->

## 🔌 Connection Types

### AI Client Connection

The AI client connects to the server via TCP sockets on the specified port. The connection follows a specific handshake protocol:

#### Connection Handshake
1. **Client opens socket** on server's port
2. **Server sends welcome**: `WELCOME\n`
3. **Client sends team name**: `TEAM-NAME\n`
4. **Server responds with**:
   - `CLIENT-NUM\n` - Number of available slots for the team
   - `X Y\n` - World dimensions (width and height)

#### Connection Rules
- **CLIENT-NUM ≥ 1**: New client can connect successfully
- **CLIENT-NUM = 0**: No slots available, connection may be refused
- **World dimensions**: Provided to help AI understand the game world

#### Communication Model
- **Asynchronous requests**: Client sends up to **10 commands** without waiting for responses
- **Request buffering**: Server queues commands and executes them in order
- **Individual blocking**: Command execution time only blocks the specific player
- **Over-limit handling**: Commands beyond 10 pending requests are ignored

### GUI Client Connection

The GUI client connects as a graphical observer to monitor the game state in real-time.

#### Authentication Process
1. **Client opens socket** on server's port
2. **Server sends welcome**: `WELCOME\n`
3. **Client identifies as GUI**: `GRAPHIC\n`
4. **Server accepts** GUI connection and starts sending game state updates

#### Communication Model
- **Server-initiated**: Server continuously broadcasts game state to GUI
- **GUI requests**: GUI can request specific information
- **Real-time updates**: All game events are immediately sent to GUI clients
- **Multiple viewers**: Multiple GUI clients can connect simultaneously

## 📤 AI Commands (Client → Server)

All commands are transmitted as character strings ending with a newline (`\n`). In case of a bad/unknown command, the server responds with `ko`.

### Movement Commands

| Command | Syntax | Time Limit | Description | Response |
|---------|--------|------------|-------------|----------|
| `Forward` | `Forward\n` | 7/f | Move up one tile in current direction | `ok` |
| `Right` | `Right\n` | 7/f | Turn 90° right | `ok` |
| `Left` | `Left\n` | 7/f | Turn 90° left | `ok` |

### Resource Management Commands

| Command | Syntax | Time Limit | Description | Response |
|---------|--------|------------|-------------|----------|
| `Take` | `Take object\n` | 7/f | Take object from current tile | `ok`/`ko` |
| `Set` | `Set object\n` | 7/f | Set object down on current tile | `ok`/`ko` |

**Valid objects**: `food`, `linemate`, `deraumere`, `sibur`, `mendiane`, `phiras`, `thystame`

### Information Commands

| Command | Syntax | Time Limit | Description | Response |
|---------|--------|------------|-------------|----------|
| `Look` | `Look\n` | 7/f | Look around current position | `[tile1, tile2, ...]` |
| `Inventory` | `Inventory\n` | 1/f | Check current inventory | `[linemate n, sibur n, ...]` |
| `Connect_nbr` | `Connect_nbr\n` | - | Number of unused team slots | `value` |

#### Look Command Response Format
The `Look` command returns a vision cone based on the player's level:
- **Level 1**: 3 tiles (front, front-left, front-right)
- **Level 2**: 8 tiles in a larger cone
- **Higher levels**: Progressively larger vision areas

Response format: `[tile_content tile_content ...]` where each tile contains visible objects and players.

#### Inventory Response Format
Returns current inventory with quantities: `[food n linemate n deraumere n sibur n mendiane n phiras n thystame n]`

### Communication Commands

| Command | Syntax | Time Limit | Description | Response |
|---------|--------|------------|-------------|----------|
| `Broadcast` | `Broadcast text\n` | 7/f | Send message to all team members | `ok` |

The broadcast message is sent to all players of the same team with directional information.

### Advanced Commands

| Command | Syntax | Time Limit | Description | Response |
|---------|--------|------------|-------------|----------|
| `Fork` | `Fork\n` | 42/f | Create a new egg for player spawning | `ok` |
| `Incantation` | `Incantation\n` | 300/f | Start level elevation ritual | `Elevation underway\nCurrent level: k` or `ko` |
| `Eject` | `Eject\n` | 7/f | Eject all other players from current tile | `ok`/`ko` |

#### Incantation Requirements
Level elevation requires:
- **Specific resources** in inventory
- **Correct number of players** of required levels on the same tile
- **Successful ritual completion** (300 time units)

### Special Events

| Event | Syntax | Description |
|-------|--------|-------------|
| Death | - | Player dies (starvation or ejection) → `dead` |

## 📥 AI Responses (Server → Client)

### Standard Responses

| Response | Meaning | Triggered By |
|----------|---------|--------------|
| `ok` | Command executed successfully | Most successful commands |
| `ko` | Command failed or invalid | Failed commands, unknown commands |
| `dead` | Player has died | Starvation or ejection |

### Information Responses

#### Look Command Response
```
[tile_content tile_content ...]
```
Each tile contains: `player food linemate deraumere sibur mendiane phiras thystame`

**Example**: `[player food linemate, food, player player thystame]`

#### Inventory Response
```
[food n linemate n deraumere n sibur n mendiane n phiras n thystame n]
```
**Example**: `[food 5 linemate 2 deraumere 0 sibur 1 mendiane 0 phiras 0 thystame 0]`

#### Connect_nbr Response
```
value
```
Where `value` is the number of available team slots.

#### Incantation Response
**Success**:
```
Elevation underway
Current level: k
```
Where `k` is the new level reached.

**Failure**: `ko`

### Broadcast Reception
When another player broadcasts, the client receives:
```
message k
```
Where:
- `message` is the broadcast text
- `k` is the direction indicator (0-8) showing where the message came from

### Connection Responses
During connection handshake:
```
WELCOME
CLIENT-NUM
X Y
```

## 📊 GUI Messages (Server → GUI)

The GUI receives real-time updates about all game events. All messages follow the format: `command parameters\n`

### Symbol Reference

| Symbol | Meaning |
|--------|---------|
| `X`, `Y` | Width/height or horizontal/vertical position |
| `q0` - `q6` | Resource quantities (food, linemate, deraumere, sibur, mendiane, phiras, thystame) |
| `#n` | Player number |
| `O` | Orientation: 1(N), 2(E), 3(S), 4(W) |
| `L` | Player or incantation level |
| `#e` | Egg number |
| `T` | Time unit |
| `N` | Team name |
| `R` | Incantation result |
| `M` | Message text |
| `i` | Resource number (0-6) |

### Map Information

| Message | Format | Description |
|---------|---------|-------------|
| **Map size** | `msz X Y\n` | Dimensions of the game world |
| **Tile content** | `bct X Y q0 q1 q2 q3 q4 q5 q6\n` | Content of a specific tile |
| **Map content** | `bct X Y q0 q1 q2 q3 q4 q5 q6\n` (×nbr_tiles) | Content of all tiles (sent on connection) |
| **Team names** | `tna N\n` (×nbr_teams) | Names of all teams |

### Player Information

| Message | Format | Description |
|---------|---------|-------------|
| **New player** | `pnw #n X Y O L N\n` | A new player has connected |
| **Player position** | `ppo #n X Y O\n` | Player's current position and orientation |
| **Player level** | `plv #n L\n` | Player's current level |
| **Player inventory** | `pin #n X Y q0 q1 q2 q3 q4 q5 q6\n` | Player's position and inventory |
| **Player death** | `pdi #n\n` | A player has died |

### Player Actions

| Message | Format | Description |
|---------|---------|-------------|
| **Expulsion** | `pex #n\n` | Player expelled others from tile |
| **Broadcast** | `pbc #n M\n` | Player sent a broadcast message |
| **Egg laying** | `pfk #n\n` | Player laid an egg |
| **Resource drop** | `pdr #n i\n` | Player dropped a resource |
| **Resource take** | `pgt #n i\n` | Player collected a resource |

### Incantation Events

| Message | Format | Description |
|---------|---------|-------------|
| **Incantation start** | `pic X Y L #n #n ...\n` | Incantation started (lists all participating players) |
| **Incantation end** | `pie X Y R\n` | Incantation finished (R = success/failure) |

### Egg Management

| Message | Format | Description |
|---------|---------|-------------|
| **New egg** | `enw #e #n X Y\n` | Egg laid by player #n at position X,Y |
| **Egg hatching** | `ebo #e\n` | Player connected to egg #e |
| **Egg death** | `edi #e\n` | Egg #e died (timeout) |

### Server Control

| Message | Format | Description |
|---------|---------|-------------|
| **Time unit** | `sgt T\n` | Current time unit value |
| **End of game** | `seg N\n` | Game ended, team N won |
| **Server message** | `smg M\n` | Message from server |
| **Unknown command** | `suc\n` | Unknown command received |
| **Bad parameters** | `sbp\n` | Command with bad parameters |

## 🔄 GUI Requests (GUI → Server)

The GUI can request specific information from the server:

| Request | Format | Response | Description |
|---------|---------|----------|-------------|
| **Map size** | `msz\n` | `msz X Y\n` | Request map dimensions |
| **Tile content** | `bct X Y\n` | `bct X Y q0 q1 q2 q3 q4 q5 q6\n` | Request specific tile content |
| **Map content** | `mct\n` | `bct ...` (×nbr_tiles) | Request all tile contents |
| **Team names** | `tna\n` | `tna N\n` (×nbr_teams) | Request all team names |
| **Player position** | `ppo #n\n` | `ppo #n X Y O\n` | Request player position |
| **Player level** | `plv #n\n` | `plv #n L\n` | Request player level |
| **Player inventory** | `pin #n\n` | `pin #n X Y q0 q1 q2 q3 q4 q5 q6\n` | Request player inventory |
| **Time unit get** | `sgt\n` | `sgt T\n` | Request current time unit |
| **Time unit set** | `sst T\n` | `sst T\n` | Set new time unit (change game speed) |

## 🎮 GUI Protocol Specifications

### Connection Flow
1. **Connect**: GUI opens socket to server
2. **Welcome**: Server sends `WELCOME\n`
3. **Authenticate**: GUI sends `GRAPHIC\n`
4. **Initial state**: Server sends complete game state:
   - Map size (`msz`)
   - All tile contents (`bct` for each tile)
   - Team names (`tna` for each team)
   - Current players (`pnw`, `ppo`, `plv`, `pin` for each)
   - Current eggs (`enw` for each)
   - Current time unit (`sgt`)

### Real-time Updates
After initial state, server continuously sends:
- **Player events**: Movement, actions, level changes
- **Game events**: Incantations, broadcasts, deaths
- **World changes**: Resource changes, new eggs
- **Time updates**: Game progression

### Message Format
- **Encoding**: ASCII text
- **Termination**: All messages end with `\n`
- **No acknowledgment**: GUI doesn't need to acknowledge received messages
- **Continuous stream**: Server sends updates as they occur

### Error Handling
- **Invalid requests**: Server responds with `suc\n` or `sbp\n`
- **Connection loss**: Server removes GUI from broadcast list
- **Malformed commands**: Server ignores and may send error response

### Resource Numbers
For `pdr`, `pgt`, and extended commands (`pia`, `pis`, `tar`, `tsr`), resource numbers are:
- `0`: food
- `1`: linemate
- `2`: deraumere
- `3`: sibur
- `4`: mendiane
- `5`: phiras
- `6`: thystame

## 🔧 Protocol Extensions

### Extended GUI Commands

These additional commands allow the GUI to request specific player actions or modifications:

| Request | Format | Description |
|---------|---------|-------------|
| **Player level up** | `plu #n\n` | Request to increase player #n's level |
| **Player level down** | `pld #n\n` | Request to decrease player #n's level |
| **Kill player** | `kil #n\n` | Request to kill player #n |
| **Player inventory add** | `pia #n i\n` | Request to add resource `i` to player #n's inventory |
| **Player inventory subtract** | `pis #n i\n` | Request to remove resource `i` from player #n's inventory |
| **Tile add resource** | `tar X Y i\n` | Request to add resource `i` to tile at position (X, Y) |
| **Tile subtract resource** | `tsr X Y i\n` | Request to remove resource `i` from tile at position (X, Y) |

#### Usage Context
- **`plu #n`**: GUI requests the server to artificially increase a player's level (admin/debug feature)
- **`pld #n`**: GUI requests the server to artificially decrease a player's level (admin/debug feature)
- **`kil #n`**: GUI requests the server to kill a specific player (admin/debug feature)
- **`pia #n i`**: GUI requests the server to add a specific resource to a player's inventory (admin/debug feature)
- **`pis #n i`**: GUI requests the server to remove a specific resource from a player's inventory (admin/debug feature)
- **`tar X Y i`**: GUI requests the server to add a specific resource to a tile (admin/debug feature)
- **`tsr X Y i`**: GUI requests the server to remove a specific resource from a tile (admin/debug feature)

#### Implementation Notes
- These commands extend the standard GUI protocol
- Typically used for administrative control or debugging purposes
- May trigger corresponding broadcast messages to other GUI clients
- Server responds with appropriate confirmation or error messages
- **Resource ID parameter (`i`)**: Uses the same resource numbering system as `pdr` and `pgt` messages (see Resource Numbers section above)
