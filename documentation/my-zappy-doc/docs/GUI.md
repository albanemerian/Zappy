# The Zappy - GUI

## Overview

The Zappy GUI is a comprehensive 3D visualization system built with a modular architecture that supports dynamic graphics library loading. The system renders the game world, provides user interaction through HUD elements, and implements multiple camera modes for enhanced user experience.

## Architecture

### Core Components

- **GUI**: Main controller that orchestrates all GUI components
- **Map**: Handles 3D world rendering (tiles, players, eggs, resources)
- **HUD**: Manages user interface elements and containers
- **CameraManager**: Controls different camera modes and behaviors
- **IDisplay**: Abstract interface for graphics libraries (Strategy Pattern)

### Dynamic Library Loading

The GUI uses a dynamic library system that allows switching between different graphics implementations at runtime:

```cpp
// Dynamic loading of graphics libraries
DLLoader<std::shared_ptr<IDisplay>> _dlLoader;
_dlLoader.Open(lib.c_str());
CreateFunc createFunc = reinterpret_cast<CreateFunc>(_dlLoader.Symbol("create"));
this->_display = createFunc();
```

## Design Patterns Implementation

### 1. Strategy Pattern

**Purpose**: Enables runtime selection of graphics rendering strategies through different display libraries.

**Implementation**:
- `IDisplay` interface defines the contract for all graphics libraries
- `Raylib` class implements the interface for Raylib-based rendering
- Dynamic library loading allows switching implementations without code changes

```cpp
class IDisplay {
public:
    virtual void initWindow(int width, int height, std::string) = 0;
    virtual void drawCube(Vector3f position, float width, float height, float length, Color32 color) = 0;
    virtual void drawText(const std::string& text, float x, float y, float fontSize, Color32 color) = 0;
    // ... other rendering methods
};

class Raylib : public IDisplay {
    // Raylib-specific implementation
};
```

**Benefits**:
- Easy addition of new graphics libraries
- Runtime graphics backend switching
- Abstraction from specific graphics API details

### 2. Composite Pattern

**Purpose**: Manages hierarchical UI structures where containers can hold other UI elements.

**Implementation**:
- `IUIElement` base interface for all UI components
- `Containers` class acts as composite, managing collections of UI elements
- Individual elements like `Button`, `Text`, `Slider` are leaf components

```cpp
class IUIElement {
public:
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual void setPosition(float x, float y) = 0;
};

class Containers : public AContainers {
private:
    std::unordered_map<std::string, std::shared_ptr<IUIElement>> _elements;
public:
    void draw() override {
        for (auto& pair : _elements) {
            pair.second->draw();
        }
    }
};
```

**Benefits**:
- Uniform treatment of individual elements and containers
- Easy addition of new UI element types
- Simplified rendering and event handling

### 3. Observer Pattern

**Purpose**: Enables automatic GUI updates when game state changes.

**Implementation**:
- `IObserver` interface defines update contract
- `GuiObserver` observes game state changes
- GUI components automatically refresh when notified

```cpp
class IObserver {
public:
    virtual void update() = 0;
};

class GuiObserver : public IObserver {
public:
    void update() override {
        if (auto gui = _gui.lock()) {
            gui->refresh();
        }
    }
private:
    std::weak_ptr<GUI> _gui;
};
```

**Benefits**:
- Automatic synchronization between game logic and display
- Decoupled communication between components
- Real-time updates without manual polling

## Camera System

### Camera Modes

1. **FREE Mode**: Free-roam camera with WASD/gamepad controls
2. **TARGETED Mode**: Camera focused on map center with orbital movement
3. **PLAYER Mode**: Camera follows a specific player with inventory display

### Camera Controls

- **TAB**: Switch between camera modes
- **Arrow Keys/Gamepad**: Navigate players in PLAYER mode
- **Mouse/Right Analog**: Camera rotation in all modes
- **WASD/Left Analog**: Movement in FREE mode

## 3D Rendering

### World Elements

- **Tiles**: Platform models with procedural positioning variation
- **Players**: Character models with team-colored rendering and orientation arrows
- **Eggs**: Spherical representations with team colors
- **Resources**: Food and rock models stacked appropriately
- **Effects**: Broadcasting animations with expanding ring effects

### Visual Features

- Team-based color coding
- Procedural tile positioning for natural appearance
- Stacking system for multiple objects on same tile
- Animation system for player broadcasts
- Orientation indicators for player direction

## HUD System

### Container Hierarchy

- **Square Container**: Control buttons (Exit, Settings, Help, Camera Reset)
- **Side Container**: Team information and player lists
- **Bottom Container**: Player inventory (PLAYER mode only)
- **TPS Container**: Time unit frequency control slider

### UI Components

- **Buttons**: Interactive elements with hover/press states
- **Text**: Information display with automatic sizing
- **Sliders**: Value input with real-time feedback
- **Help System**: Modal dialog with controls information

### Responsive Design

- Percentage-based positioning for different screen sizes
- Automatic element scaling and repositioning
- Relative positioning system for consistent layouts

## Input Handling

### Supported Input Methods

- **Keyboard**: Full keyboard navigation support
- **Mouse**: Click interactions and camera control
- **Gamepad**: Complete gamepad support for all functions

### Input Mapping

```cpp
enum Key {
    TAB,           // Camera mode switching
    UP, DOWN,      // Player navigation
    RIGHT, LEFT,   // Movement
    MOUSE_LEFT,    // UI interaction
    GM_PD_*        // Gamepad equivalents
};
```

## Audio Integration

### Audio Features

- UI interaction sounds (button clicks)
- Volume control integration
- Audio feedback for user actions

## Performance Considerations

### Optimization Techniques

- Scissor mode clipping for UI containers
- Efficient element management in containers
- Dynamic element creation/destruction
- Optimized rendering pipeline

### Memory Management

- Smart pointer usage throughout
- RAII principles for resource management
- Proper cleanup in destructors

## Extensibility

### Adding New Graphics Libraries

1. Implement `IDisplay` interface
2. Create entry point functions (`create`, `getType`)
3. Compile as shared library
4. Load dynamically at runtime

### Adding New UI Elements

1. Inherit from `IUIElement` or `AUIElement`
2. Implement required virtual methods
3. Add factory methods to `Containers`
4. Register in container system

### Custom Camera Modes

1. Extend `CameraMode` enum
2. Add handling in `CameraManager::updateCamera`
3. Implement mode-specific logic
4. Update input handling in `GUI::update`

This architecture provides a flexible, maintainable, and extensible GUI system that can adapt to different graphics backends while providing a rich user experience.

