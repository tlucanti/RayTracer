/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add.cl                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:56:57 by tlucanti          #+#    #+#             */
/*   Updated: 2022/03/09 19:57:10 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// name      [-------------]
__kernel void           add(__global const int *a, __global const int *b, __global int *c)
{
    int i = get_global_id(0);
    c[i] = a[i] + b[i];
}
