
#ifndef CLLIB_SOURCES_HPP
# define CLLIB_SOURCES_HPP

# include "defs.hpp"
# include "CLbuiltinprog.hpp"

# include <string>

CLLIB_NAMESPACE_BEGIN

namespace sources
{
    namespace __hidden_source
    {
        const char *__arithmetic_source =
        "__kernel void NAME(__global const TYPE *a, __global const TYPE *b, __global TYPE *c)"
        "{"
        "    int i = get_global_id(0);"
        "    c[i] = a[i] OPERATION b[i];"
        "}";

        const char *__arithmetic_isource =
        "__kernel void NAME(__global TYPE *a, __global const TYPE *b)"
        "{"
        "    int i = get_global_id(0);"
        "    a[i] OPERATION= b[i];"
        "}";
    }

    // add
    CLbuiltinprog addf(3, __utils::__replacer(__hidden_source::__arithmetic_source)
        .replace("NAME", "addf")
        .replace("TYPE", "float")
        .replace("OPERATION", "+")
        .str());

    // iadd
    CLbuiltinprog iaddf(2, __utils::__replacer(__hidden_source::__arithmetic_isource)
        .replace("NAME", "iaddf")
        .replace("TYPE", "float")
        .replace("OPERATION", "+")
        .str());

    // sub
    CLbuiltinprog subf(3, __utils::__replacer(__hidden_source::__arithmetic_source)
        .replace("NAME", "subf")
        .replace("TYPE", "float")
        .replace("OPERATION", "-")
        .str());
    // isub
    CLbuiltinprog isubf(2, __utils::__replacer(__hidden_source::__arithmetic_isource)
        .replace("NAME", "isubf")
        .replace("TYPE", "float")
        .replace("OPERATION", "-")
        .str());

    // mul
    CLbuiltinprog mulf(3, __utils::__replacer(__hidden_source::__arithmetic_source)
        .replace("NAME", "mulf")
        .replace("TYPE", "float")
        .replace("OPERATION", "*")
        .str());

    // imul
    CLbuiltinprog imulf(2, __utils::__replacer(__hidden_source::__arithmetic_isource)
        .replace("NAME", "imulf")
        .replace("TYPE", "float")
        .replace("OPERATION", "*")
        .str());

    // div
    CLbuiltinprog divf(3, __utils::__replacer(__hidden_source::__arithmetic_source)
        .replace("NAME", "divf")
        .replace("TYPE", "float")
        .replace("OPERATION", "/")
        .str());

    // idiv
    CLbuiltinprog idivf(2, __utils::__replacer(__hidden_source::__arithmetic_isource)
        .replace("NAME", "idivf")
        .replace("TYPE", "float")
        .replace("OPERATION", "/")
        .str());

}

CLLIB_NAMESPACE_END

#endif /* CLLIB_SOURCES_HPP */
