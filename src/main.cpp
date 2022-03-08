/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 20:13:35 by tlucanti          #+#    #+#             */
/*   Updated: 2022/03/08 20:14:06 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <iostream>

#include "../inc/GPUKernel.hpp"
#include "../inc/GPUArray.hpp"
#include "../inc/File.hpp"

#ifdef __CMAKE
const char *SUM_PROG_PATH = "../cl/sum.cl";

#else /* not __CMAKE */
const char *SUM_PROG_PATH = "./cl/sum.cl";

#endif /* __CMAKE */

int		main()
{
	std::string sum_code = tlucanti::File(SUM_PROG_PATH).text();
	tlucanti::GPUKernel ker(sum_code);

	std::vector<int> va = {1, 2, 3};
	std::vector<int> vb = {4, 5, 6};
	std::vector<int> vc(3, 0);

	tlucanti::GPUArray a(va, "r");
	tlucanti::GPUArray b(vb, "r");
	tlucanti::GPUArray c(vc, "w");

	ker.set_operands(a, b, c);
	ker.do_operation();

	a.dump(va);
	b.dump(vb);
	c.dump(vc);

	for (size_t i=0; i < vc.size(); ++i)
		std::cout << va[i] << " + " << vb[i] << " = " << vc[i] << std::endl;
}
