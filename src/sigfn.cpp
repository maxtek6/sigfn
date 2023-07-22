#include "sigfn.hpp"

#include <sstream>
#include <unordered_map>

template <class F, class... Arg>
static void sigfn_wrapped_call(F &&function, Arg... args)
{
    std::pair<bool, std::string> result;
    int status;

    status = function(args...);

    if (status == SIGFN_INVALID_SIGNUM)
    {
        result.second = "invalid signal";
    }
    else if (status == SIGFN_INVALID_HANDLER)
    {
        result.second = "invalid handler";
    }
    else
    {
        result.first = true;
    }

    if (!result.first)
    {
        throw std::runtime_error(result.second);
    }
}

static std::unordered_map<int, sigfn::handler_function> handler_map;

static void handler(int signum, void *userdata);

void sigfn::handle(int signum, const sigfn::handler_function &handler_function)
{
    handler_map.emplace(signum, handler_function);
}

void sigfn::handle(int signum, sigfn::handler_function &&handler_function)
{
    sigfn_wrapped_call(sigfn_handle, signum, handler, &handler_map);
    handler_map.emplace(signum, std::move(handler_function));
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