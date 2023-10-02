# Advanced C++ (Learning)
This is a learning project to help my C++ skills rival my Unity/C# skills.

**Purpose:** I'm disappointed in the direction that Unity (as a game engine, and as a company) has been going for a while. However, it's taught me so much about game development and API design, and inspired my interest in free, open source, stable, and extensible software with a focus on RPGs for PC and console gaming.

I'm deciding to become independent of Unity by learning an immense amount of C++, to give me much more freedom as a developer, and perhaps even potential to contribute someday to [Godot engine](https://godotengine.org/) or learn [Unreal engine](https://www.unrealengine.com). For now, I'm sticking with command line, libraries, and 2D projects. I hope this project helps to provide a human-friendly, more understandable example of working with many C++ libraries to create a small, specialized game engine.

---

## 🛠️ Platform Support
- ✅ Windows
- ✅ MacOS
    - ⚠️ I currently only own an Intel Mac computer, so I can't support Arm64 (M1/M2) / Apple Silicon computer architecture yet.
- ✅ Linux (tested on Zorin/Ubuntu)
- 🛠️ Raspberry Pi (tested on arm64)


## 🛠️ Project TODO's
This project is very much in-progress. Here are some things I'd like to do next:

- ⚠️ **I need to document the setup better for .NET hosting,** and make it easier to build. Currently, you need to copy the VS output files from the csharp project into the C++ program's output directory for the .NET hosting to work.
- Open questions about Linux:
    - Why did linking against dl work but not against libdl?
    - Why did we need to link against pthread? Is this intentional?
    - Open a GitHub bug/issue for the snap installation issues I found.
    - Perhaps learn about [dlfcn.h](https://stackoverflow.com/questions/59727057/purpose-of-using-dynamic-linking-loader-dlfcn-h-instead-of-direct-function-cal) in more detail.
    - Establish a better workflow for determining libraries from standard C++ header (.h) files
        - https://docs.oracle.com/cd/E86824_01/html/E54772/libdl-3lib.html
        - https://man7.org/linux/man-pages/man3/dlopen.3.html#LIBRARY


## Learning Topics
- JSON saving/loading
- Embedding C# scripting
- Cross-platform support (Win/Mac/Linux/Pi)
- Using EC/ECS
- File I/O
- Rendering realtime 2D pixel art
- Using GUI
- HTTP Networking
- TCP/UDP Networking


## 🛠️ Project Dependencies
- .NET 7+ (SDK) ⚠️ _(more instructions to come soon about this)_
- [Nlohmann's Json Library v3.11.2](https://github.com/nlohmann/json/releases/tag/v3.11.2)
    - Downloaded the `include.zip`, deleted the `include` folder and `meson.build` file, and changed the structure to easily `#include "nlohmann/json.hpp"`
- [Entt v3.12.2](https://github.com/skypjack/entt)
    - Downloaded their single-file header include ([entt.hpp](https://github.com/skypjack/entt/blob/v3.12.2/single_include/entt/entt.hpp))
- [GLAD v2.0.4](https://github.com/Dav1dde/glad/tree/v2.0.4)
    - Linked in their README, use their [webservice](https://gen.glad.sh/) to generate the files you need.
    - I had some trouble setting this up, but it turned out to be easy! See [glad issue #443](https://github.com/Dav1dde/glad/issues/443) for more details.
    - See [this screenshot](/docs/images/Glad%20v2.0.4%20Generation%20Options.png) for my exact options used!
- [![Git icon](/docs/images/Git%20Icon%20(Small%20Orange).png "GLFW is brought in as a git submodule")](https://git-scm.com/book/en/v2/Git-Tools-Submodules) [GLFW v3.3.8](https://github.com/glfw/glfw/tree/3.3.8)
- [![Git icon](/docs/images/Git%20Icon%20(Small%20Orange).png "GLM is brought in as a git submodule")](https://git-scm.com/book/en/v2/Git-Tools-Submodules) [GLM v0.9.9.8](https://github.com/g-truc/glm/tree/0.9.9.8)
- [![Git icon](/docs/images/Git%20Icon%20(Small%20Orange).png "Dear ImGui is brought in as a git submodule")](https://git-scm.com/book/en/v2/Git-Tools-Submodules) [(Dear) ImGui v1.89.9-docking](https://github.com/ocornut/imgui/tree/v1.89.9-docking)

## Resources
- https://www.baeldung.com/java-structural-design-patterns

- ### [C# .NET Hosting from Native C++](https://learn.microsoft.com/en-us/dotnet/core/tutorials/netcore-hosting)
    - See their GitHub docs on [Native hosting (Calling managed function)](https://github.com/dotnet/runtime/blob/main/docs/design/features/native-hosting.md#calling-managed-function-net-5-and-above) and a brief explanation of [the components involved](https://github.com/dotnet/runtime/blob/main/docs/design/features/host-components.md).