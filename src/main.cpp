/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 20:13:35 by tlucanti          #+#    #+#             */
/*   Updated: 2022/03/09 22:15:13 by tlucanti         ###   ########.fr       */
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

inline auto now() { return std::chrono::high_resolution_clock::now(); }
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

void speed_test_stl()
{
	std::vector<int> v(10000000, 1);
	auto t1 = now();
	for (auto i : v)
		i = i * i;
	std::cout << "stl array 10e8 operations: " <<
		duration<double, std::milli>(now() - t1).count() << "ms\n";
}

void speed_test_gpu()
{
	tlucanti::GPUKernel sum('+');
	std::vector<int> v(10000000, 1);
	tlucanti::GPUArray a(v);

	sum.set_operands(a, a, a);

	auto t1 = now();
	sum.do_operation();
	std::cout << "gpu array 10e8 operations: " <<
		duration<double, std::milli>(now() - t1).count() << "ms\n";
}

void test()
{
	tlucanti::GPUKernel sum('+');
	tlucanti::GPUKernel sub('-');
	tlucanti::GPUKernel mul('*');
	tlucanti::GPUKernel div('/');

	std::vector<int> va = {1, 2, 3};
	std::vector<int> vb = {4, 5, 6};
	std::vector<int> vc(3, 0);
	std::vector<int> vsum;
	std::vector<int> vsub;
	std::vector<int> vmul;
	std::vector<int> vdiv;

	tlucanti::GPUArray a(va, "r");
	tlucanti::GPUArray b(vb, "r");
	tlucanti::GPUArray sum_res(vc, "w");
	tlucanti::GPUArray sub_res(vc, "w");
	tlucanti::GPUArray mul_res(vc, "w");
	tlucanti::GPUArray div_res(vc, "w");

	sum.set_operands(a, b, sum_res);
	sub.set_operands(a, b, sub_res);
	mul.set_operands(a, b, mul_res);
	div.set_operands(b, a, div_res);

	sum.do_operation();
	sub.do_operation();
	mul.do_operation();
	div.do_operation();

	a.dump(va);
	b.dump(vb);
	sum_res.dump(vsum);
	sub_res.dump(vsub);
	mul_res.dump(vmul);
	div_res.dump(vdiv);

	for (size_t i=0; i < vsum.size(); ++i)
		std::cout << va[i] << " + " << vb[i] << " = " << vsum[i] << std::endl;
	std::cout << std::endl;
	for (size_t i=0; i < vsub.size(); ++i)
		std::cout << va[i] << " - " << vb[i] << " = " << vsub[i] << std::endl;
	std::cout << std::endl;
	for (size_t i=0; i < vmul.size(); ++i)
		std::cout << va[i] << " * " << vb[i] << " = " << vmul[i] << std::endl;
	std::cout << std::endl;
	for (size_t i=0; i < vdiv.size(); ++i)
		std::cout << vb[i] << " / " << va[i] << " = " << vdiv[i] << std::endl;
	std::cout << std::endl;

	tlucanti::GPUKernel::destroy();
}

int main()
{
	speed_test_stl();
	speed_test_gpu();
}
