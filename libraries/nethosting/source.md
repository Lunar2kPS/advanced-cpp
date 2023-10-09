# Source of .NET Hosting APIs
I retrieved these files from the following folders. Each of them are nested inside of your actual .NET installation folder:

- **Windows:** `C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Host.win-x64/7.0.1/runtimes/win-x64/native`
    - coreclr_delegates.h\*
    - hostfxr.h\*
    - nethost.h\*
    - nethost.dll
    - nethost.lib
- **MacOS:** `/users/local/share/dotnet/packs/Microsoft.NETCore.App.Host.osx-x64/7.0.11/runtimes/osx-64/native`
    - coreclr_delegates.h\*
    - hostfxr.h\*
    - nethost.h\*
    - libnethost.a
    - libnethost.dylib
- **Linux (Zorin/Ubuntu):** `/home/[USER]/.dotnet/Microsoft.NETCore.App.Host.linux-x64/7.0.11/runtimes/linux-x64/native`
    - coreclr_delegates.h\*
    - hostfxr.h\*
    - nethost.h\*
    - libnethost.a
    - libnethost.so
- **Raspberry Pi 64-bit (Debian-based Linux):** `/home/[USER]/.dotnet/Microsoft.NETCore.App.Host.linux-arm64/7.0.11/runtimes/linux-arm64/native`
    - _(Same file names as Linux above, but compiled for this platform instead)_


### Notes:
- \* These files have been tested to be the same header files on all platforms:
    - coreclr_delegates.h
    - hostfxr.h
    - nethost.h
- Note that for Linux, you just need to replace "[USER]" with your actual username
    - For example, on my machines, it's `/home/carlos`.
    - An abbreviation in command-line for this (the user directory) is simply `~`.
