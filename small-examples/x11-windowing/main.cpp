#include <X11/Xlib.h>
#include <cstring>

using std::strlen;

int main() {
    Display* display = XOpenDisplay(nullptr);
    int screen = 0;
    Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 50, 50, 250, 250, 0, BlackPixel(display, screen), WhitePixel(display, screen));
    XMapWindow(display, window);
    XSelectInput(display, window, ExposureMask);

    XEvent event;
    while (true) {
        XNextEvent(display, &event);
        switch (event.type) {
            case Expose:
                const char* message = "Hello X11 Windowing!";
                int length = strlen(message);
                XDrawString(display, window, DefaultGC(display, screen), 6, 20, message, length);
                break;
        }
    }

    return 0;
}
