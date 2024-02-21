/*
** Copyright 2024 John R. Patek Sr.
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
*/

#include "sigfn.hpp"

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

char const* sigfn::exception::what() const noexcept
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