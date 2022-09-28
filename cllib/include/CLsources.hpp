
#ifndef CLLIB_SOURCES_HPP
# define CLLIB_SOURCES_HPP

# include "CLcommon.hpp"
# include "CLbuiltinprog.hpp"

# include <string>

CLLIB_NAMESPACE_BEGIN

namespace sources
{
    namespace __hidden_source
    {
        inline constexpr const char *__arithmetic_source =
        "__kernel void NAME(__global const TYPE *a, __global const TYPE *b, __global TYPE *c)"
        "{"
        "    int i = get_global_id(0);"
        "    c[i] = a[i] OPERATION b[i];"
        "}";

        inline constexpr const char *__arithmetic_isource =
        "__kernel void NAME(__global TYPE *a, __global const TYPE *b)"
        "{"
        "    int i = get_global_id(0);"
        "    a[i] OPERATION= b[i];"
        "}";
    }

    // add
    inline CLbuiltinprog addf(3, "addf", __utils::__replacer(__hidden_source::__arithmetic_source)
        .replace("NAME", "addf")
        .replace("TYPE", "float")
        .replace("OPERATION", "+")
        .str());

    // iadd
    inline CLbuiltinprog iaddf(2, "iadd",__utils::__replacer(__hidden_source::__arithmetic_isource)
        .replace("NAME", "iaddf")
        .replace("TYPE", "float")
        .replace("OPERATION", "+")
        .str());

    // sub
    inline CLbuiltinprog subf(3, "subf",__utils::__replacer(__hidden_source::__arithmetic_source)
        .replace("NAME", "subf")
        .replace("TYPE", "float")
        .replace("OPERATION", "-")
        .str());
    // isub
    inline CLbuiltinprog isubf(2, "isubf",__utils::__replacer(__hidden_source::__arithmetic_isource)
        .replace("NAME", "isubf")
        .replace("TYPE", "float")
        .replace("OPERATION", "-")
        .str());

    // mul
    inline CLbuiltinprog mulf(3, "mulf", __utils::__replacer(__hidden_source::__arithmetic_source)
        .replace("NAME", "mulf")
        .replace("TYPE", "float")
        .replace("OPERATION", "*")
        .str());

    // imul
    inline CLbuiltinprog imulf(2, "imulf", __utils::__replacer(__hidden_source::__arithmetic_isource)
        .replace("NAME", "imulf")
        .replace("TYPE", "float")
        .replace("OPERATION", "*")
        .str());

    // div
    inline CLbuiltinprog divf(3, "divf", __utils::__replacer(__hidden_source::__arithmetic_source)
        .replace("NAME", "divf")
        .replace("TYPE", "float")
        .replace("OPERATION", "/")
        .str());

    // idiv
    inline CLbuiltinprog idivf(2, "idivf", __utils::__replacer(__hidden_source::__arithmetic_isource)
        .replace("NAME", "idivf")
        .replace("TYPE", "float")
        .replace("OPERATION", "/")
        .str());

}

CLLIB_NAMESPACE_END

#endif /* CLLIB_SOURCES_HPP */
