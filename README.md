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


## [Project Dependencies](./LIBRARIES.md)


## 📁 Subfolders in this Project
- [Small (C++ Program) Examples](./small-examples/README.md)
    - _(ℹ️ See these for simpler, smaller examples of learning various topics)_
    - Includes examples for ECS, JSON, OpenGL, UTF-8 cross-platform, File I/O, and more!


## 🛠️ Project TODO's
This project is very much in-progress. Here are some things I'd like to do next:

- Features/Bugs:
    - 🔥🐛 Learn how to measure performance/memory usage for the GameObjects I've created so far... the grass tiles seem to really eat a ton of performance, and they're barely doing anything!
    - Upgrade the ServiceLocator implementation (in the main program under `/src`) to use templates to let you define the base class that all your systems must inherit from, rather than hard-coding it to use IGameLoopSystems.
    - 🐛 For some reason, I've noticed that changing the shader from our source code's "resources" folder updates the already-built builds upon next run. The C++ program should really be reading from the /out folder's resources folder instead.
    - Dear ImGUI:
        - 🐛 Dear ImGUI's `ImGui_ImplOpenGL3_RenderDrawData(...)` function throws a 1280 (0x500) GL_INVALID_ENUM OpenGL error during drawing on 64bit Raspberry Pi (OpenGL ES 3.1).
        - 🐛 I need to somehow fix an issue with Dear IMGUI where Tab/Shift + Tab navigation on the keyboard is not working at all.
        - I want to restrict all GUI to the GLFW window, so they can't get cut off-screen.
        - I want to display float fields without forcing a specific number of decimal places.

- Documentation:
    - I want to better document what the setup was for getting the [small example UTF-8 program](./small-examples/utf-8) to work.
    - I want to better document how I switch between using OpenGL and OpenGL ES, and the pros/cons of the current approach's use of "pre-assumptions" about the current platform.
    - I want to document and measure the most performant way(s) of calling out to C# code, especially during the game loop.
    - I need to document the setup better for .NET hosting.

See my [backlog](/BACKLOG.md) for other, lower priority learning goals.

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
