
#ifndef MLXLIB_WINDOW_HPP
# define MLXLIB_WINDOW_HPP

# include "MLXcommon.hpp"
# include "MLXutils.hpp"
# include "MLXcore.hpp"
# include "MLXexception.hpp"
# include "MLXimage.hpp"

# include <string>

MLXLIB_NAMESPACE_BEGIN

namespace keys
{
# ifdef __APPLE__
    inline constexpr int KEY_Q = 12;
    inline constexpr int KEY_W = 13;
    inline constexpr int KEY_E = 14;
    inline constexpr int KEY_R = 15;
    inline constexpr int KEY_T = 17;
    inline constexpr int KEY_Y = 16;
    inline constexpr int KEY_U = 32;
    inline constexpr int KEY_I = 34;
    inline constexpr int KEY_O = 31;
    inline constexpr int KEY_P = 35;
    inline constexpr int KEY_A = 0;
    inline constexpr int KEY_S = 1;
    inline constexpr int KEY_D = 2;
    inline constexpr int KEY_F = 3;
    inline constexpr int KEY_G = 5;
    inline constexpr int KEY_H = 4;
    inline constexpr int KEY_J = 38;
    inline constexpr int KEY_K = 40;
    inline constexpr int KEY_L = 37;
    inline constexpr int KEY_Z = 6;
    inline constexpr int KEY_X = 7;
    inline constexpr int KEY_C = 8;
    inline constexpr int KEY_V = 9;
    inline constexpr int KEY_B = 11;
    inline constexpr int KEY_N = 45;
    inline constexpr int KEY_M = 46;

    inline constexpr int KEY_0 = 29;
    inline constexpr int KEY_1 = 18;
    inline constexpr int KEY_2 = 19;
    inline constexpr int KEY_3 = 20;
    inline constexpr int KEY_4 = 21;
    inline constexpr int KEY_5 = 23;
    inline constexpr int KEY_6 = 22;
    inline constexpr int KEY_7 = 26;
    inline constexpr int KEY_8 = 28;
    inline constexpr int KEY_9 = 25;

    inline constexpr int KEY_MINUS = 27;
    inline constexpr int KEY_PLUS = 24;
    inline constexpr int KEY_BACKSPACE = 51;

    inline constexpr int KEY_TAB = 48;
    inline constexpr int KEY_CAPS = 272;
    inline constexpr int KEY_LSHIFT = 257;
    inline constexpr int KEY_LCTRL = 256;
    inline constexpr int KEY_LALT = 261;
    inline constexpr int KEY_ENTER = 36;
    inline constexpr int KEY_RSHIFT = 258;
    inline constexpr int KEY_RALT = 262;
    inline constexpr int KEY_RCTRL = 269;
    inline constexpr int KEY_SPACE = 49;
    inline constexpr int KEY_ESCAPE = 53;

    inline constexpr int KEY_UP = 126;
    inline constexpr int KEY_DOWN = 125;
    inline constexpr int KEY_LEFT = 123;
    inline constexpr int KEY_RIGHT = 124;
# else
    inline constexpr int KEY_Q = 113;
    inline constexpr int KEY_W = 119;
    inline constexpr int KEY_E = 101;
    inline constexpr int KEY_R = 114;
    inline constexpr int KEY_T = 116;
    inline constexpr int KEY_Y = 121;
    inline constexpr int KEY_U = 117;
    inline constexpr int KEY_I = 105;
    inline constexpr int KEY_O = 111;
    inline constexpr int KEY_P = 112;
    inline constexpr int KEY_A = 97;
    inline constexpr int KEY_S = 115;
    inline constexpr int KEY_D = 100;
    inline constexpr int KEY_F = 102;
    inline constexpr int KEY_G = 103;
    inline constexpr int KEY_H = 104;
    inline constexpr int KEY_J = 106;
    inline constexpr int KEY_K = 107;
    inline constexpr int KEY_L = 108;
    inline constexpr int KEY_Z = 122;
    inline constexpr int KEY_X = 120;
    inline constexpr int KEY_C = 99;
    inline constexpr int KEY_V = 118;
    inline constexpr int KEY_B = 98;
    inline constexpr int KEY_N = 110;
    inline constexpr int KEY_M = 109;

    inline constexpr int KEY_0 = 48;
    inline constexpr int KEY_1 = 49;
    inline constexpr int KEY_2 = 50;
    inline constexpr int KEY_3 = 51;
    inline constexpr int KEY_4 = 52;
    inline constexpr int KEY_5 = 53;
    inline constexpr int KEY_6 = 54;
    inline constexpr int KEY_7 = 55;
    inline constexpr int KEY_8 = 56;
    inline constexpr int KEY_9 = 57;

    inline constexpr int KEY_MINUS = 45;
    inline constexpr int KEY_PLUS = 61;
    inline constexpr int KEY_BACKSPACE = 65288;

    inline constexpr int KEY_TAB = 65289;
    inline constexpr int KEY_CAPS = 65032;
    inline constexpr int KEY_LSHIFT = 65505;
    inline constexpr int KEY_LCTRL = 65507;
    inline constexpr int KEY_LALT = 65513;
    inline constexpr int KEY_ENTER = 65293;
    inline constexpr int KEY_RSHIFT = 65506;
    inline constexpr int KEY_RALT = 65514;
    inline constexpr int KEY_RCTRL = 65508;
    inline constexpr int KEY_SPACE = 32;
    inline constexpr int KEY_ESCAPE = 65307;

    inline constexpr int KEY_UP = 65362;
    inline constexpr int KEY_DOWN = 65364;
    inline constexpr int KEY_LEFT = 65361;
    inline constexpr int KEY_RIGHT = 65363;
# endif
}

namespace mouse
{
    inline constexpr int MOUSE_LEFT = 1;
    inline constexpr int MOUSE_WHEEL = 2;
    inline constexpr int MOUSE_RIGHT = 3;
    inline constexpr int MOUSE_WHEEL_DOWN = 4;
    inline constexpr int MOUSE_WHEEL_UP = 5;
}

namespace events
{
    inline constexpr int key_press = 2;
    inline constexpr int key_release = 3;
    inline constexpr int mouse_press = 4;
    inline constexpr int mouse_release = 5;
    inline constexpr int mouse_move = 6;
    inline constexpr int mouse_enter_window = 7;
    inline constexpr int mouse_leave_window = 8;
    inline constexpr int focus_in = 9;
    inline constexpr int focus_out = 10;
    inline constexpr int window_close = 17;
}

namespace masks
{
    inline constexpr int empty_mask = 0L;
    inline constexpr int key_press = 1L << 0;
    inline constexpr int key_release = 1L << 1;
    inline constexpr int mouse_press = 1L << 2;
    inline constexpr int mouse_release = 1L << 3;
    inline constexpr int mouse_enter_window = 1L << 4;
    inline constexpr int mouse_leave_window = 1L << 5;
    inline constexpr int mouse_motion = 1L << 6;
    inline constexpr int mouse_motion_hint = 1L << 7;
}

class MLXwindow : public __utils::__noncopymovable<>
{
public:
    MLXwindow(const MLXcore &core, unsigned int width, unsigned int height, const std::string &title="")
        : mlx_ptr(core.mlx_ptr)
    {
        if (width == 0 or height == 0)
            throw MLXexception("screen resolution should not be equal to zero");
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

    void put_string(const std::string &str, int x, int y, int color=color::white)
    {
        mlx_string_put(mlx_ptr, window_ptr, x, y, color, const_cast<char *>(str.c_str()));
    }

    template <class T>
    void add_hook(T func, int event, void *param=nullptr, int mask=masks::empty_mask)
    {
        mlx_hook(window_ptr, event, mask, reinterpret_cast<void (*)(...)>(func), param);
    }

    template <class T>
    void add_keyhook(void (*func)(int keycode, T *param), T *param=nullptr)
    {
        mlx_key_hook(window_ptr, reinterpret_cast<void (*)(int, void *)>(func), param);
    }

    void add_mousehook(void (*func)(int button, int x, int y, void *param), void *param=nullptr)
    {
        mlx_mouse_hook(window_ptr, func, param);
    }


    template <class T>
    void add_loop_hook(void (*func)(T *param), T *param=nullptr)
    {
        mlx_loop_hook(mlx_ptr, reinterpret_cast<void (*)(void *)>(func), param);
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
