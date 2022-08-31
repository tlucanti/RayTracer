
#ifndef CLLIB_DEVICE_HPP
# define CLLIB_DEVICE_HPP

# include "defs.hpp"
//# include "CLplatform.hpp"
# include "CLexception.hpp"

# include <vector>

CLLIB_NAMESPACE_BEGIN

class CLdevice
{
    static const cl_device_type any_type            = CL_DEVICE_TYPE_DEFAULT;
    static const cl_device_type gpu_type            = CL_DEVICE_TYPE_GPU;
    static const cl_device_type cpu_type            = CL_DEVICE_TYPE_CPU;
    static const cl_device_type accelerator_type    = CL_DEVICE_TYPE_ACCELERATOR;
    static const cl_device_type all_types           = CL_DEVICE_TYPE_ALL;
#ifdef CL_DEVICE_TYPE_CUSTOM
        static const cl_device_type custom_type         = CL_DEVICE_TYPE_CUSTOM;
#endif /* CL_DEVICE_TYPE_CUSTOM */

    WUR UNUSED cl_uint get_device_address_bits() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_ADDRESS_BITS, sizeof(cl_uint)));
    }

    WUR cl_bool is_avaliable() const
    {
        return static_cast<cl_bool>(_get_numeric_data(CL_DEVICE_AVAILABLE, sizeof(cl_bool)));
    }

    WUR UNUSED std::string get_device_builtin_kernels() const
    {
        return _get_string_data(CL_DEVICE_BUILT_IN_KERNELS);
    }

    WUR UNUSED cl_bool is_compiler_avaliable() const
    {
        return static_cast<cl_bool>(_get_numeric_data(CL_DEVICE_COMPILER_AVAILABLE, sizeof(cl_bool)));
    }

    WUR UNUSED cl_device_fp_config get_device_fp_config() const
    {
        return static_cast<cl_device_fp_config>(_get_numeric_data(CL_DEVICE_DOUBLE_FP_CONFIG, sizeof(cl_device_fp_config)));
    }

    WUR UNUSED cl_bool is_little_endian() const
    {
        return static_cast<cl_bool>(_get_numeric_data(CL_DEVICE_ENDIAN_LITTLE, sizeof(cl_bool)));
    }

    WUR UNUSED cl_bool is_error_correction_supported() const
    {
        return static_cast<cl_bool>(_get_numeric_data(CL_DEVICE_ERROR_CORRECTION_SUPPORT, sizeof(cl_bool)));
    }

    WUR UNUSED cl_device_exec_capabilities get_device_exec_capabilities() const
    {
        return static_cast<cl_device_exec_capabilities>(
            _get_numeric_data(
                CL_DEVICE_EXECUTION_CAPABILITIES,
                sizeof(cl_device_exec_capabilities)
            )
        );
    }

    WUR UNUSED std::string get_device_extensions() const
    {
        return _get_string_data(CL_DEVICE_EXTENSIONS);
    }

    WUR UNUSED cl_ulong get_device_global_mem_cache_size() const
    {
        return static_cast<cl_ulong>(_get_numeric_data(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(cl_ulong)));
    }

    WUR UNUSED cl_device_mem_cache_type get_device_mem_cache_type() const
    {
        return static_cast<cl_device_mem_cache_type>(
            _get_numeric_data(
                CL_DEVICE_GLOBAL_MEM_CACHE_TYPE,
                sizeof(cl_device_mem_cache_type)
            )
        );
    }

    WUR UNUSED cl_uint get_device_global_mem_cacheling_size() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, sizeof(cl_uint)));
    }

    WUR UNUSED cl_ulong get_device_global_mem_size() const
    {
        return static_cast<cl_ulong>(_get_numeric_data(CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong)));
    }

    WUR UNUSED size_t get_device_global_variable_preferred_total_size() const
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_GLOBAL_VARIABLE_PREFERRED_TOTAL_SIZE, sizeof(size_t)));
    }

    WUR UNUSED std::string get_device_il_version() const
    {
        return _get_string_data(CL_DEVICE_IL_VERSION);
    }

    WUR UNUSED size_t get_image2d_max_height() const
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_IMAGE2D_MAX_HEIGHT, sizeof(size_t)));
    }

    WUR UNUSED size_t get_image2d_max_width() const
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_IMAGE2D_MAX_WIDTH, sizeof(size_t)));
    }

    WUR UNUSED size_t get_image3d_max_depth() const
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_IMAGE3D_MAX_DEPTH, sizeof(size_t)));
    }

    WUR UNUSED size_t get_image3d_max_height() const
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_IMAGE3D_MAX_HEIGHT, sizeof(size_t)));
    }

    WUR UNUSED size_t get_image3d_max_width() const
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_IMAGE3D_MAX_WIDTH, sizeof(size_t)));
    }

    WUR UNUSED cl_uint get_image_base_address_alignment() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT, sizeof(cl_uint)));
    }

    WUR UNUSED size_t get_device_max_array_size() const
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_IMAGE_MAX_ARRAY_SIZE, sizeof(size_t)));
    }

    WUR UNUSED size_t get_device_image_max_size() const
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_IMAGE_MAX_BUFFER_SIZE, sizeof(size_t)));
    }

    WUR UNUSED cl_uint get_device_image_pitch_alignment() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_IMAGE_PITCH_ALIGNMENT, sizeof(cl_uint)));
    }

    WUR UNUSED cl_bool is_image_supported() const
    {
        return static_cast<cl_bool>(_get_numeric_data(CL_DEVICE_IMAGE_SUPPORT, sizeof(cl_bool)));
    }

    WUR UNUSED cl_bool is_linker_available() const
    {
        return static_cast<cl_bool>(_get_numeric_data(CL_DEVICE_LINKER_AVAILABLE, sizeof(cl_bool)));
    }

    WUR UNUSED cl_ulong get_device_local_mem_size() const
    {
        return static_cast<cl_ulong>(_get_numeric_data(CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong)));
    }

    WUR UNUSED cl_device_local_mem_type get_device_local_mem_type() const
    {
        return static_cast<cl_device_local_mem_type>(
            _get_numeric_data(
                CL_DEVICE_LOCAL_MEM_TYPE,
                sizeof(cl_device_local_mem_type)
            )
        );
    }

    WUR UNUSED cl_uint get_device_max_clock_frequency() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint)));
    }

    WUR UNUSED cl_uint get_device_max_compute_units() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint)));
    }

    WUR UNUSED cl_uint get_device_max_constant_args() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_CONSTANT_ARGS, sizeof(cl_uint)));
    }

    WUR UNUSED cl_ulong get_device_max_constant_buffer_size() const
    {
        return static_cast<cl_ulong>(_get_numeric_data(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(cl_ulong)));
    }

    WUR UNUSED size_t get_device_max_global_variable_size() const
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_MAX_GLOBAL_VARIABLE_SIZE, sizeof(size_t)));
    }

    WUR UNUSED cl_ulong get_device_max_mem_alloc_size() const
    {
        return static_cast<cl_ulong>(_get_numeric_data(CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong)));
    }

    WUR UNUSED cl_uint get_device_max_num_sub_groups() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_NUM_SUB_GROUPS, sizeof(cl_uint)));
    }

    WUR UNUSED cl_uint get_device_max_on_events() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_ON_DEVICE_EVENTS, sizeof(cl_uint)));
    }

    WUR UNUSED cl_uint get_device_max_on_device_queqes() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_ON_DEVICE_QUEUES, sizeof(cl_uint)));
    }

    WUR UNUSED size_t get_device_max_parameter_size() const
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_MAX_PARAMETER_SIZE, sizeof(size_t)));
    }

    WUR UNUSED cl_uint get_device_max_pipe_args() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_PIPE_ARGS, sizeof(cl_uint)));
    }

    WUR UNUSED cl_uint get_device_max_read_image_args() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_READ_IMAGE_ARGS, sizeof(cl_uint)));
    }

    WUR UNUSED cl_uint get_device_max_read_write_image_args() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_READ_WRITE_IMAGE_ARGS, sizeof(cl_uint)));
    }

    WUR UNUSED cl_uint get_device_max_samplers() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_SAMPLERS, sizeof(cl_uint)));
    }

    WUR UNUSED size_t get_device_max_work_group_size() const
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t)));
    }

    WUR UNUSED cl_uint get_device_max_work_item_dimensions() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint)));
    }

    WUR UNUSED std::vector<size_t> get_device_max_work_item_sizes() const
    {
        cl_int              error;
        cl_uint             dims = get_device_max_work_item_dimensions();
        std::vector<size_t> ret(dims);

        error = clGetDeviceInfo(
                device_id,
                CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
                sizeof(size_t) * dims,
                ret.data(),
                nullptr
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);
        return ret;
    }

    WUR UNUSED cl_uint get_device_max_write_image_args() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_WRITE_IMAGE_ARGS, sizeof(cl_uint)));
    }

    WUR UNUSED cl_uint get_device_mem_base_addr_align() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MEM_BASE_ADDR_ALIGN, sizeof(cl_uint)));
    }

    WUR UNUSED std::string get_device_name() const
    {
        return _get_string_data(CL_DEVICE_NAME);
    }

    WUR UNUSED cl_uint get_device_native_vector_width_char() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR, sizeof(cl_uint)));
    }

    WUR UNUSED cl_uint get_device_native_vector_width_short() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT, sizeof(cl_uint)));
    }

    WUR UNUSED cl_uint get_device_native_vector_width_int() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_NATIVE_VECTOR_WIDTH_INT, sizeof(cl_uint)));
    }

    WUR UNUSED cl_uint get_device_native_vector_width_long() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG, sizeof(cl_uint)));
    }

    WUR UNUSED cl_uint get_device_native_vector_width_float() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT, sizeof(cl_uint)));
    }

    WUR UNUSED cl_uint get_device_native_vector_width_double() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE, sizeof(cl_uint)));
    }

    WUR UNUSED cl_uint get_device_native_vector_width_half() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF, sizeof(cl_uint)));
    }

    WUR UNUSED std::string get_device_opencl_c_version() const
    {
        return _get_string_data(CL_DEVICE_OPENCL_C_VERSION);
    }

    WUR UNUSED cl_device_id get_device_parent_device() const
    {
        return _get_struct_data<cl_device_id>(CL_DEVICE_PARENT_DEVICE);
    }

    WUR UNUSED cl_device_affinity_domain get_device_addinity_domain() const
    {
        return static_cast<cl_device_affinity_domain>(
            _get_numeric_data(
                CL_DEVICE_PARTITION_AFFINITY_DOMAIN,
                sizeof(cl_device_affinity_domain)
            )
        );
    }

    WUR UNUSED cl_uint get_device_partition_max_sub_devices() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_PARTITION_MAX_SUB_DEVICES, sizeof(cl_uint)));
    }

    WUR UNUSED std::vector<cl_device_partition_property> get_device_partition_property() const
    {
        cl_int      error;
        std::vector<cl_device_partition_property> ret(3);

        error = clGetDeviceInfo(
                device_id,
                CL_DEVICE_PARTITION_PROPERTIES,
                sizeof(cl_device_partition_property) * 3,
                ret.data(),
                nullptr
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);

        return ret;
    }

    WUR UNUSED std::vector<cl_device_partition_property> get_device_partition_type() const
    {
        cl_int      error;
        std::vector<cl_device_partition_property> ret(5);

        error = clGetDeviceInfo(
                device_id,
                CL_DEVICE_PARTITION_TYPE,
                sizeof(cl_device_partition_property) * 5,
                ret.data(),
                nullptr
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);

        return ret;
    }



private:
    CLdevice()
        : device_id()
    {}

    void _set_device_id(cl_device_id id)
    {
        device_id = id;
    }

    WUR std::string _get_string_data(cl_device_info type) const
    {
        cl_int      error;
        size_t      info_size;
        std::string info;

        error = clGetDeviceInfo(
            device_id,
            type,
            0,
            nullptr,
            &info_size
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);

        info.resize(info_size - 1);
        error = clGetDeviceInfo(
            device_id,
            type,
            info_size,
            info.data(),
            nullptr
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);

        return info;
    }

    template <class struct_T>
    WUR struct_T _get_struct_data(cl_device_info type) const
    {
        cl_int      error;
        struct_T    info;

        error = clGetDeviceInfo(
                device_id,
                type,
                sizeof(struct_T),
                &info,
                nullptr
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);

        return info;
    }

    WUR unsigned long long _get_numeric_data(cl_device_info type, size_t value_size) const
    {
        cl_int              error;
        unsigned long long  info;

        error = clGetDeviceInfo(
                device_id,
                type,
                value_size,
                &info,
                nullptr
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);

        return info;
    }

    cl_device_id    device_id;

    friend class CLplatform;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_DEVICE_HPP */
