# Advanced C++ (Learning)
This is a learning project to help my C++ skills rival my Unity/C# skills.

**Purpose:** I'm disappointed in the direction that Unity (as a game engine, and as a company) has been going for a while. I'm particularly interested in stable, extensible software with a focus on RPGs for PC and console gaming.

So, I'm deciding to become independent of Unity by learning an immense amount of C++, to give me much more freedom as a developer, and perhaps even potential to contribute someday to [Godot engine](https://godotengine.org/) or use [Unreal engine](https://www.unrealengine.com). For now, I'm sticking with command line, libraries, and 2D projects.

---


### 🛠️ TODO's
- ⚠️ **I need to document the setup better,** and make it easier to build. Currently, you need to copy the VS output files from the csharp project into the C++ program's output directory for the .NET hosting to work.
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
- Cross-platform support (Win/Max/Linux/Pi)
- Using EC/ECS
- File I/O
- Rendering realtime 2D pixel art
- Using GUI
- HTTP Networking
- TCP/UDP Networking


## Project Dependencies
- [Json for Modern C++ v3.11.2](https://github.com/nlohmann/json/releases/tag/v3.11.2)
    - Downloaded the `include.zip`, deleted the `include` folder and `meson.build` file, and changed the structure to easily `#include "nlohmann/json.hpp"`


## Resources
- https://github.com/skypjack/entt 
- https://github.com/dotnet/roslyn
- https://www.baeldung.com/java-structural-design-patterns
- (Roslyn) https://www.nuget.org/packages/Microsoft.CodeAnalysis.CSharp/

- ### [C# .NET Hosting from Native C++](https://learn.microsoft.com/en-us/dotnet/core/tutorials/netcore-hosting)
- See their GitHub docs on [Native hosting (Calling managed function)](https://github.com/dotnet/runtime/blob/main/docs/design/features/native-hosting.md#calling-managed-function-net-5-and-above) and a brief explanation of [the components involved](https://github.com/dotnet/runtime/blob/main/docs/design/features/host-components.md).