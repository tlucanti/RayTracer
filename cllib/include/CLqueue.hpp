
#ifndef CLLIB_QUEUE_HPP
# define CLLIB_QUEUE_HPP

# include "CLcommon.hpp"
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
    ) THROW :
        queue()
    {
        cl_int  error;

#ifdef __APPLE__
        (void)properties;
        queue = clCreateCommandQueue(context.__get_context(), device.__get_device(), 0, &error);
#else
        queue = clCreateCommandQueueWithProperties(context.__get_context(), device.__get_device(), properties, &error);
#endif /* __APPLE__ */
        if (error)
            throw CLexception(error);
    }

    void finish() const THROW
    {
        cl_int  error;

        error = clFinish(queue);
        if (error != CL_SUCCESS)
            throw CLexception(error);
    }

    void flush() const THROW
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
        : queue(nullptr)
    {}

    CLqueue &operator =(CLqueue &&mv) THROW
    {
        if (this == &mv)
            return *this;
        _destroy();
        queue = std::move(mv.queue);
        mv.queue = nullptr;
        return *this;
    }

    CLqueue(CLqueue &&mv) THROW
        : queue(nullptr)
    {
        *this = std::move(mv);
    }

private:
    void _destroy() THROW
    {
        cl_int  error;

        if (queue != nullptr)
        {
            error = clReleaseCommandQueue(queue);
            if (error != CL_SUCCESS)
                throw CLexception(error);
        }
    }

    cl_command_queue    queue;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_QUEUE_HPP */
