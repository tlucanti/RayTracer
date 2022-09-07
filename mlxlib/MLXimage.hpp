
#ifndef MLXLIB_IMAGE_HPP
# define MLXLIB_IMAGE_HPP

# include "MLXdefs.hpp"
# include "MLXcore.hpp"
# include "MLXexception.hpp"

MLXLIB_NAMESPACE_BEGIN

class MLXimage
{
public:
    MLXimage(const MLXcore &core, int width=800, int height=600)
        : mlx_ptr(core.mlx_ptr)
    {
        image_ptr = mlx_new_image(mlx_ptr, width, height);
        if (image_ptr == nullptr)
            throw MLXexception("mlx image initialization error");
    }

    ~MLXimage()
    {
        mlx_destroy_image(mlx_ptr, image_ptr);
    }

private:
    void    *mlx_ptr;
    void    *image_ptr;

    friend class MLXwindow;
};

MLXLIB_NAMESPACE_END

#endif /* MLXLIB_IMAGE_HPP */
