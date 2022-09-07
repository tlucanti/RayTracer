
#ifndef MLXLIB_WINDOW_HPP
# define MLXLIB_WINDOW_HPP

# include "MLXdefs.hpp"
# include "MLXutils.hpp"
# include "MLXcore.hpp"
# include "MLXexception.hpp"
# include "MLXimage.hpp"

# include <string>

MLXLIB_NAMESPACE_BEGIN

class MLXwindow : public __utils::__noncopymovable<>
{
public:
    MLXwindow(const MLXcore &core, int width=800, int height=600, const std::string &title="")
        : mlx_ptr(core.mlx_ptr)
    {
        window_ptr = mlx_new_window(mlx_ptr, width, height, title.c_str());
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
