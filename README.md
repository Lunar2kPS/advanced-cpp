# Advanced C++ Learning -- Game Engine Foundation
**Purpose:** I'm disappointed in the direction that Unity (as a game engine, and as a company) has been going for a while. However, it's taught me so much about game development and API design, and inspired my interest in free, open source, stable, and extensible software with a focus on RPGs for PC and console gaming.

I'm deciding to learn an immense amount of C++, to give me much more freedom as a developer, and perhaps even potential to contribute someday to open source projects like the [Godot engine](https://godotengine.org/), or learn [Unreal engine](https://www.unrealengine.com).

I hope this project helps to provide a human-friendly, more understandable example of working with many C++ libraries to create a small, specialized game engine.

![Project Overview Screenshot](/docs/images/Project%20Overview.png)


## Platform Support
- ✅ Windows
- ✅ MacOS (Intel64 and Arm64 (M1/M2))
- ✅ Linux (tested on Zorin/Ubuntu)
- ✅ Raspberry Pi (tested on arm64 (Debian-based))


## 🛠️ Project TODO's
This project is very much in-progress. Here are some things I'd like to do next:

- Features/Bugs:
    - 🔥 Learn how to measure performance/memory usage for the GameObjects I've created so far... the grass tiles seem to really eat a ton of performance, and they're barely doing anything!
    - Upgrade the ServiceLocator implementation (in the main program under `/src`) to use templates to let you define the base class that all your systems must inherit from, rather than hard-coding it to use IGameLoopSystems.
    - Dear ImGUI:
        - 🐛 I need to somehow fix an issue with Dear IMGUI where Tab/Shift + Tab navigation on the keyboard is not working at all.
        - I want to display float fields without forcing a specific number of decimal places.
        - I want to restrict all GUI to the GLFW window, so they can't get cut off-screen.
        - Dear ImGUI's `ImGui_ImplOpenGL3_RenderDrawData(...)` function throws a 1280 (0x500) GL_INVALID_ENUM OpenGL error during drawing on 64bit Raspberry Pi (OpenGL ES 3.1).
    - C++ learning topics:
        - Measuring performance and/or profiling
        - Rendering realtime 2D pixel art
        - Programming design patterns
    - 🐛 For some reason, I've noticed that changing the shader from our source code's "resources" folder updates the already-built builds upon next run. The C++ program should really be reading from the /out folder's resources folder instead.

- C++ Learning:
    - HTTP Networking
    - TCP/UDP Networking

- Documentation:
    - I'd like to better document what this repo shows off. Currently my notes are a little bit unorganized for a first-time viewer.
    - I'd like to upgrade the **Project Dependencies** section,
        - To make it easier to see which methods are used to include the libraries (single-file header, source with existing CMakeLists.txt, etc.)
        - To mention licenses
    - I want to better document what the setup was for getting the [small example UTF-8 program](./small-examples/utf-8) to work.
    - I want to better document how I switch between using OpenGL and OpenGL ES, and the pros/cons of the current approach's use of "pre-assumptions" about the current platform.
    - I want to document and measure the most performant way(s) of calling out to C# code, especially during the game loop.
    - I need to document the setup better for .NET hosting.

See my [backlog](/BACKLOG.md) for other, lower priority learning goals.


## 📁 Subfolders in this Project
- [Small (C++ Program) Examples](./small-examples/README.md)
    - Includes examples for ECS, JSON, OpenGL, UTF-8 cross-platform, File I/O, and more!


## Project Dependencies
- .NET 7+ (SDK) 🛠️ _(more instructions to come soon about this)_
    - ⚙️ The sources for these files are documented [here](/libraries/nethosting/source.md).
- [Nlohmann's Json Library v3.11.2](https://github.com/nlohmann/json/releases/tag/v3.11.2)
    - ℹ️ Nlohmann's Json Library allows you to easily read and write to [JSON _(JavaScript Object Notation)_](https://www.json.org/json-en.html), which is a widely used text-based format for saving and loading data into any program that supports reading/writing to it.
    - ⚙️ **Include/compile/link instructions:** Download the `include.zip` (from one of the GitHub releases), delete the `include` folder and `meson.build` file, and change the folder structure to easily `#include "nlohmann/json.hpp"`
- [EnTT v3.12.2](https://github.com/skypjack/entt)
    - ℹ️ EnTT is a library for ECS _(Entity-Component System)_ architecture, similar to Unity GameObjects (entities) and MonoBehaviours (components).
    - ⚙️ **Include/compile/link instructions:** Download their single-file header include ([entt.hpp](https://github.com/skypjack/entt/blob/v3.12.2/single_include/entt/entt.hpp))
- [GLAD v2.0.4](https://github.com/Dav1dde/glad/tree/v2.0.4)
    - ℹ️ _(GL Loader-Generator)_ is used to load modern OpenGL functions on a variety of machines and environments, to make it much easier to get started using OpenGL features in cross-platform projects.
    - ⚙️ **Include/compile/link instructions:** Linked in their README, use their [webservice](https://gen.glad.sh/) to generate the files you need.
        - I had some trouble setting this up, but it turned out to be easy! See [glad issue #443](https://github.com/Dav1dde/glad/issues/443) for more details.
        - See [this screenshot](/docs/images/Glad%20v2.0.4%20Generation%20Options.png) for my exact options used!
- [![Git icon](/docs/images/Git%20Icon%20(Small%20Orange).png "GLFW is brought in as a git submodule")](https://git-scm.com/book/en/v2/Git-Tools-Submodules) [GLFW v3.3.8](https://github.com/glfw/glfw/tree/3.3.8)
    - ℹ️ GLFW _(Graphics Library Framework)_ is used for creating our game window in a cross-platform way.
- [![Git icon](/docs/images/Git%20Icon%20(Small%20Orange).png "GLM is brought in as a git submodule")](https://git-scm.com/book/en/v2/Git-Tools-Submodules) [GLM v0.9.9.8](https://github.com/g-truc/glm/tree/0.9.9.8)
    - ℹ️ GLM _(OpenGL Mathematics)_ is used for our vector and matrix types, with similar usage to GLSL shader programs.
- [![Git icon](/docs/images/Git%20Icon%20(Small%20Orange).png "Dear ImGUI is brought in as a git submodule")](https://git-scm.com/book/en/v2/Git-Tools-Submodules) [Dear ImGUI v1.89.9-docking](https://github.com/ocornut/imgui/tree/v1.89.9-docking)
    - ℹ️ Dear ImGUI _([previously named just "ImGUI"](https://github.com/ocornut/imgui/discussions/4041) -- Immediate Mode Graphical User Interface)_ is used for developer/debugging user interface, like panels, text, buttons, and other GUI controls.
- [stb_image](https://github.com/nothings/stb)
    - ℹ️ stb_image is a library for loading image files (such as PNG, JPG, GIF, and more) from memory and disk.
    - ⚙️ **Include/compile/link instructions:** Download their single-file header include ([stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)) directly, and choose one of their [licenses](https://github.com/nothings/stb/blob/master/LICENSE).


## Resources
- [Programming Design Patterns (IN BASIC ENGLISH!)](https://refactoring.guru/design-patterns)
    - [In C#](https://refactoring.guru/design-patterns/csharp)
    - [In C++](https://refactoring.guru/design-patterns/cpp)
- [Game Programming Design Patterns](https://gameprogrammingpatterns.com/contents.html)
- [EnTT Crash Course](https://skypjack.github.io/entt/md_docs_md_entity.html)
- [Arbitrary type JSON serialization](https://json.nlohmann.me/features/arbitrary_types/)
- Dear ImGui styles ([here](https://github.com/GraphicsProgramming/dear-imgui-styles) and [here](https://github.com/ocornut/imgui/issues/707))
- [The Definitive C++ Book Guide and List](https://stackoverflow.com/questions/388242/the-definitive-c-book-guide-and-list)
- [Discussion on proper use of GLAD](https://github.com/Dav1dde/glad/issues/443)
- [Discussion on (inability to) unload .NET runtime host](https://github.com/dotnet/runtime/issues/70229)
- [ThreadPools](https://learn.microsoft.com/en-us/dotnet/api/system.threading.threadpool?view=net-7.0)
- [OpenGL Docs](https://docs.gl/)
- [Do's and Do NOT's of multithreading](https://gamedev.stackexchange.com/a/80077)
- [File I/O in C++](https://www.w3schools.com/cpp/cpp_files.asp)
- ### [C# .NET Hosting from Native C++](https://learn.microsoft.com/en-us/dotnet/core/tutorials/netcore-hosting)
    - See their GitHub docs on [Native hosting (Calling managed function)](https://github.com/dotnet/runtime/blob/main/docs/design/features/native-hosting.md#calling-managed-function-net-5-and-above) and a brief explanation of [the components involved](https://github.com/dotnet/runtime/blob/main/docs/design/features/host-components.md).
