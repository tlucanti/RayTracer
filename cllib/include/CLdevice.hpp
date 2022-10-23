
#ifndef CLLIB_DEVICE_HPP
# define CLLIB_DEVICE_HPP

# include "CLcommon.hpp"
# include "CLexception.hpp"

# include <vector>
# include <iostream>

CLLIB_NAMESPACE_BEGIN

class CLdevice : public __utils::__noncopyble<>
{
public:
    static const cl_device_type any_type            = CL_DEVICE_TYPE_DEFAULT;
    static const cl_device_type gpu_type            = CL_DEVICE_TYPE_GPU;
    static const cl_device_type cpu_type            = CL_DEVICE_TYPE_CPU;
    static const cl_device_type accelerator_type    = CL_DEVICE_TYPE_ACCELERATOR;
    static const cl_device_type all_types           = CL_DEVICE_TYPE_ALL;
#ifdef CL_DEVICE_TYPE_CUSTOM
        static const cl_device_type custom_type     = CL_DEVICE_TYPE_CUSTOM;
#endif /* CL_DEVICE_TYPE_CUSTOM */

# ifdef CL_DEVICE_ADDRESS_BITS
    WUR UNUSED cl_uint get_device_address_bits() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_ADDRESS_BITS, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_AVAILABLE
    WUR cl_bool is_avaliable() const THROW
    {
        return static_cast<cl_bool>(_get_numeric_data(CL_DEVICE_AVAILABLE, sizeof(cl_bool)));
    }
# endif

# ifdef CL_DEVICE_BUILT_IN_KERNELS
    WUR UNUSED std::string get_device_builtin_kernels() const THROW
    {
        return _get_string_data(CL_DEVICE_BUILT_IN_KERNELS);
    }
# endif

# ifdef CL_DEVICE_COMPILER_AVAILABLE
    WUR UNUSED cl_bool is_compiler_avaliable() const THROW
    {
        return static_cast<cl_bool>(_get_numeric_data(CL_DEVICE_COMPILER_AVAILABLE, sizeof(cl_bool)));
    }
# endif

# ifdef CL_DEVICE_DOUBLE_FP_CONFIG
    WUR UNUSED cl_device_fp_config get_device_fp_config() const THROW
    {
        return static_cast<cl_device_fp_config>(_get_numeric_data(CL_DEVICE_DOUBLE_FP_CONFIG, sizeof(cl_device_fp_config)));
    }
# endif

# ifdef CL_DEVICE_ENDIAN_LITTLE
    WUR UNUSED cl_bool is_little_endian() const THROW
    {
        return static_cast<cl_bool>(_get_numeric_data(CL_DEVICE_ENDIAN_LITTLE, sizeof(cl_bool)));
    }
# endif

# ifdef CL_DEVICE_ERROR_CORRECTION_SUPPORT
    WUR UNUSED cl_bool is_error_correction_supported() const THROW
    {
        return static_cast<cl_bool>(_get_numeric_data(CL_DEVICE_ERROR_CORRECTION_SUPPORT, sizeof(cl_bool)));
    }
# endif

# ifdef CL_DEVICE_EXECUTION_CAPABILITIES
    WUR UNUSED cl_device_exec_capabilities get_device_exec_capabilities() const THROW
    {
        return static_cast<cl_device_exec_capabilities>(
            _get_numeric_data(
                CL_DEVICE_EXECUTION_CAPABILITIES,
                sizeof(cl_device_exec_capabilities)
            )
        );
    }
# endif

# ifdef CL_DEVICE_EXTENSIONS
    WUR UNUSED std::string get_device_extensions() const THROW
    {
        return _get_string_data(CL_DEVICE_EXTENSIONS);
    }
# endif

# ifdef CL_DEVICE_GLOBAL_MEM_CACHE_SIZE
    WUR UNUSED cl_ulong get_device_global_mem_cache_size() const THROW
    {
        return static_cast<cl_ulong>(_get_numeric_data(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(cl_ulong)));
    }
# endif

# ifdef CL_DEVICE_GLOBAL_MEM_CACHE_TYPE
    WUR UNUSED cl_device_mem_cache_type get_device_mem_cache_type() const THROW
    {
        return static_cast<cl_device_mem_cache_type>(
            _get_numeric_data(
                CL_DEVICE_GLOBAL_MEM_CACHE_TYPE,
                sizeof(cl_device_mem_cache_type)
            )
        );
    }
# endif

# ifdef CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE
    WUR UNUSED cl_uint get_device_global_mem_cacheling_size() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_GLOBAL_MEM_SIZE
    WUR UNUSED cl_ulong get_device_global_mem_size() const THROW
    {
        return static_cast<cl_ulong>(_get_numeric_data(CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong)));
    }
# endif

# ifdef CL_DEVICE_GLOBAL_VARIABLE_PREFERRED_TOTAL_SIZE
    WUR UNUSED size_t get_device_global_variable_preferred_total_size() const THROW
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_GLOBAL_VARIABLE_PREFERRED_TOTAL_SIZE, sizeof(size_t)));
    }
# endif

# ifdef CL_DEVICE_IL_VERSION
    WUR UNUSED std::string get_device_il_version() const THROW
    {
        return _get_string_data(CL_DEVICE_IL_VERSION);
    }
# endif

# ifdef CL_DEVICE_IMAGE2D_MAX_HEIGHT
    WUR UNUSED size_t get_image2d_max_height() const THROW
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_IMAGE2D_MAX_HEIGHT, sizeof(size_t)));
    }
# endif

# ifdef CL_DEVICE_IMAGE2D_MAX_WIDTH
    WUR UNUSED size_t get_image2d_max_width() const THROW
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_IMAGE2D_MAX_WIDTH, sizeof(size_t)));
    }
# endif

# ifdef CL_DEVICE_IMAGE3D_MAX_DEPTH
    WUR UNUSED size_t get_image3d_max_depth() const THROW
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_IMAGE3D_MAX_DEPTH, sizeof(size_t)));
    }
# endif

# ifdef CL_DEVICE_IMAGE3D_MAX_HEIGHT
    WUR UNUSED size_t get_image3d_max_height() const THROW
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_IMAGE3D_MAX_HEIGHT, sizeof(size_t)));
    }
# endif

# ifdef CL_DEVICE_IMAGE3D_MAX_WIDTH
    WUR UNUSED size_t get_image3d_max_width() const THROW
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_IMAGE3D_MAX_WIDTH, sizeof(size_t)));
    }
# endif

# ifdef CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT
    WUR UNUSED cl_uint get_image_base_address_alignment() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_IMAGE_MAX_ARRAY_SIZE
    WUR UNUSED size_t get_device_max_array_size() const THROW
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_IMAGE_MAX_ARRAY_SIZE, sizeof(size_t)));
    }
# endif

# ifdef CL_DEVICE_IMAGE_MAX_BUFFER_SIZE
    WUR UNUSED size_t get_device_image_max_size() const THROW
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_IMAGE_MAX_BUFFER_SIZE, sizeof(size_t)));
    }
# endif

# ifdef CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    WUR UNUSED cl_uint get_device_image_pitch_alignment() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_IMAGE_PITCH_ALIGNMENT, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_IMAGE_SUPPORT
    WUR UNUSED cl_bool is_image_supported() const THROW
    {
        return static_cast<cl_bool>(_get_numeric_data(CL_DEVICE_IMAGE_SUPPORT, sizeof(cl_bool)));
    }
# endif

# ifdef CL_DEVICE_LINKER_AVAILABLE
    WUR UNUSED cl_bool is_linker_available() const THROW
    {
        return static_cast<cl_bool>(_get_numeric_data(CL_DEVICE_LINKER_AVAILABLE, sizeof(cl_bool)));
    }
# endif

# ifdef CL_DEVICE_LOCAL_MEM_SIZE
    WUR UNUSED cl_ulong get_device_local_mem_size() const THROW
    {
        return static_cast<cl_ulong>(_get_numeric_data(CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong)));
    }
# endif

# ifdef CL_DEVICE_LOCAL_MEM_TYPE
    WUR UNUSED cl_device_local_mem_type get_device_local_mem_type() const THROW
    {
        return static_cast<cl_device_local_mem_type>(
            _get_numeric_data(
                CL_DEVICE_LOCAL_MEM_TYPE,
                sizeof(cl_device_local_mem_type)
            )
        );
    }
# endif

# ifdef CL_DEVICE_MAX_CLOCK_FREQUENCY
    WUR UNUSED cl_uint get_device_max_clock_frequency() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_MAX_COMPUTE_UNITS
    WUR UNUSED cl_uint get_device_max_compute_units() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_MAX_CONSTANT_ARGS
    WUR UNUSED cl_uint get_device_max_constant_args() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_CONSTANT_ARGS, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE
    WUR UNUSED cl_ulong get_device_max_constant_buffer_size() const THROW
    {
        return static_cast<cl_ulong>(_get_numeric_data(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(cl_ulong)));
    }
# endif

# ifdef CL_DEVICE_MAX_GLOBAL_VARIABLE_SIZE
    WUR UNUSED size_t get_device_max_global_variable_size() const THROW
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_MAX_GLOBAL_VARIABLE_SIZE, sizeof(size_t)));
    }
# endif

# ifdef CL_DEVICE_MAX_MEM_ALLOC_SIZE
    WUR UNUSED cl_ulong get_device_max_mem_alloc_size() const THROW
    {
        return static_cast<cl_ulong>(_get_numeric_data(CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong)));
    }
# endif

# ifdef CL_DEVICE_MAX_NUM_SUB_GROUPS
    WUR UNUSED cl_uint get_device_max_num_sub_groups() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_NUM_SUB_GROUPS, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_MAX_ON_DEVICE_EVENTS
    WUR UNUSED cl_uint get_device_max_on_events() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_ON_DEVICE_EVENTS, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_MAX_ON_DEVICE_QUEUES
    WUR UNUSED cl_uint get_device_max_on_device_queqes() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_ON_DEVICE_QUEUES, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_MAX_PARAMETER_SIZE
    WUR UNUSED size_t get_device_max_parameter_size() const THROW
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_MAX_PARAMETER_SIZE, sizeof(size_t)));
    }
# endif

# ifdef CL_DEVICE_MAX_PIPE_ARGS
    WUR UNUSED cl_uint get_device_max_pipe_args() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_PIPE_ARGS, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_MAX_READ_IMAGE_ARGS
    WUR UNUSED cl_uint get_device_max_read_image_args() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_READ_IMAGE_ARGS, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_MAX_READ_WRITE_IMAGE_ARGS
    WUR UNUSED cl_uint get_device_max_read_write_image_args() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_READ_WRITE_IMAGE_ARGS, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_MAX_SAMPLERS
    WUR UNUSED cl_uint get_device_max_samplers() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_SAMPLERS, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_MAX_WORK_GROUP_SIZE
    WUR UNUSED size_t get_device_max_work_group_size() const THROW
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t)));
    }
# endif

# ifdef CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS
    WUR UNUSED cl_uint get_device_max_work_item_dimensions() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS
    WUR UNUSED std::vector<size_t> get_device_max_work_item_sizes() const THROW
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
# endif

# ifdef CL_DEVICE_MAX_WRITE_IMAGE_ARGS
    WUR UNUSED cl_uint get_device_max_write_image_args() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MAX_WRITE_IMAGE_ARGS, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_MEM_BASE_ADDR_ALIGN
    WUR UNUSED cl_uint get_device_mem_base_addr_align() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_MEM_BASE_ADDR_ALIGN, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_NAME
    WUR UNUSED std::string get_device_name() const THROW
    {
        return _get_string_data(CL_DEVICE_NAME);
    }
# endif

# ifdef CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR
    WUR UNUSED cl_uint get_device_native_vector_width_char() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT
    WUR UNUSED cl_uint get_device_native_vector_width_short() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_NATIVE_VECTOR_WIDTH_INT
    WUR UNUSED cl_uint get_device_native_vector_width_int() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_NATIVE_VECTOR_WIDTH_INT, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG
    WUR UNUSED cl_uint get_device_native_vector_width_long() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT
    WUR UNUSED cl_uint get_device_native_vector_width_float() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE
    WUR UNUSED cl_uint get_device_native_vector_width_double() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF
    WUR UNUSED cl_uint get_device_native_vector_width_half() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_OPENCL_C_VERSION
    WUR UNUSED std::string get_device_opencl_c_version() const THROW
    {
        return _get_string_data(CL_DEVICE_OPENCL_C_VERSION);
    }
# endif

# ifdef CL_DEVICE_PARENT_DEVICE
    WUR UNUSED cl_device_id get_device_parent_device() const THROW
    {
        return _get_struct_data<cl_device_id>(CL_DEVICE_PARENT_DEVICE);
    }
# endif

# ifdef CL_DEVICE_PARTITION_AFFINITY_DOMAIN
    WUR UNUSED cl_device_affinity_domain get_device_addinity_domain() const THROW
    {
        return static_cast<cl_device_affinity_domain>(
            _get_numeric_data(
                CL_DEVICE_PARTITION_AFFINITY_DOMAIN,
                sizeof(cl_device_affinity_domain)
            )
        );
    }
# endif

# ifdef CL_DEVICE_PARTITION_MAX_SUB_DEVICES
    WUR UNUSED cl_uint get_device_partition_max_sub_devices() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_PARTITION_MAX_SUB_DEVICES, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_PARTITION_PROPERTIES
    WUR UNUSED std::vector<cl_device_partition_property> get_device_partition_property() const THROW
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
# endif

# ifdef CL_DEVICE_PARTITION_TYPE
    WUR UNUSED std::vector<cl_device_partition_property> get_device_partition_type() const THROW
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
# endif

# ifdef CL_DEVICE_PIPE_MAX_ACTIVE_RESERVATIONS
    WUR UNUSED cl_uint get_device_max_active_reservations() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_PIPE_MAX_ACTIVE_RESERVATIONS, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_PIPE_MAX_PACKET_SIZE
    WUR UNUSED cl_uint get_device_pipe_max_packet_size() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_PIPE_MAX_PACKET_SIZE, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_PLATFORM
    WUR UNUSED cl_platform_id get_device_platform() const THROW
    {
        return _get_struct_data<cl_platform_id>(CL_DEVICE_PLATFORM);
    }
# endif

# ifdef CL_DEVICE_PREFERRED_GLOBAL_ATOMIC_ALIGNMENT
    WUR UNUSED cl_uint get_device_preferred_global_atomic_alignment() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_PREFERRED_GLOBAL_ATOMIC_ALIGNMENT, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_PREFERRED_INTEROP_USER_SYNC
    WUR UNUSED cl_bool get_device_preferred_interop_user_sync() const THROW
    {
        return static_cast<cl_bool>(_get_numeric_data(CL_DEVICE_PREFERRED_INTEROP_USER_SYNC, sizeof(cl_bool)));
    }
# endif

# ifdef CL_DEVICE_PREFERRED_LOCAL_ATOMIC_ALIGNMENT
    WUR UNUSED cl_uint get_device_preferred_local_atomic_alignment() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_PREFERRED_LOCAL_ATOMIC_ALIGNMENT, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_PREFERRED_PLATFORM_ATOMIC_ALIGNMENT
    WUR UNUSED cl_uint get_device_preferred_platform_atomic_alignment() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_PREFERRED_PLATFORM_ATOMIC_ALIGNMENT, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR
    WUR UNUSED cl_uint get_device_preferred_vector_width_char() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT
    WUR UNUSED cl_uint get_device_preferred_vector_width_short() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT
    WUR UNUSED cl_uint get_device_preferred_vector_width_int() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG
    WUR UNUSED cl_uint get_device_preferred_vector_width_long() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT
    WUR UNUSED cl_uint get_device_preferred_vector_width_float() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE
    WUR UNUSED cl_uint get_device_preferred_vector_width_double() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF
    WUR UNUSED cl_uint get_device_preferred_vector_width_half() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_PRINTF_BUFFER_SIZE
    WUR UNUSED size_t get_device_printf_buffer_size() const THROW
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_PRINTF_BUFFER_SIZE, sizeof(size_t)));
    }
# endif

# ifdef CL_DEVICE_PROFILE
    WUR UNUSED std::string get_device_profile() const THROW
    {
        return _get_string_data(CL_DEVICE_PROFILE);
    }
# endif

# ifdef CL_DEVICE_PROFILING_TIMER_RESOLUTION
    WUR UNUSED size_t get_device_profiling_timer_resolution() const THROW
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_PROFILING_TIMER_RESOLUTION, sizeof(size_t)));
    }
# endif

# ifdef CL_DEVICE_QUEUE_ON_DEVICE_MAX_SIZE
    WUR UNUSED size_t get_device_queue_on_device_max_size() const THROW
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_QUEUE_ON_DEVICE_MAX_SIZE, sizeof(size_t)));
    }
# endif

# ifdef CL_DEVICE_QUEUE_ON_DEVICE_PREFERRED_SIZE
    WUR UNUSED size_t get_device_queue_on_device_preferred_size() const THROW
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_QUEUE_ON_DEVICE_PREFERRED_SIZE, sizeof(size_t)));
    }
# endif

# ifdef CL_DEVICE_QUEUE_ON_DEVICE_PROPERTIES
    WUR UNUSED cl_command_queue_properties get_device_queue_on_device_properties() const THROW
    {
        return static_cast<cl_command_queue_properties>(
            _get_numeric_data(
                CL_DEVICE_QUEUE_ON_DEVICE_PROPERTIES,
                sizeof(cl_command_queue_properties)
            )
        );
    }
# endif

# ifdef CL_DEVICE_QUEUE_ON_HOST_PROPERTIES
    WUR UNUSED cl_command_queue_properties get_device_queue_on_host_properties() const THROW
    {
        return static_cast<cl_command_queue_properties>(
            _get_numeric_data(
                CL_DEVICE_QUEUE_ON_HOST_PROPERTIES,
                sizeof(cl_command_queue_properties)
            )
        );
    }
# endif

# ifdef CL_DEVICE_REFERENCE_COUNT
    WUR UNUSED size_t get_device_reference_count() const THROW
    {
        return static_cast<size_t>(_get_numeric_data(CL_DEVICE_REFERENCE_COUNT, sizeof(size_t)));
    }
# endif

# ifdef CL_DEVICE_SINGLE_FP_CONFIG
    WUR UNUSED cl_device_fp_config get_device_single_fp_config() const THROW
    {
        return static_cast<cl_device_fp_config>(_get_numeric_data(CL_DEVICE_SINGLE_FP_CONFIG, sizeof(cl_device_fp_config)));
    }
# endif

# ifdef CL_DEVICE_SPIR_VERSIONS
    WUR UNUSED std::string get_device_spir_versions() const THROW
    {
        return _get_string_data(CL_DEVICE_SPIR_VERSIONS);
    }
# endif

# ifdef CL_DEVICE_SUBGROUP_INDEPENDENT_FORWARD_PROGRESS
    WUR UNUSED cl_bool get_device_reference_count() const THROW
    {
        return static_cast<cl_bool>(_get_numeric_data(CL_DEVICE_SUBGROUP_INDEPENDENT_FORWARD_PROGRESS, sizeof(cl_bool)));
    }
# endif

# ifdef CL_DEVICE_SVM_CAPABILITIES
    WUR UNUSED cl_device_svm_capabilities get_device_svm_capabilities() const THROW
    {
        return static_cast<cl_device_svm_capabilities>
        (_get_numeric_data(
                CL_DEVICE_SVM_CAPABILITIES,
                sizeof(cl_device_svm_capabilities)
                )
                );
    }
# endif

# ifdef CL_DEVICE_TERMINATE_CAPABILITY_KHR
    WUR UNUSED cl_device_terminate_capability_khr() const THROW
    {
        return static_cast<cl_device_terminate_capability_khr>(
                _get_numeric_data(
                        CL_DEVICE_TERMINATE_CAPABILITY_KHR,
                        sizeof(cl_device_terminate_capability_khr)
                        )
                        );
    }
# endif

# ifdef CL_DEVICE_TYPE
    WUR UNUSED cl_device_type get_device_type() const THROW
    {
        return static_cast<cl_device_type>(_get_numeric_data(CL_DEVICE_TYPE, sizeof(cl_device_type)));
    }
# endif

# ifdef CL_DEVICE_VENDOR
    WUR UNUSED std::string get_device_vendor() const THROW
    {
        return _get_string_data(CL_DEVICE_VENDOR);
    }
# endif

# ifdef CL_DEVICE_VENDOR_ID
    WUR UNUSED cl_uint get_devive_vendor_id() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_DEVICE_VENDOR_ID, sizeof(cl_uint)));
    }
# endif

# ifdef CL_DEVICE_VERSION
    WUR UNUSED std::string get_device_version() const THROW
    {
        return _get_string_data(CL_DEVICE_VERSION);
    }
# endif

# ifdef CL_DRIVER_VERSION
    WUR UNUSED std::string get_driver_version() const THROW
    {
        return _get_string_data(CL_DRIVER_VERSION);
    }
# endif

    ~CLdevice() THROW
    {
        _destroy();
    }

    CLdevice() = delete;

    WUR const cl_device_id &__get_device() const NOEXCEPT
    {
        return device_id;
    }

    CLdevice &operator =(CLdevice &&mv) THROW
    {
        if (this == &mv)
            return *this;
        _destroy();
        device_id = std::move(mv.device_id);
        mv.device_id = nullptr;
        return *this;
    }

    CLdevice(CLdevice &&mv) THROW
        : device_id(nullptr)
    {
        *this = std::move(mv);
    }

private:

    void _destroy() THROW
    {
        cl_int  error;

        if (device_id != nullptr)
        {
            std::cout << "released device " << get_device_name() << std::endl;
            error = clReleaseDevice(device_id);
            if (error != CL_SUCCESS)
                throw CLexception(error);
        }
    }

    CLdevice(cl_device_id device_id) NOEXCEPT
            : device_id(device_id)
    {}

    WUR std::string _get_string_data(cl_device_info type) const THROW
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
    WUR struct_T _get_struct_data(cl_device_info type) const THROW
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

    WUR unsigned long long _get_numeric_data(cl_device_info type, size_t value_size) const THROW
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
