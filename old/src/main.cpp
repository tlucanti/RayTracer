/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 20:13:35 by tlucanti          #+#    #+#             */
/*   Updated: 2022/03/31 11:20:18 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <iostream>
#include <ctime>

#include <GPUKernel.hpp>
#include <GPUArray.hpp>
#include <File.hpp>

#include "window.hpp"

#ifdef __CMAKE
const char *SUM_PROG_PATH = "../cl/sum.cl";
#else /* not __CMAKE */
const char *SUM_PROG_PATH = "./cl/sum.cl";
#endif /* __CMAKE */

namespace hidden
{
    struct timespec *_timer_get() {
        static struct timespec ts = {};
        return &ts;
    }
}

void time_start()
{
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, hidden::_timer_get());
}

double time_end()
{
    struct timespec ts = {};
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
    struct timespec *g_ts = hidden::_timer_get();
    time_t sec = ts.tv_sec - g_ts->tv_sec;
    long nsec = ts.tv_nsec - g_ts->tv_nsec;
    return static_cast<double>(sec) + static_cast<double>(nsec) * 1e-9;
}

void speed_test_stl() {
    std::vector<int> v(10000000, 1);
    time_start();
    for (auto &i : v)
        i = i * i;
    double t = time_end();
    std::cout << "stl array 10e8 operations: " << t * 1e3 << "ms\n";
}

void speed_test_gpu() {
    tlucanti::GPUKernel<'+'> sum;
    std::vector<int> v(10000000, 1);
    tlucanti::GPUArray a(v);

    sum.set_operands(a, a, a);

    time_start();
    sum.do_operation();
    double t = time_end();
    std::cout << "gpu array 10e8 operations: " << t * 1e3 << "ms\n";
}

void test() {
    tlucanti::GPUKernel<'+'> sum;
    tlucanti::GPUKernel<'-'> sub;
    tlucanti::GPUKernel<'*'> mul;
    tlucanti::GPUKernel<'/'> div;

    std::vector<int> va = {1, 2, 3};
    std::vector<int> vb = {4, 5, 6};
    std::vector<int> vc(3, 0);
    std::vector<int> vsum;
    std::vector<int> vsub;
    std::vector<int> vmul;
    std::vector<int> vdiv;

    tlucanti::GPUArray<int, 'r'> lol(10);
    tlucanti::GPUArray<int, 'r'> a(va);
    tlucanti::GPUArray<int, 'r'> b(vb);
    tlucanti::GPUArray<int, 'w'> sum_res(vc);
    tlucanti::GPUArray<int, 'w'> sub_res(vc);
    tlucanti::GPUArray<int, 'w'> mul_res(vc);
    tlucanti::GPUArray<int, 'w'> div_res(vc);

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

    for (size_t i = 0; i < vsum.size(); ++i)
        std::cout << va[i] << " + " << vb[i] << " = " << vsum[i] << std::endl;
    std::cout << std::endl;
    for (size_t i = 0; i < vsub.size(); ++i)
        std::cout << va[i] << " - " << vb[i] << " = " << vsub[i] << std::endl;
    std::cout << std::endl;
    for (size_t i = 0; i < vmul.size(); ++i)
        std::cout << va[i] << " * " << vb[i] << " = " << vmul[i] << std::endl;
    std::cout << std::endl;
    for (size_t i = 0; i < vdiv.size(); ++i)
        std::cout << vb[i] << " / " << va[i] << " = " << vdiv[i] << std::endl;
    std::cout << std::endl;

    tlucanti::GPUContext::destroy();
}

int main() {
    speed_test_stl();
    speed_test_gpu();

//    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//    if (ren == nullptr) {
//        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
//        return 1;
//    }
//    while (1);
}
