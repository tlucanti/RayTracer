/*
Компилируется след. строкой:   cc test.c -o test -lX11
*/

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <stdexcept>
#include <string>

class XWindow
{
public:
    XWindow() = default;

    void create_window(int width, int height, int start_x=10, int start_y=10)
    {
        _display = XOpenDisplay(getenv("DISPLAY"));
        if (_display == nullptr)
            throw std::runtime_error(std::string("cannot connect to X server") + strerror(errno));

        _screen = XDefaultScreen(_display);

        XSetWindowAttributes _attr = {
                ParentRelative, /* backgroud pixelmap */
                XBlackPixel(_display, _screen), /* background color */
                CopyFromParent, /* border pixelmap */
                XWhitePixel(_display, _screen), /* border color */
                0, /* bit gravity */
                0, /* window gravity */
                0, /* backing store */
                0, /* backing planes */
                0, /* backing pixel */
                0, /* save under */
                0, /* saved events */
                0, /* not propogated events */
                0, /* override redirect */
                0, /* colormap */
                0 /* cursor */
        };

        _window = XCreateWindow(
                _display, /* display */
                RootWindow(_display, _screen), /* parent */
                start_x, start_y, /* start coordinates of window */
                width, height, /* size of window */
                1, /* border width */
                1, /* depth */
                InputOutput, /* window class */
                CopyFromParent, /* visual class */
                0, /* window attributes mask */
                &_attr /* window attributes */
        );
    }

    void close_hook(void (*func)())
    {
        XSelectInput(_display, _window, KeyPressMask);
        _close_func = func;
    }

    void loop()
    {
        XMapWindow(_display, _window);
        while (true)
        {
            XNextEvent(_display, &_event);

            switch (_event.type)
            {
                case KeyPress:
                {
                    if (_close_func != nullptr)
                    {
                        _close_func();
                        return ;
                    }
                    break ;
                }
            }
        }
    }

    ~XWindow()
    {
        XCloseDisplay(_display);
    }

private:
    Display *_display;
    int     _screen;
    Window  _window;
    XEvent  _event;

    void (*_close_func)() = nullptr;
};

void close_win() {}

int main()
{
    XWindow win;
    win.create_window(200, 200);
    win.close_hook(&close_win);
    win.loop();
}

int lol() {
 Display *d;
 Window w;
 XEvent e;
 const char *msg = "Hello, World!";
 int s;

 /* Соединиться с X сервером, если X сервер на удаленной машине
  * следует разрешить на машине, где запущен X Server 
  * удаленные соединения командой xhost+ (см. man xhost)
  */
  if ((d = XOpenDisplay(getenv("DISPLAY"))) == NULL) {
printf("Can't connect X server:%s\n", strerror(errno));
exit(1);
  }

 s = XDefaultScreen(d);

 /* Создать окно */
 w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 200, 200, 1,
                         XBlackPixel(d, s), XWhitePixel(d, s));

 /* На какие события будем реагировать */
 XSelectInput(d, w, KeyPressMask);

/* Вывести окно на экран */
 XMapWindow(d, w);

 /* Бесконечный цикл обработки событий */
 while (1) {
   XNextEvent(d, &e);

   
  /* Перерисовать окно /
   if (e.type == Expose) {
     XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 10, 10);
     XDrawString(d, w, DefaultGC(d, s), 50, 50, msg, strlen(msg));
   }
   */
 /* При нажатии кнопки-выход */
   if (e.type == KeyPress)
     break;
 }

/* Закрыть соединение с X сервером */
 XCloseDisplay(d);

 return 0;
}
