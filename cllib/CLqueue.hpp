
#ifndef CLLIB_QUEUE_HPP
# define CLLIB_QUEUE_HPP

# include "defs.hpp"
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
        queue()
    {
        cl_int  error;

        queue = clCreateCommandQueue(context.__get_context(), device.__get_device(), 0, &error);
//        queue = clCreateCommandQueueWithProperties(context.__get_context(), device.__get_device(), properties, &error);
        if (error)
            throw CLexception(error);
    }

    WUR const cl_command_queue &__get_queue() const
    {
        return queue;
    }

    ~CLqueue() THROW
    {
        cl_int  error;

        error = clReleaseCommandQueue(queue);
        if (error != CL_SUCCESS)
            throw CLexception(error);
    }

    CLqueue()=delete;

private:
    cl_command_queue    queue;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_QUEUE_HPP */
