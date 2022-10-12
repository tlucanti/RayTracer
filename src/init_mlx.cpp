
#include <rtx.hpp>
#include <event_hooks.hpp>

void rtx::init_mlx()
{
    mlxlib::MLXcore core;
    rtx::data::win = new mlxlib::MLXwindow(core, rtx::config::width, rtx::config::height);
    rtx::data::img = new mlxlib::MLXimage(core, (rtx::config::width), (rtx::config::height));

    //    win.add_keyhook(keypress_hook);
    rtx::data::win->add_hook(rtx::hooks::keypress_hook, mlxlib::events::key_press, nullptr, mlxlib::masks::key_press);
    rtx::data::win->add_hook(rtx::hooks::keyrelease_hook, mlxlib::events::key_release, nullptr, mlxlib::masks::key_release);
    rtx::data::win->add_hook(rtx::hooks::mouse_hook, mlxlib::events::mouse_move, nullptr, mlxlib::masks::mouse_motion);
//    win.add_hook(enter_hook, mlxlib::events::mouse_enter_window, nullptr, mlxlib::masks::mouse_enter_window);
    rtx::data::win->add_hook(collapse, mlxlib::events::window_close, nullptr);
    rtx::data::win->add_hook(rtx::hooks::mousepress_hook, mlxlib::events::mouse_press, nullptr, mlxlib::masks::mouse_press);
    rtx::data::win->add_hook(rtx::hooks::mouserelease_hook, mlxlib::events::mouse_release, nullptr, mlxlib::masks::mouse_release);
    rtx::data::win->add_loop_hook(rtx::hooks::framehook);

}