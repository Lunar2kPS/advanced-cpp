# Advanced C++ (Learning)
This is a learning project to help my C++ skills rival my Unity/C# skills.

## Purpose
I'm disappointed in Unity's greed, and am deciding to become independent of their greed by learning an immense amount of C++, giving me much more freedom, and perhaps even potential to use Unreal Engine later on for 3D projects.
For now, I'm sticking with command line, libraries, and 2D projects.

## Learning Topics
- JSON saving/loading
- Embedding C# scripting
- File I/O
- Using GUI
- Using EC/ECS
- Cross-platform support (Win/Max/Linux/Pi)
- HTTP Networking
- TCP/UDP Networking
- Rendering realtime 2D pixel art

## Project Dependencies
- [Json for Modern C++ v3.11.2](https://github.com/nlohmann/json/releases/tag/v3.11.2)
    - Downloaded the `include.zip`, deleted the `include` folder and `meson.build` file, and changed the structure to easily `#include "nlohmann/json.hpp"`

## Resources
- https://github.com/skypjack/entt 
- https://github.com/dotnet/roslyn

- https://www.baeldung.com/java-structural-design-patterns
- (Roslyn) https://www.nuget.org/packages/Microsoft.CodeAnalysis.CSharp/

### .NET Hosting from Native C++
- https://learn.microsoft.com/en-us/dotnet/core/tutorials/netcore-hosting
- https://github.com/dotnet/samples/blob/main/core/hosting/src/NativeHost/nativehost.cpp
- https://github.com/dotnet/samples/blob/main/core/hosting/readme.md
