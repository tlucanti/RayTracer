
#ifndef MLXLIB_CORE_HPP
# define MLXLIB_CORE_HPP

# include "MLXdefs.hpp"
# include "MLXexception.hpp"

MLXLIB_NAMESPACE_BEGIN

class MLXcore : public __utils::__noncopymovable<>
{
public:
    MLXcore()
    {
        mlx_ptr = mlx_init();
        if (mlx_ptr == nullptr)
            throw MLXexception("MLX core initialization error");
    }

    ~MLXcore() = default;

private:
    void    *mlx_ptr;

    friend class MLXwindow;
    friend class MLXimage;
};

MLXLIB_NAMESPACE_END

#endif /* MLXLIB_CORE_HPP */
