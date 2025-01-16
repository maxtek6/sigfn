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

#ifndef INTERNAL_HPP
#define INTERNAL_HPP

#include <sigfn.h>
#include <sigfn.hpp>

#include <channels.hpp>

#ifdef _WIN32
typedef void (*__sighandler_t)(int);
#endif

namespace sigfn
{
    namespace internal
    {
        const std::string invalid_syscall = "sigfn: signal() failed";
        const std::string invalid_handler = "sigfn: invalid handler";
        const std::string empty_sigset = "sigfn: empty signum set";
        const std::string invalid_timeval = "sigfn: invalid timeval";

        struct state
        {
            static std::unordered_map<int, sigfn::handler_function> handler_map;
            static std::string error_message;
            static void hook(int signum, __sighandler_t callback);
            static void callback(int signum);
        };

        // adding because the C++ interface is not cooperating with the template
        void handle(int signum, sigfn_handler_func handler, void *userdata);

        template <class IteratorType>
        void handle_sigset(IteratorType begin, IteratorType end, channels::buffered_channel<int> &channel)
        {
            const std::size_t size = std::distance(begin, end);
            if (size > 0)
            {
                std::for_each(
                    begin,
                    end,
                    [&](int signum)
                    {
                        sigfn::handle(
                            signum,
                            [&](int value)
                            {
                                channel.write(value);
                            });
                    });
            }
            else
            {
                throw std::runtime_error(empty_sigset);
            }
        }

        template <class F, class... Args>
        int try_catch_return(F &&f, Args &&...args)
        {
            int result(0);
            try
            {
                f(std::forward<Args>(args)...);
                state::error_message.clear();
            }
            catch (const std::exception &e)
            {
                state::error_message = e.what();
                result = -1;
            }
            return result;
        }

        template <class IteratorType>
        void wait(IteratorType begin, IteratorType end, int &signum)
        {
            channels::buffered_channel<int> channel(std::max<std::size_t>(std::distance(begin, end), 1));
            handle_sigset(begin, end, channel);
            channel.read(signum);
        }

        template <class IteratorType>
        bool wait_for(IteratorType begin, IteratorType end, int &signum, const std::chrono::system_clock::duration &timeout)
        {
            channels::buffered_channel<int> channel(std::max<std::size_t>(std::distance(begin, end), 1));
            handle_sigset(begin, end, channel);
            return channel.read_for(signum, timeout) == channels::read_status::success;
        }

        template <class IteratorType>
        bool wait_until(IteratorType begin, IteratorType end, int &signum, const std::chrono::system_clock::time_point &deadline)
        {
            channels::buffered_channel<int> channel(std::max<std::size_t>(std::distance(begin, end), 1));
            handle_sigset(begin, end, channel);
            return channel.read_until(signum, deadline) == channels::read_status::success;
        }

        std::chrono::system_clock::duration make_duration(const struct timeval *timeval);

        std::chrono::system_clock::time_point make_time_point(const struct timeval *timeval);
    }
}

#endif