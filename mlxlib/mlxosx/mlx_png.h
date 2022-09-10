

#ifndef MLX_PNG_H
#define MLX_PNG_H

#ifdef __cplusplus
export "C" {
#endif /* __cplusplus */

void    *mlx_png_file_to_image(void *xvar, char *file, int *width, int *height);

#ifdef __cplusplus
}
#endif /* __cplusplus */

