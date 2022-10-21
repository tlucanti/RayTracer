/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 19:16:10 by tlucanti          #+#    #+#             */
/*   Updated: 2021/04/20 15:33:06 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ray_traycer.h"

int 	camera(t_camera_input *inp, t_camera **dest)
{
	t_camera	*new_camera;

	new_camera = (t_camera *)malloc(sizeof(t_camera));
	if (!new_camera)
		return (1 + ft_free(inp->o) + ft_free(inp->view_vector));
	new_camera->fovx = 2.f * tanf(inp->fovx * PI / 360.f) / g_scene->res_x;
	new_camera->fovy = 2.f * tanf(inp->fovy * (float)g_scene->res_y
			 / (float)g_scene->res_x * PI / 360.f) / (float)g_scene->res_y;
	set_angles(new_camera, inp->view_vector);
	new_camera->gamma = 0;
	new_camera->o = inp->o;
	new_camera->view_vector = inp->view_vector;
	new_camera->rotation_matrix = (t_matrix3 *)malloc(sizeof(t_matrix3));
	if (!new_camera->rotation_matrix)
		return (1 + ft_free(inp->o) + ft_free(inp->view_vector));
	rotate_matrix(new_camera->rotation_matrix, new_camera->alpha,
		new_camera->theta, new_camera->gamma);
	*dest = new_camera;
	return (0);
}

static float    signf(float f)
{
    if (f > 0)
        return (1.f);
    if (f < 0)
        return (-1.f);
    return (0.f);
}

void	set_angles(t_camera *new_camera, t_point *view_vector)
{
	if (fabsf(view_vector->x) + fabsf(view_vector->z) < EPS)
	{
		new_camera->alpha = 0;
		new_camera->theta = -signf(view_vector->y) * PI_HALF;
	}
	else
	{
		new_camera->alpha = -atanf(-view_vector->x / view_vector->z);
		if (view_vector->z < 0)
			new_camera->alpha += PI;
		new_camera->theta = -atanf(view_vector->y
				/ sqrtf(view_vector->x * view_vector->x
					+ view_vector->z * view_vector->z));
	}
}
