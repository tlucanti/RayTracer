
# ifdef __APPLE__
#  include <OpenCL/opencl.h>
# else /* not __APPLE__ */
#  include <CL/cl.h>
# endif /* __APPLE__ */

# include <iostream>
# include <vector>

static const char *cl_strerror(int cl_errno)
{
    switch(cl_errno)
    {
        // run-time and JIT compiler errors
        case 0: return "CL_SUCCESS";
        case -1: return "CL_DEVICE_NOT_FOUND";
        case -2: return "CL_DEVICE_NOT_AVAILABLE";
        case -3: return "CL_COMPILER_NOT_AVAILABLE";
        case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
        case -5: return "CL_OUT_OF_RESOURCES";
        case -6: return "CL_OUT_OF_HOST_MEMORY";
        case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
        case -8: return "CL_MEM_COPY_OVERLAP";
        case -9: return "CL_IMAGE_FORMAT_MISMATCH";
        case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
        case -11: return "CL_BUILD_PROGRAM_FAILURE";
        case -12: return "CL_MAP_FAILURE";
        case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
        case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
        case -15: return "CL_COMPILE_PROGRAM_FAILURE";
        case -16: return "CL_LINKER_NOT_AVAILABLE";
        case -17: return "CL_LINK_PROGRAM_FAILURE";
        case -18: return "CL_DEVICE_PARTITION_FAILED";
        case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

            // compile-time errors
        case -30: return "CL_INVALID_VALUE";
        case -31: return "CL_INVALID_DEVICE_TYPE";
        case -32: return "CL_INVALID_PLATFORM";
        case -33: return "CL_INVALID_DEVICE";
        case -34: return "CL_INVALID_CONTEXT";
        case -35: return "CL_INVALID_QUEUE_PROPERTIES";
        case -36: return "CL_INVALID_COMMAND_QUEUE";
        case -37: return "CL_INVALID_HOST_PTR";
        case -38: return "CL_INVALID_MEM_OBJECT";
        case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
        case -40: return "CL_INVALID_IMAGE_SIZE";
        case -41: return "CL_INVALID_SAMPLER";
        case -42: return "CL_INVALID_BINARY";
        case -43: return "CL_INVALID_BUILD_OPTIONS";
        case -44: return "CL_INVALID_PROGRAM";
        case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
        case -46: return "CL_INVALID_KERNEL_NAME";
        case -47: return "CL_INVALID_KERNEL_DEFINITION";
        case -48: return "CL_INVALID_KERNEL";
        case -49: return "CL_INVALID_ARG_INDEX";
        case -50: return "CL_INVALID_ARG_VALUE";
        case -51: return "CL_INVALID_ARG_SIZE";
        case -52: return "CL_INVALID_KERNEL_ARGS";
        case -53: return "CL_INVALID_WORK_DIMENSION";
        case -54: return "CL_INVALID_WORK_GROUP_SIZE";
        case -55: return "CL_INVALID_WORK_ITEM_SIZE";
        case -56: return "CL_INVALID_GLOBAL_OFFSET";
        case -57: return "CL_INVALID_EVENT_WAIT_LIST";
        case -58: return "CL_INVALID_EVENT";
        case -59: return "CL_INVALID_OPERATION";
        case -60: return "CL_INVALID_GL_OBJECT";
        case -61: return "CL_INVALID_BUFFER_SIZE";
        case -62: return "CL_INVALID_MIP_LEVEL";
        case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
        case -64: return "CL_INVALID_PROPERTY";
        case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
        case -66: return "CL_INVALID_COMPILER_OPTIONS";
        case -67: return "CL_INVALID_LINKER_OPTIONS";
        case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

            // extension errors
        case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
        case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
        case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
        case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
        case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
        case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
        default: return "Unknown OpenCL error";
    }
}

const char *kernel_code =
"__kernel void add(__global const int *a, __global const int *b, __global int *c)"
"{"
"    int i = get_global_id(0);"
"    c[i] = a[i] + b[i];"
"}";

const char *kernel2_code =
"__kernel void addf(__global const float *a, float b, __global float *c)"
"{"
"    int i = get_global_id(0);"
"    c[i] = a[i] + b;"
"}";



int main()
{
    cl_int          error;

    // platform ids
    cl_platform_id  platform_id;
    cl_uint         num_platforms;
    error = clGetPlatformIDs(1, &platform_id, &num_platforms);
    std::cout << "platform_num: " << num_platforms << std::endl;
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror(error));

    // platform info
    size_t info_size;
    std::string info;
    error = clGetPlatformInfo(platform_id, CL_PLATFORM_NAME, 0, nullptr, &info_size);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror(error));
    info.resize(info_size - 1);
    error = clGetPlatformInfo(platform_id, CL_PLATFORM_NAME, info_size, info.data(), nullptr);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror(error));
    std::cout << "platform name: " << info << std::endl;

    // devices
    cl_device_id    device_id;
    cl_uint         num_devices;
    error = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &num_devices);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror(error));

    // device info
    error = clGetDeviceInfo(device_id, CL_DEVICE_NAME, 0, nullptr, &info_size);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror(error));
    info.resize(info_size - 1);
    error = clGetDeviceInfo(device_id, CL_DEVICE_NAME, info_size, info.data(), nullptr);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror(error));
    std::cout << "device name: " << info << std::endl;

    // context
    cl_context      context;
    context = clCreateContext(nullptr, 1, &device_id, nullptr, nullptr, &error);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));

    // queue
    cl_command_queue    queue;
    queue = clCreateCommandQueueWithProperties(context, device_id, nullptr, &error);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));

# if 0
    // program
    cl_program      program;
    program = clCreateProgramWithSource(context, 1, &kernel_code, nullptr, &error);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));

    // program info
//    error = clGetProgramInfo(program, CL_PROGRAM_KERNEL_NAMES_APPLE, 0, nullptr, &info_size);
//    if (error != CL_SUCCESS)
//        throw std::runtime_error(cl_strerror(error));
//    info.resize(/*info_size*/ 3000 - 1);
//                error = clGetProgramInfo(program, CL_PROGRAM_KERNEL_NAMES, 3000 - 1, info.data(), &info_size);
//    if (error != CL_SUCCESS)
//        throw std::runtime_error(cl_strerror(error));
//    std::cout << "kernel names: " << info << std::endl;

    // compile
    error = clBuildProgram(program, 1, &device_id, nullptr, nullptr, nullptr);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));

    // kernel
    cl_kernel       kernel;
    const char *kernel_name = "add";
    kernel = clCreateKernel(program, kernel_name, &error);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));

    // buffers
    cl_mem buffer1, buffer2, buffer3;
    std::vector<int> arr1 = {1, 2, 3, 4, 5};
    std::vector<int> arr2 = {6, 7, 8, 9, 0};
    std::vector<int> arr3(arr1.size());

    // create buffers
    buffer1 = clCreateBuffer(context, CL_MEM_READ_WRITE, arr1.size() * sizeof(int), nullptr, &error);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));
    buffer2 = clCreateBuffer(context, CL_MEM_READ_WRITE, arr2.size() * sizeof(int), nullptr, &error);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));
    buffer3 = clCreateBuffer(context, CL_MEM_READ_WRITE, arr3.size() * sizeof(int), nullptr, &error);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));

    // fill buffers
    error = clEnqueueWriteBuffer(queue, buffer1, CL_TRUE, 0, arr1.size() * sizeof(int), arr1.data(), 0, nullptr, nullptr);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));
    error = clEnqueueWriteBuffer(queue, buffer2, CL_TRUE, 0, arr2.size() * sizeof(int), arr2.data(), 0, nullptr, nullptr);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));
    error = clEnqueueWriteBuffer(queue, buffer3, CL_TRUE, 0, arr3.size() * sizeof(int), arr3.data(), 0, nullptr, nullptr);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));

    // set args
    error =  clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer1);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));
    error =  clSetKernelArg(kernel, 1, sizeof(cl_mem), &buffer2);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));
    error =  clSetKernelArg(kernel, 2, sizeof(cl_mem), &buffer3);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));

    // run kernel
    size_t work_size[1] = { arr1.size() };
    error = clEnqueueNDRangeKernel(queue, kernel, 1, nullptr, work_size, nullptr, 0, nullptr, nullptr);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));

    // get result
    error = clEnqueueReadBuffer(queue, buffer3, CL_TRUE, 0, arr3.size() * sizeof(int), arr3.data(), 0, nullptr, nullptr);

    for (auto i : arr3)
        std::cout << i << ' ';
    std::cout << '\n';
#endif

    // program
    cl_program      program;
    program = clCreateProgramWithSource(context, 1, &kernel2_code, nullptr, &error);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));

    // program info
//    error = clGetProgramInfo(program, CL_PROGRAM_KERNEL_NAMES_APPLE, 0, nullptr, &info_size);
//    if (error != CL_SUCCESS)
//        throw std::runtime_error(cl_strerror(error));
//    info.resize(/*info_size*/ 3000 - 1);
//                error = clGetProgramInfo(program, CL_PROGRAM_KERNEL_NAMES, 3000 - 1, info.data(), &info_size);
//    if (error != CL_SUCCESS)
//        throw std::runtime_error(cl_strerror(error));
//    std::cout << "kernel names: " << info << std::endl;

    // compile
    error = clBuildProgram(program, 1, &device_id, nullptr, nullptr, nullptr);
    if (error != CL_SUCCESS)
    {
        error = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, nullptr, &info_size);
        if (error != CL_SUCCESS)
            throw std::runtime_error(cl_strerror(error));
        info.resize(info_size);
        error = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, info_size, info.data(), nullptr);
        if (error != CL_SUCCESS)
            throw std::runtime_error(cl_strerror(error));
        std::cout << "compile error\n" << info << std::endl;
        throw std::runtime_error(cl_strerror((error)));
    }

    // kernel
    cl_kernel       kernel;
    const char *kernel_name = "addf";
    kernel = clCreateKernel(program, kernel_name, &error);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));

    // buffers
    cl_mem buffer1, buffer3;
    std::vector<float> arr1 = {1.5, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    std::vector<float> arr3(arr1.size(), -1);

    // create buffers
    buffer1 = clCreateBuffer(context, CL_MEM_READ_WRITE, arr1.size() * sizeof(int), nullptr, &error);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));
    buffer3 = clCreateBuffer(context, CL_MEM_READ_WRITE, arr3.size() * sizeof(int), nullptr, &error);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));

    // fill buffers
    error = clEnqueueWriteBuffer(queue, buffer1, CL_TRUE, 0, arr1.size() * sizeof(float), arr1.data(), 0, nullptr, nullptr);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));
    error = clEnqueueWriteBuffer(queue, buffer3, CL_TRUE, 0, arr3.size() * sizeof(float), arr3.data(), 0, nullptr, nullptr);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));

    // set args
    float   arg1 = 0.5;
    error =  clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer1);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));
    error =  clSetKernelArg(kernel, 1, sizeof(float), &arg1);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));
    error =  clSetKernelArg(kernel, 2, sizeof(cl_mem), &buffer3);
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));

    // run kernel
    size_t local_size[1] = { 4 };
    size_t global_size[1] = { 16 };
    error = clEnqueueNDRangeKernel(
        queue,
        kernel,
        1,
        nullptr,
        global_size,
        local_size,
        0,
        nullptr,
        nullptr
    );
    if (error != CL_SUCCESS)
        throw std::runtime_error(cl_strerror((error)));


    // get result
    error = clEnqueueReadBuffer(queue, buffer3, CL_TRUE, 0, arr3.size() * sizeof(int), arr3.data(), 0, nullptr, nullptr);

    for (auto i : arr3)
        std::cout << i << ' ';
    std::cout << '\n';
}