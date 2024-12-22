/*
 * Copyright 2024 Maxtek Consulting
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

#ifndef __FUNCTION_NAME__
#ifdef WIN32 // WINDOWS
#define __FUNCTION_NAME__ __FUNCTION__
#else //*NIX
#define __FUNCTION_NAME__ __func__
#endif
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
}

void echo_signum(int signum, void *userdata)
{
    *(int *)userdata = signum;
}