
#ifndef MLXLIB_WINDOW_HPP
# define MLXLIB_WINDOW_HPP

# include "MLXdefs.hpp"
# include "MLXutils.hpp"
# include "MLXcore.hpp"
# include "MLXexception.hpp"
# include "MLXimage.hpp"

# include <string>

MLXLIB_NAMESPACE_BEGIN

namespace keys
{
    const int KEY_Q = 113;
    const int KEY_W = 119;
    const int KEY_E = 101;
    const int KEY_R = 114;
    const int KEY_T = 116;
    const int KEY_Y = 121;
    const int KEY_U = 117;
    const int KEY_I = 105;
    const int KEY_O = 111;
    const int KEY_P = 112;
    const int KEY_A = 97;
    const int KEY_S = 115;
    const int KEY_D = 100;
    const int KEY_F = 102;
    const int KEY_G = 103;
    const int KEY_H = 104;
    const int KEY_J = 106;
    const int KEY_K = 107;
    const int KEY_L = 108;
    const int KEY_Z = 122;
    const int KEY_X = 120;
    const int KEY_C = 99;
    const int KEY_V = 118;
    const int KEY_B = 98;
    const int KEY_N = 110;
    const int KEY_M = 109;

    const int KEY_0 = 48;
    const int KEY_1 = 49;
    const int KEY_2 = 50;
    const int KEY_3 = 51;
    const int KEY_4 = 52;
    const int KEY_5 = 53;
    const int KEY_6 = 54;
    const int KEY_7 = 55;
    const int KEY_8 = 56;
    const int KEY_9 = 57;
    const int KEY_MINUS = 45;
    const int KEY_PLUS = 61;
    const int KEY_BACKSPACE = 65288;

    const int KEY_TAB = 65289;
    const int KEY_CAPS = 65032;
    const int KEY_LSHIFT = 65505;
    const int KEY_LCTRL = 65507;
    const int KEY_LALT = 65513;
    const int KEY_ENTER = 65293;
    const int KEY_RSHIFT = 65506;
    const int KEY_RALT = 65514;
    const int KEY_RCTRL = 65508;
}

namespace events
{
    const int key_press = 2;
    const int key_release = 3;
    const int mouse_press = 4;
    const int mouse_release = 5;
    const int mouse_move = 6;
    const int mouse_enter_window = 7;
    const int mouse_leave_window = 8;
    const int focus_in = 9;
    const int focus_out = 10;
}

namespace masks
{
    const int empty_mask = 0L;
    const int key_press = 1L << 0;
    const int key_release = 1L << 1;
    const int mouse_press = 1L << 2;
    const int mouse_release = 1L << 3;
    const int mouse_enter_window = 1L << 4;
    const int mouse_leave_window = 1L << 5;
    const int mouse_motion = 1L << 6;
    const int mouse_motion_hint = 1L << 7;
}

class MLXwindow : public __utils::__noncopymovable<>
{
public:
    MLXwindow(const MLXcore &core, int width=800, int height=600, const std::string &title="")
        : mlx_ptr(core.mlx_ptr)
    {
        window_ptr = mlx_new_window(mlx_ptr, width, height, const_cast<char *>(title.c_str()));
        if (window_ptr == nullptr)
            throw MLXexception("mlx window initialization error");
    }

    ~MLXwindow()
    {
        mlx_destroy_window(mlx_ptr, window_ptr);
    }

    void clear()
    {
        mlx_clear_window(mlx_ptr, window_ptr);
    }

    void put_image(const MLXimage &image, int x_offset=0, int y_offset=0)
    {
        mlx_put_image_to_window(mlx_ptr, window_ptr, image.image_ptr, x_offset, y_offset);
    }

    void put_string(const std::string &str, int x, int y, int color=Color::white)
    {
        mlx_string_put(mlx_ptr, window_ptr, x, y, color, const_cast<char *>(str.c_str()));
    }

    template <class T>
    void add_hook(T func, int event, void *param=nullptr, int mask=masks::empty_mask)
    {
        mlx_hook(window_ptr, event, mask, reinterpret_cast<void (*)(...)>(func), param);
    }

    template <class T>
    void add_keyhook(void (*func)(int keycode, T *param), void *param=nullptr)
    {
        mlx_key_hook(window_ptr, reinterpret_cast<void (*)(int, void *)>(func), param);
    }

    NORET void event_loop()
    {
        mlx_loop(mlx_ptr);
    }

private:
    void    *mlx_ptr;
    void    *window_ptr;
};

MLXLIB_NAMESPACE_END

#endif /* MLXLIB_WINDOW_HPP */
