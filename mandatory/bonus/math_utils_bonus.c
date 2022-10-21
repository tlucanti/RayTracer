/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 17:44:29 by tlucanti          #+#    #+#             */
/*   Updated: 2021/04/20 15:33:53 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ray_traycer.h"

float	triple_product(t_point *n, t_point *a, t_point *p, t_point *v)
{
	return (
		v->x * (n->y * (p->z - a->z) - n->z * (p->y - a->y))
		- v->y * (n->x * (p->z - a->z) - n->z * (p->x - a->x))
		+ v->z * (n->x * (p->y - a->y) - n->y * (p->x - a->x))
	);
}

float	q_rsqrt(float number)
{
	unsigned long	i;
	float			x2;
	float			y;
	void			*ptr;

	x2 = number * 0.5f;
	y = number;
	ptr = &y;
	i = *(unsigned long *)ptr;
	i = 0x5f3759df - (i >> 1u);
	ptr = &i;
	y = *(float *)ptr;
	y = y * (1.5f - (x2 * y * y));
	y = y * (1.5f - (x2 * y * y));
	return (y);
}
