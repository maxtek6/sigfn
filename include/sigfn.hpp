/*
** Copyright 2024 Maxtek Consulting
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

#ifndef SIGFN_HPP
#define SIGFN_HPP

#include "sigfn.h"

#include <functional>
#include <string>
#include <unordered_map>

namespace sigfn
{
    /**
     * @brief signal handler function object type
     *
     * @param signum signal number
     */
    typedef std::function<void(int)> handler_function;

    const std::string INVALID_SIGNUM = "sigfn: invalid signal code";
    const std::string INVALID_HANDLER = "sigfn: invalid signal handler";

    /**
     * @brief sigfn exception
     *
     * @class exception allows sigfn error codes to be thrown as exceptions
     */
    class exception final : public std::exception
    {
    private:
        std::string _error_message;

    public:
        exception(int status);
        const char *what() const noexcept override;
    };

    /**
     * @brief attach handler to specific signal using copy semantics
     *
     * @param signum signal to be handled
     * @param handler_function function object associated with this signal
     */
    void handle(int signum, const handler_function &handler_function);

    /**
     * @brief attach handler to specific signal using move semantics
     *
     * @param signum signal to be handled
     * @param handler_function function object associated with this signal
     */
    void handle(int signum, handler_function &&handler_function);

    /**
     * @brief ignore a specific signal
     *
     * @param signum signal to be ignored
     */
    void ignore(int signum);

    /**
     * @brief reset a specific signal to its default behavior
     *
     * @param signum signal to be reset
     */
    void reset(int signum);
}

#endif