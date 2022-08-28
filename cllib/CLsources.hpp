
#ifndef CLLIB_SOURCES_HPP
# define CLLIB_SOURCES_HPP

# include <string>

# include "defs.hpp"

CLLIB_NAMESPACE_BEGIN

namespace sources
{
    namespace __hidden_source
    {
        const char *__arithmetic_source =
        "__kernel void ${NAME}(__global const ${TYPE} *a, __global const ${TYPE} *b, __global ${TYPE} *c)"
        "{"
        "    int i = get_global_id(0);"
        "    c[i] = a[i] ${OPERATION} b[i];"
        "}";

        const char *__arithmetic_isource =
        "__kernel void ${NAME}(__global const ${TYPE} *a, __global const ${TYPE} *b"
        "{"
        "    int i = get_global_id(0);"
        "    a[i] ${OPERATION}= b[i];"
        "}";
    }

    // add
    std::string addf = __utils::__replacer(__hidden_source::__arithmetic_source)
        .replace("${NAME}", "addf")
        .replace("${TYPE}", "float")
        .replace("${OPERATION}", "+")
        .str();

    // iadd
    std::string iaddf = __utils::__replacer(__hidden_source::__arithmetic_isource)
        .replace("${NAME}", "iaddf")
        .replace("${TYPE}", "float")
        .replace("${OPERATION}", "+")
        .str();

    // sub
    std::string subf = __utils::__replacer(__hidden_source::__arithmetic_source)
        .replace("${NAME}", "subf")
        .replace("${TYPE}", "float")
        .replace("${OPERATION}", "-")
        .str();
    // isub
    std::string isubf = __utils::__replacer(__hidden_source::__arithmetic_isource)
        .replace("${NAME}", "isubf")
        .replace("${TYPE}", "float")
        .replace("${OPERATION}", "-")
        .str();

    // mul
    std::string mulf = __utils::__replacer(__hidden_source::__arithmetic_source)
        .replace("${NAME}", "mulf")
        .replace("${TYPE}", "float")
        .replace("${OPERATION}", "*")
        .str();

    // imul
    std::string imulf = __utils::__replacer(__hidden_source::__arithmetic_isource)
        .replace("${NAME}", "imulf")
        .replace("${TYPE}", "float")
        .replace("${OPERATION}", "*")
        .str();

    // div
    std::string divf = __utils::__replacer(__hidden_source::__arithmetic_source)
        .replace("${NAME}", "divf")
        .replace("${TYPE}", "float")
        .replace("${OPERATION}", "/")
        .str();

    // idiv
    std::string idivf = __utils::__replacer(__hidden_source::__arithmetic_isource)
        .replace("${NAME}", "idivf")
        .replace("${TYPE}", "float")
        .replace("${OPERATION}", "/")
        .str();

}

CLLIB_NAMESPACE_END

#endif /* CLLIB_SOURCES_HPP */
