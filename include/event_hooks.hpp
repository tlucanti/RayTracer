
#ifndef EVENT_HOOKS_HPP
# define EVENT_HOOKS_HPP

#include <common.hpp>

RTX_NAMESPACE_BEGIN

namespace hooks
{
    void keypress_hook(int keycode, void *);
    void keyrelease_hook(int keycode, void *);
    void mouse_hook(int x, int y, void *);
    void mousepress_hook(int button, int x, int y, void *);
    void mouserelease_hook(int button, void *);
    void framehook(void *);
} /* hooks */

RTX_NAMESPACE_END

#endif /* EVENT_HOOKS_HPP */
