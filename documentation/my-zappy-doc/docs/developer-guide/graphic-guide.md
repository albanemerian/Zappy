# Developer Guide: Creating Custom Graphics Libraries

## Overview

The Zappy GUI system uses a dynamic library loading mechanism that allows developers to create custom graphics implementations. This guide explains how to develop your own graphics library that integrates seamlessly with the Zappy GUI.

## Prerequisites

- C++ knowledge (C++17 or later)
- Understanding of graphics programming concepts
- Familiarity with shared library development
- Graphics API knowledge (OpenGL, DirectX, Vulkan, etc.)

## Architecture Requirements

### Interface Compliance

Your graphics library must implement the `IDisplay` interface completely. This interface defines all rendering and window management functions required by the GUI.

### Essential Components

1. **Window Management**: Creating and managing the application window
2. **3D Rendering**: Drawing 3D primitives and models
3. **2D Rendering**: Text and UI element rendering
4. **Input Handling**: Keyboard, mouse, and gamepad input
5. **Camera System**: 3D camera management
6. **Resource Loading**: Model and texture loading

## Implementation Guide

### Step 1: Interface Implementation

Create a class that inherits from `IDisplay`:

```cpp
#include "IDisplay.hpp"

class MyCustomDisplay : public IDisplay {
private:
    // Your graphics context/state variables
    
public:
    // Implement all pure virtual functions from IDisplay
    Vector2i getMonitorSize() override;
    void initWindow(int width, int height, std::string title) override;
    void beginDrawing() override;
    void endDrawing() override;
    // ... implement all other methods
};
```

### Step 2: Core Rendering Functions

#### Window Management
```cpp
void MyCustomDisplay::initWindow(int width, int height, std::string title) {
    // Initialize your graphics context
    // Create window with specified dimensions
    // Set up initial rendering state
}

bool MyCustomDisplay::isOpen() {
    // Return true if window should remain open
    // Handle window close events
}
```

#### 3D Primitives
```cpp
void MyCustomDisplay::drawCube(Vector3f position, float width, float height, 
                               float length, Color32 color) {
    // Render a 3D cube at specified position with given dimensions
    // Apply the color to the cube
}

void MyCustomDisplay::drawSphere(Vector3f position, float radius, Color32 color) {
    // Render a 3D sphere at specified position
}
```

#### 2D Rendering
```cpp
void MyCustomDisplay::drawText(const std::string& text, float x, float y, 
                               float fontSize, Color32 color) {
    // Render text at specified screen coordinates
    // Handle font sizing and color
    // Should support custom font loading via loadFont method
}

void MyCustomDisplay::drawTextEx(const std::string& text, float x, float y, 
                                 float fontSize, float spacing, Color32 color) {
    // Extended text rendering with custom spacing
}

bool MyCustomDisplay::loadFont(const std::string& id, const std::string& filepath) {
    // Load custom font from file and store with given ID
    // Return true on success, false on failure
}

void MyCustomDisplay::drawRectangleRec(FloatRect rec, Color32 color) {
    // Draw a 2D rectangle for UI elements
}
```

### Step 3: Input System Implementation

#### Keyboard Input
```cpp
int MyCustomDisplay::getKeyId(enum Key key) {
    // Map generic Key enum to your graphics library's key codes
    switch(key) {
        case TAB: return YOUR_LIB_KEY_TAB;
        case UP: return YOUR_LIB_KEY_UP;
        // ... map all keys
    }
}

bool MyCustomDisplay::isKeyPressed(int key) {
    // Return true if key was just pressed this frame
}
```

#### Mouse Input
```cpp
Vector2f MyCustomDisplay::getMousePosition() {
    // Return current mouse cursor position
}

bool MyCustomDisplay::isMouseButtonPressed(int button) {
    // Handle mouse button press detection
}
```

#### Gamepad Support
```cpp
bool MyCustomDisplay::isGamepadAvailable() {
    // Check if gamepad is connected
}

float MyCustomDisplay::getGamepadAxisMovement(int axis) {
    // Return analog stick/trigger values (-1.0 to 1.0)
}
```

### Step 4: Camera System

```cpp
void MyCustomDisplay::initCamera() {
    // Set up initial 3D camera parameters
    // Position, target, up vector, FOV, etc.
}

void MyCustomDisplay::setCameraPosition(Vector3f position) {
    // Update camera position in 3D space
}

void MyCustomDisplay::begin3DMode() {
    // Set up 3D rendering mode
    // Apply camera transformations
}

void MyCustomDisplay::end3DMode() {
    // Return to 2D rendering mode
}
```

### Step 5: Model Loading System

```cpp
bool MyCustomDisplay::loadModel(const std::string& id, const std::string& filepath,
                                Vector3f center) {
    // Load 3D model from file
    // Store it with given ID for later reference
    // Apply center offset if needed
    return success;
}

void MyCustomDisplay::drawModelEx(const std::string& id, Vector3f position,
                                  Vector3f rotationAxis, float rotationAngle,
                                  Vector3f scale, Color32 tint) {
    // Draw previously loaded model with transformations
}
```

### Step 6: Advanced Features

#### Scissor Mode (UI Clipping)
```cpp
void MyCustomDisplay::beginScissorMode(IntRect rect) {
    // Enable scissor testing for UI container clipping
}

void MyCustomDisplay::endScissorMode() {
    // Disable scissor testing
}
```

#### Collision Detection
```cpp
bool MyCustomDisplay::checkCollisionPointRec(Vector2f point, FloatRect rec) {
    // Check if point is inside rectangle (for UI interactions)
    return (point.x >= rec.x && point.x <= rec.x + rec.width &&
            point.y >= rec.y && point.y <= rec.y + rec.height);
}
```

### Step 7: Entry Point Creation

Create the required entry point functions:

```cpp
// EntryPoint.cpp
#include "MyCustomDisplay.hpp"
#include "LoaderType.hpp"

extern "C" {
    std::shared_ptr<IDisplay> create(void) {
        return std::make_shared<MyCustomDisplay>();
    }
    
    ModuleType_t getType(void) {
        return ModuleType_t::DISPLAY_MODULE;
    }
}
```

### Step 8: Build Configuration

#### CMakeLists.txt Example
```cmake
cmake_minimum_required(VERSION 3.10)
project(MyCustomDisplay)

set(CMAKE_CXX_STANDARD 17)

# Add your graphics library dependencies
find_package(YourGraphicsLib REQUIRED)

# Create shared library
add_library(MyCustomDisplay SHARED
    MyCustomDisplay.cpp
    EntryPoint.cpp
    # ... other source files
)

target_link_libraries(MyCustomDisplay
    YourGraphicsLib::YourGraphicsLib
    # ... other dependencies
)

# Ensure proper symbol export
set_target_properties(MyCustomDisplay PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN 1
)
```

## Testing Your Implementation

### Basic Functionality Test

1. **Window Creation**: Verify window opens with correct size
2. **Basic Rendering**: Test primitive drawing functions
3. **Input Response**: Check keyboard/mouse/gamepad input
4. **3D Rendering**: Verify 3D primitives render correctly
5. **Text Rendering**: Ensure UI text displays properly

### Integration Test

1. Compile your library as a shared object (.so/.dll)
2. Place it in the appropriate directory (/lib)
3. Launch Zappy GUI with your library:
   ```bash
   ./zappy_gui
   ```

### Performance Considerations

#### Optimization Tips

1. **Batch Rendering**: Group similar draw calls together
2. **State Management**: Minimize graphics state changes
3. **Memory Management**: Use object pooling for frequently created objects
4. **Culling**: Implement frustum culling for 3D objects
5. **LOD System**: Use level-of-detail for distant objects

#### Profiling

- Monitor frame time consistency
- Check memory usage patterns
- Measure draw call count
- Profile input latency

## Common Implementation Patterns

### Graphics Context Management

```cpp
class MyCustomDisplay : public IDisplay {
private:
    struct GraphicsContext {
        // Window handle
        // Renderer instance
        // Current camera state
        // Loaded resources
    } _context;
    
public:
    MyCustomDisplay() {
        // Initialize graphics context
    }
    
    ~MyCustomDisplay() {
        // Cleanup resources
    }
};
```

### Resource Management

```cpp
class ResourceManager {
private:
    std::unordered_map<std::string, ModelData> _models;
    std::unordered_map<std::string, TextureData> _textures;
    
public:
    bool loadModel(const std::string& id, const std::string& path);
    ModelData* getModel(const std::string& id);
    void cleanup();
};
```

### Input State Tracking

```cpp
class InputManager {
private:
    std::array<bool, 256> _keyStates;
    std::array<bool, 256> _previousKeyStates;
    Vector2f _mousePosition;
    
public:
    void update();
    bool isKeyPressed(int key);
    bool isKeyReleased(int key);
    bool isKeyDown(int key);
};
```

## Debugging Tips

### Common Issues

1. **Symbol Export**: Ensure entry point functions are properly exported
2. **Library Dependencies**: Check all required libraries are available
3. **Coordinate Systems**: Verify coordinate system matches expected format
4. **Memory Leaks**: Use tools like Valgrind to detect memory issues
5. **Thread Safety**: Ensure graphics calls are made from main thread

### Debug Output

Add logging to track function calls:

```cpp
void MyCustomDisplay::drawCube(Vector3f position, float width, float height, 
                               float length, Color32 color) {
    #ifdef DEBUG
    std::cout << "Drawing cube at (" << position.x << ", " << position.y 
              << ", " << position.z << ")" << std::endl;
    #endif
    
    // Your implementation
}
```

## Example Libraries

### Minimal OpenGL Implementation

A basic OpenGL implementation would include:
- GLFW for window management
- OpenGL 3.3+ for rendering
- FreeType for text rendering
- GLEW/GLAD for OpenGL loading

### Vulkan Implementation

For high-performance applications:
- Vulkan API for rendering
- VMA (Vulkan Memory Allocator)
- SPIRV-Cross for shader compilation
- Custom command buffer management

## Conclusion

Creating a custom graphics library for Zappy requires implementing the complete `IDisplay` interface while maintaining compatibility with the existing GUI system. Focus on correctness first, then optimize for performance. The modular design allows for extensive customization while maintaining the core functionality of the Zappy visualization system.

Remember to thoroughly test your implementation with various scenarios and consider contributing back to the project if you create a useful graphics backend!
