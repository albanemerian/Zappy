# Developer Guide: Creating Custom Audio Libraries

## Overview

The Zappy GUI system uses a dynamic library loading mechanism that allows developers to create custom audio implementations. This guide explains how to develop your own audio library that integrates seamlessly with the Zappy GUI.

## Prerequisites

- C++ knowledge (C++17 or later)
- Understanding of audio programming concepts
- Familiarity with shared library development
- audio API knowledge (OpenGL, DirectX, Vulkan, etc.)

## Architecture Requirements

### Interface Compliance

Your audio library must implement the `IAudio` interface completely. This interface defines all rendering and window management functions required by the GUI.

### Essential Components

1. **Window Management**: Creating and managing the application window
2. **3D Rendering**: Drawing 3D primitives and models
3. **2D Rendering**: Text and UI element rendering
4. **Input Handling**: Keyboard, mouse, and gamepad input
5. **Camera System**: 3D camera management
6. **Resource Loading**: Model and texture loading

## Implementation Guide

### Step 1: Interface Implementation

Create a class that inherits from `IAudio`:

```cpp
#include "IAudio.hpp"

class MyCustomAudio : public IAudio {
private:
    // Your audio context/state variables
public:
    // Implement all pure virtual functions from IAudio
    float getSFXVolumeLevel() override;
    float getMusicVolumeLevel() override;
    bool loadSound(const std::string& id, const std::string& filepath) override;
    void playSound(const std::string& id, float volume) override;
    // ... implement all other methods
};
```

### Step 2: Audio Functions

#### Audio Management
```cpp
bool MyCustomAudio::loadSound(const std::string& id, const std::string& filepath) {
    // Initialize your audio musique
    // And store it in a map with the id in index
}

bool MyCustomAudio::playSound(const std::string& id, float volume) {
    // Play id sound, with the volume in param
}
```

### Step 3: Entry Point Creation

Create the required entry point functions:

```cpp
// EntryPoint.cpp
#include "MyCustomAudio.hpp"
#include "LoaderType.hpp"

extern "C" {
    std::shared_ptr<IAudio> create(void) {
        return std::make_shared<MyCustomAudio>();
    }
    
    ModuleType_t getType(void) {
        return ModuleType_t::AUDIO_MODULE;
    }
}
```

### Step 8: Build Configuration

#### CMakeLists.txt Example
```cmake
cmake_minimum_required(VERSION 3.10)
project(MyCustomAudio)

set(CMAKE_CXX_STANDARD 17)

# Add your audio library dependencies
find_package(YouraudioLib REQUIRED) // for exemple SFML-Audio

# Create shared library
add_library(MyCustomAudio SHARED
    MyCustomAudio.cpp
    EntryPoint.cpp
    # ... other source files
)

target_link_libraries(MyCustomAudio
    YouraudioLib::YouraudioLib
    # ... other dependencies
)

# Ensure proper symbol export
set_target_properties(MyCustomAudio PROPERTIES
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
5. **Text Rendering**: Ensure UI text Audios properly

### Integration Test

1. Compile your library as a shared object (.so/.dll)
2. Place it in the appropriate directory (/lib)
3. Launch Zappy GUI with your library:
   ```bash
   ./zappy_gui
   ```
