/*
 * Copyright 2025 Maxtek Consulting
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "internal.hpp"

std::unordered_map<int, sigfn::handler_function> sigfn::internal::state::handler_map;
std::string sigfn::internal::state::error_message;

void sigfn::internal::state::hook(int signum, __sighandler_t callback)
{
    const __sighandler_t result = signal(signum, callback);
    if (result == SIG_ERR)
    {
        throw std::runtime_error(invalid_syscall);
    }
}

void sigfn::internal::state::callback(int signum)
{
    handler_map.at(signum)(signum);
}

void sigfn::internal::handle(int signum, sigfn_handler_func handler, void *userdata)
{
    sigfn::handler_function handler_function;
    if (handler != nullptr)
    {
        handler_function = [handler, userdata](int signum)
        {
            handler(signum, userdata);
        };
    }
    sigfn::handle(signum, handler_function);
}

std::chrono::system_clock::duration sigfn::internal::make_duration(const struct timeval *timeval)
{
    if (timeval == nullptr)
    {
        throw std::runtime_error(invalid_timeval);
    }
    return std::chrono::seconds(timeval->tv_sec) + std::chrono::microseconds(timeval->tv_usec);
}

std::chrono::system_clock::time_point sigfn::internal::make_time_point(const struct timeval *timeval)
{
    return std::chrono::system_clock::time_point(make_duration(timeval));
}

void sigfn::handle(int signum, const sigfn::handler_function &handler)
{
    if (!handler)
    {
        throw std::runtime_error(sigfn::internal::invalid_handler);
    }
    internal::state::hook(signum, internal::state::callback);
    internal::state::handler_map[signum] = handler;
}

void sigfn::handle(int signum, sigfn::handler_function &&handler)
{
    if (!handler)
    {
        throw std::runtime_error(sigfn::internal::invalid_handler);
    }
    internal::state::hook(signum, internal::state::callback);
    internal::state::handler_map[signum] = std::move(handler);
}

void sigfn::ignore(int signum)
{
    internal::state::hook(signum, SIG_IGN);
    static_cast<void>(sigfn::internal::state::handler_map.erase(signum));
}

void sigfn::reset(int signum)
{
    internal::state::hook(signum, SIG_DFL);
    static_cast<void>(sigfn::internal::state::handler_map.erase(signum));
}

int sigfn::wait(std::initializer_list<int> signums)
{
    int signum(-1);
    internal::wait(signums.begin(), signums.end(), signum);
    return signum;
}

std::optional<int> sigfn::wait_for(std::initializer_list<int> signums, const std::chrono::system_clock::duration &timeout)
{
    std::optional<int> result;
    int signum(-1);
    if (internal::wait_for(signums.begin(), signums.end(), signum, timeout))
    {
        result = signum;
    }
    return result;
}

std::optional<int> sigfn::wait_until(std::initializer_list<int> signums, const std::chrono::system_clock::time_point &deadline)
{
    std::optional<int> result;
    int signum(-1);
    if (internal::wait_until(signums.begin(), signums.end(), signum, deadline))
    {
        result = signum;
    }
    return result;
}

int sigfn_handle(int signum, sigfn_handler_func handler, void *userdata)
{
    return sigfn::internal::try_catch_return(sigfn::internal::handle, signum, handler, userdata);
}

int sigfn_ignore(int signum)
{
    return sigfn::internal::try_catch_return(sigfn::ignore, signum);
}

int sigfn_reset(int signum)
{
    return sigfn::internal::try_catch_return(sigfn::reset, signum);
}

int sigfn_wait(const int *signums, size_t count, int *received)
{
    std::vector<int> signums_vec(signums, signums + count);
    return sigfn::internal::try_catch_return(
        [&]()
        {
            int signum(-1);
            sigfn::internal::wait(signums_vec.begin(), signums_vec.end(), signum);
            if (received != nullptr)
            {
                *received = signum;
            }
        });
}

int sigfn_wait_for(const int *signums, size_t count, int *received, const struct timeval *timeout)
{
    bool finished(false);
    std::vector<int> signums_vec(signums, signums + count);
    int result = sigfn::internal::try_catch_return(
        [&]()
        {
            int signum(-1);
            finished = sigfn::internal::wait_for(signums_vec.begin(), signums_vec.end(), signum, sigfn::internal::make_duration(timeout));
            if (finished && received != nullptr)
            {
                *received = signum;
            }
        });
    if (result == 0 && !finished)
    {
        result = 1;
    }
    return result;
}

int sigfn_wait_until(const int *signums, size_t count, int *received, const struct timeval *deadline)
{
    bool finished(false);
    std::vector<int> signums_vec(signums, signums + count);
    int result = sigfn::internal::try_catch_return(
        [&]()
        {
            int signum(-1);
            finished = sigfn::internal::wait_until(signums_vec.begin(), signums_vec.end(), signum, sigfn::internal::make_time_point(deadline));
            if (finished && received != nullptr)
            {
                *received = signum;
            }
        });
    if (result == 0 && !finished)
    {
        result = 1;
    }
    return result;
}

const char *sigfn_error()
{
    const char *result(nullptr);
    if (!sigfn::internal::state::error_message.empty())
    {
        result = sigfn::internal::state::error_message.c_str();
    }
    return result;
}