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

#include <maxtest.hpp>
#include "internal.hpp"

#define PASS 0
#define FAIL 1

#define INVALID_SIGNUM -1
#define INVALID_HANDLER nullptr

#ifndef _WIN32 // WINDOWS
#include <unistd.h>
template <class Period, class Rep>
static void signal_from_child(int signum, const std::chrono::duration<Rep, Period> &duration)
{
    const pid_t pid = getpid();
    if (fork() == 0)
    {
        std::this_thread::sleep_for(duration);
        kill(pid, signum);
        _exit(0);
    }
}
#endif

static void echo_signum(int signum, void *userdata);

// GCOV_EXCL_START
MAXTEST_MAIN
{
    MAXTEST_TEST_CASE(sigfn_handle)
    {
        int result;
        int signum(SIGINT);
        int flag(INVALID_SIGNUM);
        result = ::sigfn_handle(signum, echo_signum, &flag);
        MAXTEST_ASSERT(result == PASS);
        raise(signum);
        MAXTEST_ASSERT(flag == signum);
    };

    MAXTEST_TEST_CASE(sigfn_ignore)
    {
        MAXTEST_ASSERT(::sigfn_ignore(INVALID_SIGNUM) == -1);
        MAXTEST_ASSERT(::sigfn_ignore(SIGINT) == 0);
    };

    MAXTEST_TEST_CASE(sigfn_reset)
    {
        MAXTEST_ASSERT(::sigfn_reset(INVALID_SIGNUM) == -1);
        MAXTEST_ASSERT(::sigfn_reset(SIGINT) == 0);
    };

    MAXTEST_TEST_CASE(sigfn_wait)
    {
#ifndef _WIN32 // WINDOWS
        const int signums[1] = {SIGINT};
        int signum(INVALID_SIGNUM);
        int result;
        // test empty sigset
        result = ::sigfn_wait(NULL, 0, &signum);
        MAXTEST_ASSERT(result == -1);
        MAXTEST_ASSERT(signum == INVALID_SIGNUM);
        // test invalid signum
        int zero(0);
        result = ::sigfn_wait(&signum, 1, &zero);
        MAXTEST_ASSERT(result == -1);
        MAXTEST_ASSERT(zero == 0);
        // expect success
        signal_from_child(SIGINT, std::chrono::milliseconds(20));
        result = ::sigfn_wait(&signums[0], 1, &signum);
        MAXTEST_ASSERT(result == 0);
        MAXTEST_ASSERT(signums[0] == signum);
        // expect success but do not store signum
        signal_from_child(SIGINT, std::chrono::milliseconds(20));
        result = ::sigfn_wait(&signums[0], 1, NULL);
        MAXTEST_ASSERT(result == 0);
#endif
    };

    MAXTEST_TEST_CASE(sigfn_wait_for)
    {
#ifndef _WIN32 // WINDOWS
        const int signums[1] = {SIGINT};
        int signum(INVALID_SIGNUM);
        int result;
        struct timeval timeout;
        // test invalid timeval
        result = ::sigfn_wait_for(&signums[0], 1, &signum, NULL);
        MAXTEST_ASSERT(result == -1);
        MAXTEST_ASSERT(signum == INVALID_SIGNUM);
        timeout.tv_sec = 0;
        timeout.tv_usec = 200000;
        // test empty sigset
        result = ::sigfn_wait_for(NULL, 0, &signum, &timeout);
        MAXTEST_ASSERT(result == -1);
        MAXTEST_ASSERT(signum == INVALID_SIGNUM);
        // expect success
        signal_from_child(SIGINT, std::chrono::milliseconds(20));
        result = ::sigfn_wait_for(&signums[0], 1, &signum, &timeout);
        MAXTEST_ASSERT(result == 0);
        MAXTEST_ASSERT(signums[0] == signum);
        // expect success but do not store signum
        signal_from_child(SIGINT, std::chrono::milliseconds(20));
        result = ::sigfn_wait_for(&signums[0], 1, NULL, &timeout);
        MAXTEST_ASSERT(result == 0);
        // expect timeout
        timeout.tv_sec = 0;
        timeout.tv_usec = 1;
        signum = INVALID_SIGNUM;
        result = ::sigfn_wait_for(&signums[0], 1, &signum, &timeout);
        MAXTEST_ASSERT(result == 1);
        MAXTEST_ASSERT(signum == INVALID_SIGNUM);
#endif
    };

    MAXTEST_TEST_CASE(sigfn_wait_until)
    {
#ifndef _WIN32 // WINDOWS
        const int signums[1] = {SIGINT};
        int signum(INVALID_SIGNUM);
        int result;
        struct timeval deadline;
        struct timeval now;
        // test invalid timeval
        result = ::sigfn_wait_until(&signums[0], 1, &signum, NULL);
        MAXTEST_ASSERT(result == -1);
        MAXTEST_ASSERT(signum == INVALID_SIGNUM);
        // expect success
        gettimeofday(&now, NULL);
        deadline.tv_sec = now.tv_sec + 1;
        deadline.tv_usec = now.tv_usec;
        // test empty sigset
        result = ::sigfn_wait_until(NULL, 0, &signum, &deadline);
        MAXTEST_ASSERT(result == -1);
        MAXTEST_ASSERT(signum == INVALID_SIGNUM);
        signal_from_child(SIGINT, std::chrono::milliseconds(20));
        result = ::sigfn_wait_until(&signums[0], 1, &signum, &deadline);
        MAXTEST_ASSERT(result == 0);
        MAXTEST_ASSERT(signums[0] == signum);
        // expect success but do not store signum
        signal_from_child(SIGINT, std::chrono::milliseconds(20));
        result = ::sigfn_wait_until(&signums[0], 1, NULL, &deadline);
        MAXTEST_ASSERT(result == 0);
        deadline.tv_sec = now.tv_sec;
        deadline.tv_usec = now.tv_usec;
        signum = INVALID_SIGNUM;
        // expect timeout
        result = ::sigfn_wait_until(&signums[0], 1, &signum, &deadline);
        MAXTEST_ASSERT(result == 1);
        MAXTEST_ASSERT(signum == INVALID_SIGNUM);
#endif
    };

    MAXTEST_TEST_CASE(sigfn_error)
    {
        int flag;
        std::string error;
        ::sigfn_handle(INVALID_SIGNUM, echo_signum, &flag);
        error = ::sigfn_error();
        MAXTEST_ASSERT(error == sigfn::internal::invalid_syscall);
        ::sigfn_handle(SIGINT, INVALID_HANDLER, &flag);
        error = ::sigfn_error();
        MAXTEST_ASSERT(error == sigfn::internal::invalid_handler);
        ::sigfn_handle(SIGINT, echo_signum, &flag);
        MAXTEST_ASSERT(::sigfn_error() == nullptr);
    };

    MAXTEST_TEST_CASE(sigfn::handle)
    {
        int flag(0);
        const sigfn::handler_function invalid_copy;
        const sigfn::handler_function valid_copy = [&](int signum)
        {
            flag = 1;
        };
        sigfn::handler_function invalid_move;
        sigfn::handler_function valid_move = [&](int signum)
        {
            flag = 2;
        };

        const std::function<void(int, const sigfn::handler_function &, const std::string &)> try_catch_copy(
            [](
                int signum,
                const sigfn::handler_function &handler,
                const std::string &expected_error)
            {
                std::string actual_error;
                try
                {
                    sigfn::handle(signum, handler);
                }
                catch (const std::exception &e)
                {
                    actual_error = e.what();
                }
                MAXTEST_ASSERT(expected_error == actual_error);
            });
        const std::function<void(int, sigfn::handler_function &&, const std::string &)> try_catch_move(
            [](
                int signum,
                sigfn::handler_function &&handler,
                const std::string &expected_error)
            {
                std::string actual_error;
                try
                {
                    sigfn::handle(signum, std::move(handler));
                }
                catch (const std::exception &e)
                {
                    actual_error = e.what();
                }
                MAXTEST_ASSERT(expected_error == actual_error);
            });
        try_catch_copy(INVALID_SIGNUM, valid_copy, sigfn::internal::invalid_syscall);
        try_catch_copy(SIGINT, invalid_copy, sigfn::internal::invalid_handler);
        try_catch_copy(SIGINT, valid_copy, "");
        raise(SIGINT);
        MAXTEST_ASSERT(flag == 1);
        try_catch_move(SIGINT, std::move(invalid_move), sigfn::internal::invalid_handler);
        try_catch_move(SIGINT, std::move(valid_move), "");
        raise(SIGINT);
        MAXTEST_ASSERT(flag == 2);
    };

    MAXTEST_TEST_CASE(sigfn::ignore)
    {
        const std::function<void(int, bool)> try_catch_assert(
            [](
                int signum,
                bool expect_error)
            {
                bool has_error;

                has_error = false;
                try
                {
                    sigfn::ignore(signum);
                }
                catch (const std::exception &e)
                {
                    has_error = (e.what() != nullptr);
                }
                MAXTEST_ASSERT(expect_error == has_error);
            });
        try_catch_assert(INVALID_SIGNUM, true);
        try_catch_assert(SIGINT, false);
    };

    MAXTEST_TEST_CASE(sigfn::reset)
    {
        const std::function<void(int, bool)> try_catch_assert(
            [](
                int signum,
                bool expect_error)
            {
                bool has_error;

                has_error = false;
                try
                {
                    sigfn::reset(signum);
                }
                catch (const std::exception &e)
                {
                    has_error = (e.what() != nullptr);
                }
                MAXTEST_ASSERT(expect_error == has_error);
            });
        try_catch_assert(INVALID_SIGNUM, true);
        try_catch_assert(SIGINT, false);
    };

    MAXTEST_TEST_CASE(sigfn::wait)
    {
#ifndef _WIN32 // WINDOWS
        const std::function<void(std::initializer_list<int>, bool)> try_catch_assert(
            [](
                std::initializer_list<int> signums,
                bool expect_error)
            {
                bool has_error;

                has_error = false;
                try
                {
                    sigfn::wait(signums);
                }
                catch (const std::exception &e)
                {
                    has_error = (e.what() != nullptr);
                }
                MAXTEST_ASSERT(expect_error == has_error);
            });
        try_catch_assert({}, true);
        try_catch_assert({INVALID_SIGNUM}, true);
        signal_from_child(SIGINT, std::chrono::milliseconds(20));
        try_catch_assert({SIGINT}, false);
#endif
    };

    MAXTEST_TEST_CASE(sigfn::wait_for)
    {
#ifndef _WIN32 // WINDOWS
        const std::function<void(int, bool, bool)> try_catch_assert(
            [](
                int signum,
                bool expect_error,
                bool expect_timeout)
            {
                bool has_error;
                std::optional<int> result;

                has_error = false;
                try
                {
                    result = sigfn::wait_for({signum}, std::chrono::milliseconds(200));
                }
                catch (const std::exception &e)
                {
                    has_error = (e.what() != nullptr);
                }
                MAXTEST_ASSERT(expect_error == has_error);
                if (!expect_error)
                {
                    MAXTEST_ASSERT(expect_timeout == !result.has_value());
                }
            });
        try_catch_assert(INVALID_SIGNUM, true, false);
        signal_from_child(SIGINT, std::chrono::milliseconds(20));
        try_catch_assert(SIGINT, false, false);
        try_catch_assert(SIGINT, false, true);
#endif
    };

    MAXTEST_TEST_CASE(sigfn::wait_until)
    {
#ifndef _WIN32 // WINDOWS
        const std::function<void(int, bool, bool)> try_catch_assert(
            [](
                int signum,
                bool expect_error,
                bool expect_timeout)
            {
                bool has_error;
                std::optional<int> result;

                has_error = false;
                try
                {
                    result = sigfn::wait_until({signum}, std::chrono::system_clock::now() + std::chrono::milliseconds(200));
                }
                catch (const std::exception &e)
                {
                    has_error = (e.what() != nullptr);
                }
                MAXTEST_ASSERT(expect_error == has_error);
                if (!expect_error)
                {
                    MAXTEST_ASSERT(expect_timeout == !result.has_value());
                }
            });
        try_catch_assert(INVALID_SIGNUM, true, false);
        signal_from_child(SIGINT, std::chrono::milliseconds(20));
        try_catch_assert(SIGINT, false, false);
        try_catch_assert(SIGINT, false, true);
#endif
    };
}

void echo_signum(int signum, void *userdata)
{
    *(int *)userdata = signum;
}