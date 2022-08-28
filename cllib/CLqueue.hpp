
#ifndef CLLIB_QUEUE_HPP
# define CLLIB_QUEUE_HPP

# include "defs.hpp"
# include "CLexception.hpp"
# include "CLcontext.hpp"
# include "CLdevice.hpp"

CLLIB_NAMESPACE_BEGIN

class CLqueue
{
public:
    CLqueue(
        const CLcontext &context,
        const CLdevice &device,
        cl_command_queue_properties properties=0
    ) :
        queue()
    {
        cl_int  error;

        queue = clCreateCommandQueue(context.get_context(), device.get_device_id(), properties, &error);
        if (error)
            throw CLexception(error);
    }

    WUR cl_command_queue get_queue() const
    {
        return queue;
    }

private:
    cl_command_queue    queue;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_QUEUE_HPP */
