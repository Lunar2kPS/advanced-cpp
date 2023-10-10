# Small C++ Program Examples
These small C++ programs demonstrate how to use tools, libraries, and programming design patterns, on a more basic level.

## 📜 How to Run These Programs
NOTE: Each project has a name, defined near the top of their CMakeLists.txt file with the `project(...)` CMake statement. For example, `project("JsonExample" VERSION 1.0.0)` defines the project name to be "JsonExample".

1. Open a (Git) bash terminal to the example project's directory.
2. Run `cmake . -B out`
    - This uses CMake to generate the build system files (like Makefiles, Visual Studio project files, etc.) together with the source files (from `.`, the current directory of your terminal).
3. Run `cmake --build out --config [CONFIG]`, where `[CONFIG]` is either `Debug` or `Release`.
    - This uses the build tool/generator used by CMake to actually build your project.
4. Using the **name of your project** _(see above)_, run your executable:
    - `./out/[CONFIG]/[PROJECT_NAME].exe` _(Windows)_
    - `./out/[CONFIG]/[PROJECT_NAME]` _(Mac/Linux)_