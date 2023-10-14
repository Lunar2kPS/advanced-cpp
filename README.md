# Advanced C++ (Learning)
This is a learning project to help my C++ skills rival my Unity/C# skills.

**Purpose:** I'm disappointed in the direction that Unity (as a game engine, and as a company) has been going for a while. However, it's taught me so much about game development and API design, and inspired my interest in free, open source, stable, and extensible software with a focus on RPGs for PC and console gaming.

I'm deciding to learn an immense amount of C++, to give me much more freedom as a developer, and perhaps even potential to contribute someday to open source projects like the [Godot engine](https://godotengine.org/), or learn [Unreal engine](https://www.unrealengine.com). For now, I'm sticking with command line, libraries, and 2D projects. I hope this project helps to provide a human-friendly, more understandable example of working with many C++ libraries to create a small, specialized game engine.

---

## Platform Support
- ✅ Windows
- ✅ MacOS (Intel64 and Arm64 (M1/M2))
- ✅ Linux (tested on Zorin/Ubuntu)
- ✅ Raspberry Pi (tested on arm64)


## 🛠️ Project TODO's
This project is very much in-progress. Here are some things I'd like to do next:

- I need to document the setup better for .NET hosting.
- I want to split up the project to have easier, smaller examples of using different techniques more individually as well. (Showing both how to use techniques on an individual level, _as well as_ integrating them all together).
- ### Learning Topics
    - JSON saving/loading
    - Using EC/ECS
    - Embedding C# scripting _(native .NET hosting)_
    - Programming design patterns
    - File I/O
    - Rendering realtime 2D pixel art
    - Using GUI
    - HTTP Networking
    - TCP/UDP Networking
    - Measuring performance and/or profiling

See my [backlog](/BACKLOG.md) for other, lower priority learning goals.


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
- [![Git icon](/docs/images/Git%20Icon%20(Small%20Orange).png "Dear ImGui is brought in as a git submodule")](https://git-scm.com/book/en/v2/Git-Tools-Submodules) [(Dear) ImGui v1.89.9-docking](https://github.com/ocornut/imgui/tree/v1.89.9-docking)
    - ℹ️ Dear ImGui _([previously named just "ImGui"](https://github.com/ocornut/imgui/discussions/4041) -- Immediate Mode Graphical User Interface)_ is used for developer/debugging user interface, like panels, text, buttons, and other GUI controls.

## Resources
- https://www.baeldung.com/java-structural-design-patterns
- 🛠️ TODO: Sort through these helpful links:
    - [EnTT Crash Course](https://skypjack.github.io/entt/md_docs_md_entity.html)
    - [Arbitrary type JSON serialization](https://json.nlohmann.me/features/arbitrary_types/)
    - Dear ImGui styles ([here](https://github.com/GraphicsProgramming/dear-imgui-styles) and [here](https://github.com/ocornut/imgui/issues/707))
    - [The Definitive C++ Book Guide and List](https://stackoverflow.com/questions/388242/the-definitive-c-book-guide-and-list)
    - [Discussion on proper use of GLAD](https://github.com/Dav1dde/glad/issues/443)
    - [ThreadPools](https://learn.microsoft.com/en-us/dotnet/api/system.threading.threadpool?view=net-7.0)
    - [OpenGL Docs](https://docs.gl/)
    - [Do's and Do NOT's of multithreading](https://gamedev.stackexchange.com/a/80077)
    - [File I/O in C++](https://www.w3schools.com/cpp/cpp_files.asp)
    - Programming design patterns (C# focus, because that's what I'm familiar with):
        - https://www.dofactory.com/net/design-patterns
        - https://www.c-sharpcorner.com/UploadFile/bd5be5/design-patterns-in-net/
        - https://refactoring.guru/design-patterns/csharp
        - (C++) https://gameprogrammingpatterns.com/contents.html
    - [FreeImage library](https://freeimage.sourceforge.io/download.html)
        - [FreeImage docs](https://cfhcable.dl.sourceforge.net/project/freeimage/Source%20Documentation/3.18.0/FreeImage3180.pdf)

- ### [C# .NET Hosting from Native C++](https://learn.microsoft.com/en-us/dotnet/core/tutorials/netcore-hosting)
    - See their GitHub docs on [Native hosting (Calling managed function)](https://github.com/dotnet/runtime/blob/main/docs/design/features/native-hosting.md#calling-managed-function-net-5-and-above) and a brief explanation of [the components involved](https://github.com/dotnet/runtime/blob/main/docs/design/features/host-components.md).