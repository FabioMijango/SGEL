# SGEL - Simple Game Engine Layer

![Version](https://img.shields.io/badge/version-1.0-blue.svg)
![C++](https://img.shields.io/badge/C%2B%2B-23-brightgreen.svg?&logo=cplusplus)
![Static Badge](https://img.shields.io/badge/SDL3-2D_Game_Engine-blue?style=flat&logo=SDL)


**SGEL** is a 2D game engine, with basic features, like scene workflow, assets managment and action mapping

---
## Requirements

- **C++23** or higher
- **CMake 4.2** or higher
- **SDL3** (downloaded automatically, even if you have SDL3 installed) 
- **SDL3_image** (downloaded automatically)

---
## Installation

Installing **_SGEL_** will download and build **_SDL3_** and **_SDL3-Image_** automatically, **it may take a few minutes** depending on your internet connection and computer performance.

### Using CMake (Recommended)

#### 1. Add SGEL as a Submodule

```cmake
include(FetchContent)
FetchContent_Declare(
        SGEL
        GIT_REPOSITORY https://github.com/FabioMijango/SGEL.git
        GIT_TAG main # Check for releases if you want a stable version
        GIT_SHALLOW TRUE
        EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(SGEL)

add_executable(MyGame main.cpp)
target_link_libraries(MyGame SGEL)
```

#### 2. Build Your Project

```bash
mkdir build
cd build
cmake ..
# If you want to build the examples, run next line instead
# cmake .. -DSGEL_BUILD_EXAMPLES=ON 
cmake --build .
```

### Manual Installation

#### 1. Clone the Repository into the library folder for your project
```bash
git clone https://github.com/tu-usuario/SGEL.git
cd SGEL
```

#### 2. Compile the Library

```bash
mkdir build
cd build
cmake ..
# If you want to build the examples, run next line instead
# cmake .. -DSGEL_BUILD_EXAMPLES=ON 
cmake --build .
```

---
## Examples

### 1. Basic Window (`01_basic_window`)
Simple example that creates a window

### 2. Assets and Animations (`02_assets_and_animations`)
Show how to load assets and create animations.

### 3. Input Actions (`03_inputActions`)
Show how to map and use input actions.

---
## Roadmap

- [ ] Physics engine integration
- [ ] Font support (SDL_ttf)
- [ ] Audio support (SDL_mixer)
- [ ] Documentation and tutorials

