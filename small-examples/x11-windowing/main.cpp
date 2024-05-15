#include <X11/Xlib.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include "glad/gl-x11-example.h"    //WARNING: I had to rename this due to an include name clash with other C++ projects in this workspace/repo, but ordinarily I'd keep this to glad/gl.h
#include "glad/glx.h"

using std::cout;
using std::cerr;
using std::endl;
using std::strlen;

//NOTE: Some amazing code is available at: https://github.com/gamedevtech/X11OpenGLWindow

//NOTE: If you need to debug this program, use the following .vscode/launch.json in VS Code:
// {
//     // Use IntelliSense to learn about possible attributes.
//     // Hover to view descriptions of existing attributes.
//     // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
//     "version": "0.2.0",
//     "configurations": [
//         {
//             "name": "C++ Launch (Windows)",
//             "type": "cppdbg",
//             "request": "launch",
//             "program": "${workspaceFolder}/small-examples/x11-windowing/out/X11Windowing",
//             "cwd": "${workspaceFolder}/small-examples/x11-windowing/out",
//             "stopAtEntry": false
//         }
//     ]
// }
//And make sure to install GDB or LLDB. For example, with GDB,
//  sudo apt install gdb


int main() {
    Display* connection = XOpenDisplay(nullptr);
    if (connection == nullptr) {
        cerr << "Failed to open connection to the X windowing server!" << endl;
        return 1;
    }

    int screen = DefaultScreen(connection);
    int visualAttributes[] = {
        GLX_RGBA,
        GLX_DEPTH_SIZE, 24,
        GLX_DOUBLEBUFFER, true,
        None
    };

    // gladLoadGLXInternalLoader
    int glxVersion = gladLoaderLoadGLX(connection, screen);
    if (glxVersion == 0) {
        cerr << "Unable to load GLX." << endl;
        return 2;
    }
    cout << "Loaded GLX " << (glxVersion / 10000) << "." << (glxVersion % 10) << "!" << endl;

    XVisualInfo* info = glXChooseVisual(connection, screen, visualAttributes);
    if (info == nullptr) {
        cerr << "Failed to find a suitable visual." << endl;
        return 3;
    }

    Window rootWindow = RootWindow(connection, info->screen);

    Colormap colormap = XCreateColormap(connection, rootWindow, info->visual, AllocNone);

    XSetWindowAttributes windowAttributes = { };
    windowAttributes.colormap = colormap;
    windowAttributes.event_mask = ExposureMask | KeyPressMask;

    Window window = XCreateWindow(connection, rootWindow, 50, 50, 250, 250, 0,
        info->depth, InputOutput, info->visual, CWColormap | CWEventMask, &windowAttributes);

    Atom deleteWindowAtom = XInternAtom(connection, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(connection, window, &deleteWindowAtom, True);

    XMapWindow(connection, window);
    XStoreName(connection, window, "OpenGL Demo Window");
    XSelectInput(connection, window, ExposureMask);

    GLXContext ctx = glXCreateContext(connection, info, nullptr, GL_TRUE);
    glXMakeCurrent(connection, window, ctx);

    int glVersion = gladLoaderLoadGL();
    if (glVersion == 0) {
        cerr << "Failed to load OpenGL." << endl;
        return 4;
    }
    cout << "Loaded OpenGL " << (glVersion / 10000) << "." << (glVersion % 10) << "!" << endl;

    bool quit = false;
    while (!quit) {
        while (XPending(connection)) {
            XEvent event;
            XNextEvent(connection, &event);

            switch (event.type) {
                case ClientMessage:
                    if (event.xclient.message_type == XInternAtom(connection, "WM_PROTOCOLS", True) &&
                        (Atom) event.xclient.data.l[0] == deleteWindowAtom) {
                            quit = true;
                    }
                    break;
            }
        }

        glViewport(0, 0, 250, 250);
        glClearColor(0, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glXSwapBuffers(connection, window);
        int ms = 10;
        usleep(1000 * ms); //microseconds
    }

    glXMakeCurrent(connection, (unsigned long) NULL, nullptr);
    glXDestroyContext(connection, ctx);

    XFree(info);
    XDestroyWindow(connection, window);
    XFreeColormap(connection, colormap);
    XCloseDisplay(connection);

    gladLoaderUnloadGL();
    gladLoaderUnloadGLX();

    //TODO: Error on close,
    //  X connection to :1 broken (explicit kill or server shutdown).

    return 0;
}
