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

#ifndef SIGFN_HPP
#define SIGFN_HPP

/**
 * @file sigfn.hpp
 * @brief SigFn C++ interface
 * @author John R. Patek Sr.
 */

#include <csignal>
#include <algorithm>
#include <chrono>
#include <functional>
#include <initializer_list>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <thread>

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

namespace sigfn
{
    /**
     * @brief signal handler function object type
     *
     * @param signum signal number
     */
    typedef std::function<void(int)> handler_function;

    /**
     * @brief attach handler to specific signal using copy semantics
     *
     * @param signum signal to be handled
     * @param handler_function function object associated with this signal
     */
    DLL_EXPORT void handle(int signum, const handler_function &handler_function);

    /**
     * @brief attach handler to specific signal using move semantics
     *
     * @param signum signal to be handled
     * @param handler_function function object associated with this signal
     */
    DLL_EXPORT void handle(int signum, handler_function &&handler_function);

    /**
     * @brief ignore a specific signal
     *
     * @param signum signal to be ignored
     */
    DLL_EXPORT void ignore(int signum);

    /**
     * @brief reset a specific signal to its default behavior
     *
     * @param signum signal to be reset
     */
    DLL_EXPORT void reset(int signum);

    DLL_EXPORT int wait(std::initializer_list<int> signums);

    DLL_EXPORT std::optional<int> wait_for(std::initializer_list<int> signums, const std::chrono::system_clock::duration &timeout);

    DLL_EXPORT std::optional<int> wait_until(std::initializer_list<int> signums, const std::chrono::system_clock::time_point &deadline);
}

#endif