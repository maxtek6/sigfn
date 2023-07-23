#include "sigfn.hpp"

#include <unordered_map>

sigfn::exception::exception(int status)
{
    if (status == SIGFN_INVALID_SIGNUM)
    {
        _error_message = sigfn::INVALID_SIGNUM;
    }
    else if (status == SIGFN_INVALID_HANDLER)
    {
        _error_message = sigfn::INVALID_HANDLER;
    }
    else
    {
        _error_message = "";
    }
}

char const* sigfn::exception::what() const
{
    return _error_message.c_str();
}

template <class F, class... Arg>
static void sigfn_wrapped_call(F &&function, Arg... args)
{
    std::pair<bool, std::string> result;
    int status;

    status = function(args...);

    if (status != SIGFN_SUCCESS)
    {
        throw sigfn::exception(status);
    }
}

static std::unordered_map<int, sigfn::handler_function> handler_map;

static void handler(int signum, void *userdata);

void sigfn::handle(int signum, const sigfn::handler_function &handler_function)
{
    sigfn_wrapped_call(sigfn_handle, signum, handler, &handler_map);
    handler_map[signum] = handler_function;
}

void sigfn::handle(int signum, sigfn::handler_function &&handler_function)
{
    sigfn_wrapped_call(sigfn_handle, signum, handler, &handler_map);
    handler_map[signum] = std::move(handler_function);
}

void sigfn::ignore(int signum)
{
    sigfn_wrapped_call(sigfn_ignore, signum);
}

void sigfn::reset(int signum)
{
    sigfn_wrapped_call(sigfn_reset, signum);
}

void handler(int signum, void *userdata)
{
    std::unordered_map<int, sigfn::handler_function> *map;

    map = reinterpret_cast<std::unordered_map<int, sigfn::handler_function> *>(userdata);

    map->at(signum)(signum);
}