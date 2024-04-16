# CTest (Example)
This is an example C++ CMake-based project that uses CTest to define separate programs for unit tests. The return codes are used to denote success (exit code 0) vs. failure (any non-zero exit code).

Read more about it from the [CMake docs](https://cmake.org/cmake/help/book/mastering-cmake/chapter/Testing%20With%20CMake%20and%20CTest.html).

I wrote out explicit [build](./build.sh) and [run-tests](./run-tests.sh) scripts to make the commands easy to look back to. Just run either those scripts, or the equivalent commands, in your own Git Bash terminal, to build the project (with tests), and run the CTest unit tests, respectively.

Running the unit tests shows the results in your terminal.

> [!NOTE]
> This is a very basic example though, as noted from the `# WARNING` comment in the [CMakeLists.txt](./CMakeLists.txt), that this currently as-written would require a separate program for every unit test!

> [!NOTE]
> Also note that in order to run the `ctest` command, you must first change-directory into your build output folder (`out` in this example project). Otherwise, the command won't see any of your unit tests.
