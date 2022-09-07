
#include "mlxlib"

int main()
{
    mlxlib::MLXcore core;
    mlxlib::MLXwindow window(core);
    mlxlib::MLXimage image(core);

    window.put_image(image);
    window.put_string("hello", 200, 200);

    window.event_loop();
}
