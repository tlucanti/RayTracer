
#ifndef MLXLIB_IMAGE_HPP
# define MLXLIB_IMAGE_HPP

# include "MLXcommon.hpp"
# include "MLXcore.hpp"
# include "MLXexception.hpp"

# include <vector>

MLXLIB_NAMESPACE_BEGIN

class MLXimage
{
public:
    MLXimage(const MLXcore &core, int width, int height)
        : mlx_ptr(core.mlx_ptr), image_ptr(nullptr), width(width), height(height)
    {
        image_ptr = mlx_new_image(mlx_ptr, width, height);
        image_data = reinterpret_cast<unsigned int *>(
            mlx_get_data_addr(image_ptr, &bits_per_pixel, &size_line, &endian));
        if (image_ptr == nullptr)
            throw MLXexception("mlx image initialization error");
    }

    ~MLXimage()
    {
        mlx_destroy_image(mlx_ptr, image_ptr);
    }

    void fill(const std::vector<unsigned int> &pixel_data, size_t offset=0)
    {
        if (pixel_data.size() - offset != width * height)
            throw MLXexception("pixel data size mismatch");
        for (size_t i=0; i < pixel_data.size(); ++i)
            image_data[i] = pixel_data[i];
    }

    WUR unsigned int *raw_pixel_data() const NOEXCEPT
    {
        return image_data;
    }

    void put_pixel(unsigned int x, unsigned int y, unsigned int color)
    {
        image_data[y * width + x] = color;
    }

private:
    void    *mlx_ptr;
    void    *image_ptr;

    unsigned int    *image_data;
    int             bits_per_pixel;
    int             size_line;
    int             endian;

    int     width;
    int     height;

    friend class MLXwindow;
};

MLXLIB_NAMESPACE_END

#endif /* MLXLIB_IMAGE_HPP */
