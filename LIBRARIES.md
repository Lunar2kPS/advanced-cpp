# Libraries (Project Dependencies)
These are the libraries currently used by this C++ project:

| 📕 Library                | 🔑 License                   | ℹ️ Description             |
| ------------------------- | ---------------------------- | ------------------------- |
| [.NET 7+ Hosting API]     | MIT                          | The .NET hosting header files enable our C++ program to host a .NET runtime. Basically, I need this so we I use C# scripting on top of the C++ game engine, since C# code can run in the .NET runtime. |
| [Nlohmann's JSON Library] | MIT                          | Nlohmann's Json Library allows you to easily read and write to [JSON _(JavaScript Object Notation)_](https://www.json.org/json-en.html), which is a widely used text-based format for saving and loading data into any program that supports reading/writing to it. |
| [EnTT]                    | MIT                          | EnTT is a library for ECS _(Entity-Component System)_ architecture, similar to Unity GameObjects (entities) and MonoBehaviours (components). |
| [GLAD]                    | Public Domain, CC0 (according to [glad](https://github.com/Dav1dde/glad/tree/glad2#License)) | GLAD _(GL Loader-Generator)_ is used to load modern OpenGL functions on a variety of machines and environments, to make it much easier to get started using OpenGL features in cross-platform projects. |
| [GLFW]                    | ZLib                         | GLFW _(Graphics Library Framework)_ is used for creating our game window in a cross-platform way, with support for mouse/keyboard input. |
| [GLM]                     | MIT                          | GLM _(OpenGL Mathematics)_ is used for our vector and matrix types, with similar usage to GLSL shader programs. |
| [Dear ImGUI]              | MIT                          | Dear ImGUI _([previously named just "ImGUI"](https://github.com/ocornut/imgui/discussions/4041) -- Immediate Mode Graphical User Interface)_ is used for developer/debugging user interface, like panels, text, buttons, and other GUI controls. |
| [stb_image]               | MIT **or** Public Domain     | stb_image is a library for loading image files (such as PNG, JPG, GIF, and more) from memory and disk. |

[.NET 7+ Hosting API]: /libraries/nethosting/source.md
[Nlohmann's JSON Library]: https://github.com/nlohmann/json/releases/tag/v3.11.2
[EnTT]: https://github.com/skypjack/entt
[GLAD]: https://github.com/Dav1dde/glad/tree/v2.0.4
[GLFW]: https://github.com/glfw/glfw/tree/3.3.8
[GLM]: https://github.com/g-truc/glm/tree/0.9.9.8
[Dear ImGUI]: https://github.com/ocornut/imgui/tree/v1.89.9-docking
[stb_image]: https://github.com/nothings/stb

## ⚙️ Include/Compiling/Linking Instructions
🛠️ **TODO:** Add instructions for each library on how to integrate them using CMake.

- .NET 7+ Hosting API
    - The sources for these files are documented [here](/libraries/nethosting/source.md).
- [Nlohmann's JSON Library v3.11.2](https://github.com/nlohmann/json/releases/tag/v3.11.2)
    - Download the `include.zip` (from one of the GitHub releases), delete the `include` folder and `meson.build` file, and change the folder structure to easily `#include "nlohmann/json.hpp"`
- [EnTT v3.12.2](https://github.com/skypjack/entt)
    - Download their single-file header include ([entt.hpp](https://github.com/skypjack/entt/blob/v3.12.2/single_include/entt/entt.hpp))
- [GLAD v2.0.4](https://github.com/Dav1dde/glad/tree/v2.0.4)
    - Linked in their README, use their [webservice](https://gen.glad.sh/) to generate the files you need.
        - I had some trouble setting this up, but it turned out to be easy! See [glad issue #443](https://github.com/Dav1dde/glad/issues/443) for more details.
        - See [this screenshot](/docs/images/Glad%20v2.0.4%20Generation%20Options.png) for my exact options used!
- [![Git icon](/docs/images/Git%20Icon%20(Small%20Orange).png "GLFW is brought in as a git submodule")](https://git-scm.com/book/en/v2/Git-Tools-Submodules) [GLFW v3.3.8](https://github.com/glfw/glfw/tree/3.3.8)
- [![Git icon](/docs/images/Git%20Icon%20(Small%20Orange).png "GLM is brought in as a git submodule")](https://git-scm.com/book/en/v2/Git-Tools-Submodules) [GLM v0.9.9.8](https://github.com/g-truc/glm/tree/0.9.9.8)
- [![Git icon](/docs/images/Git%20Icon%20(Small%20Orange).png "Dear ImGUI is brought in as a git submodule")](https://git-scm.com/book/en/v2/Git-Tools-Submodules) [Dear ImGUI v1.89.9-docking](https://github.com/ocornut/imgui/tree/v1.89.9-docking)
- [stb_image](https://github.com/nothings/stb)
    - Download their single-file header include ([stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)) directly, and choose one of their [licenses](https://github.com/nothings/stb/blob/master/LICENSE).

## Related Resources to These Libraries
- [C# .NET Hosting from Native C++](https://learn.microsoft.com/en-us/dotnet/core/tutorials/netcore-hosting)
    - See their GitHub docs on [Native hosting (Calling managed function)](https://github.com/dotnet/runtime/blob/main/docs/design/features/native-hosting.md#calling-managed-function-net-5-and-above) and a brief explanation of [the components involved](https://github.com/dotnet/runtime/blob/main/docs/design/features/host-components.md).
    - [Discussion on (inability to) unload .NET runtime host](https://github.com/dotnet/runtime/issues/70229)
- [Arbitrary type JSON serialization](https://json.nlohmann.me/features/arbitrary_types/)
- [EnTT Crash Course](https://skypjack.github.io/entt/md_docs_md_entity.html)
- [Discussion on proper use of GLAD](https://github.com/Dav1dde/glad/issues/443)
- Dear ImGui styles ([here](https://github.com/GraphicsProgramming/dear-imgui-styles) and [here](https://github.com/ocornut/imgui/issues/707))
