#ifndef SIGFN_HPP
#define SIGFN_HPP

#include "sigfn.h"

#include <functional>

namespace sigfn
{
    typedef std::function<void(int)> handler_function;

    void handle(int signum, const handler_function &handler_function);

    void handle(int signum, handler_function &&handler_function);

    void ignore(int signum);

    void reset(int signum); 
}

#endif