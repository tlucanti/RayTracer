
#ifndef CLLIB_QUEUE_HPP
# define CLLIB_QUEUE_HPP

# include "CLdefs.hpp"
# include "CLexception.hpp"
# include "CLcontext.hpp"
# include "CLdevice.hpp"

CLLIB_NAMESPACE_BEGIN

class CLqueue : public __utils::__noncopymovable<>
{
public:
    CLqueue(
        const CLcontext &context,
        const CLdevice &device,
        const cl_command_queue_properties *properties=nullptr
    ) :
        queue(), ref_count(new int(1))
    {
        cl_int  error;

#ifdef __APPLE__
        queue = clCreateCommandQueue(context.__get_context(), device.__get_device(), 0, &error);
#else
        queue = clCreateCommandQueueWithProperties(context.__get_context(), device.__get_device(), properties, &error);
#endif /* __APPLE__ */
        if (error)
            throw CLexception(error);
    }

    void finish() const
    {
        cl_int  error;

        error = clFinish(queue);
        if (error != CL_SUCCESS)
            throw CLexception(error);
    }

    void flush() const
    {
        cl_int  error;

        error = clFlush(queue);
        if (error != CL_SUCCESS)
            throw CLexception(error);
    }

    WUR const cl_command_queue &__get_queue() const NOEXCEPT
    {
        return queue;
    }

    ~CLqueue() THROW
    {
        _destroy();
    }

    CLqueue() NOEXCEPT
        : queue(nullptr), ref_count(nullptr)
    {}

    CLqueue &operator =(const CLqueue &cpy)
    {
        if (this == &cpy or cpy.ref_count == nullptr)
            return *this;
        _destroy();
        queue = cpy.queue;
        ref_count = cpy.ref_count;
        ++*ref_count;
        return *this;
    }

private:
    void _destroy()
    {
        cl_int  error;

        if (ref_count != nullptr and --*ref_count == 0)
        {
            error = clReleaseCommandQueue(queue);
            if (error != CL_SUCCESS)
                throw CLexception(error);
        }
    }

    cl_command_queue    queue;
    int                 *ref_count;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_QUEUE_HPP */
