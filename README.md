# Advanced C++ Learning -- Game Engine Foundation
This repo gives an overview of my C/C++ learning, and small example programs integrating libraries with CMake, particularly with my interest in gaming/RPGs, 2D and 3D graphics, and networking.

I want to encourage free, open source, stable, and extensible software with a focus on RPGs for PC and console gaming. Perhaps I may be able to contribute someday to open source projects like the [Godot engine](https://godotengine.org/), or learn [Unreal engine](https://www.unrealengine.com).

![Project Overview Screenshot](/docs/images/Project%20Overview.png)


## Platform Support
- ✅ Windows
- ✅ MacOS (Intel64 and Arm64 (M1/M2))
- ✅ Linux (tested on Zorin/Ubuntu)
- ✅ Raspberry Pi (tested on arm64 (Debian-based))


## ⭐ Building and Running The Project
Note that I use the Git Bash with VS Code.

**1:** This repo uses Git submodules for my C++ CMake build scripts, some of the libraries, so you may need to run:
```sh
git submodule update --init --recursive
```

**2:** You can make the build and run commands more easily available using the following command to set some shortcut ("alias") commands for your current terminal session:
```sh
source ./cmake-scripts/aliases.sh
```

It will tell you the newly-available commands.

**3:** Then, you can simply just run the following to build and run the release mode of this project:
```sh
build && run-release
```

ℹ️ _For more details on what those build and run scripts are doing, check out the **cmake-scripts** Git submodule under `/cmake-scripts` and read its [README.md](https://github.com/Lunar2kPS/cmake-scripts)._


## 📕 [Project Dependencies](./LIBRARIES.md)
See this for descriptions of the libraries/dependencies I use.


## 📁 Subfolders in this Project
- [Small (C++ Program) Examples](./small-examples/README.md)
    - _(See these for simpler, smaller examples of learning various topics)_
    - Includes examples for ECS, JSON, OpenGL, UTF-8 cross-platform, File I/O, and more!


## 🛠️ Project TODO's
This is a learning project, where I gather a lot of my C++/C#/game engine notes over time, and show off small examples for learning purposes.

My complete game engine is private, but this Git repo gives you a lot of the basics to start with, if you're curious about making your own game engine.

Here are some things I'd like to do next:

- Features/Bugs:
    - (A lot of work) Investigate into performance -- For some reason, the OpenGL draw calls I have with grass tiles are really heavy. I not only want to employ optimization techniques like frustum culling, draw call batching, and GPU instancing, but I also want to understand if there's any ways to optimize the code as-is if my game actually did need to keep 100s of draw calls per frame.
    - 🐛 For some reason, I've noticed that changing the shader from our source code's "resources" folder updates the already-built builds upon next run. The C++ program should really be reading from the /out folder's resources folder instead.
    - Dear ImGUI:
        - 🐛 Dear ImGUI's `ImGui_ImplOpenGL3_RenderDrawData(...)` function throws a 1280 (0x500) GL_INVALID_ENUM OpenGL error during drawing on 64bit Raspberry Pi (OpenGL ES 3.1).
        - 🐛 I need to somehow fix an issue with Dear IMGUI where Tab/Shift + Tab navigation on the keyboard is not working at all.
        - I want to restrict all GUI to the GLFW window, so they can't get cut off-screen.
        - I want to display float fields without forcing a specific number of decimal places.

- Documentation:
    - I need to document the setup better for .NET hosting.
    - I want to better document how I switch between using OpenGL and OpenGL ES, and the pros/cons of the current approach's use of "pre-assumptions" about the current platform.
    - I want to document and measure the most performant way(s) of calling out to C# code, especially during the game loop.
    - I want to better document what the setup was for getting the [small example UTF-8 program](./small-examples/utf-8) to work.

See my [backlog](/BACKLOG.md) for other, lower priority learning goals.


## Resources
- [OpenGL Docs](https://docs.gl/)
- C++ Learning:
    - [The Definitive C++ Book Guide and List](https://stackoverflow.com/questions/388242/the-definitive-c-book-guide-and-list)
    - [File I/O in C++](https://www.w3schools.com/cpp/cpp_files.asp)
    - [Do's and Do NOT's of multithreading](https://gamedev.stackexchange.com/a/80077)
    - [ThreadPools](https://learn.microsoft.com/en-us/dotnet/api/system.threading.threadpool?view=net-7.0)
- [Programming Design Patterns](https://refactoring.guru/design-patterns)
    - [In C#](https://refactoring.guru/design-patterns/csharp)
    - [In C++](https://refactoring.guru/design-patterns/cpp)
- [Game Programming Design Patterns](https://gameprogrammingpatterns.com/contents.html)
