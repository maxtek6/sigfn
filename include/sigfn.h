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

#ifndef SIGFN_H
#define SIGFN_H

/**
 * @file sigfn.h
 * @brief SigFn C interface
 * @author John R. Patek Sr.
 */

#include <stdint.h>
#include <stdlib.h>
#include <signal.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
    struct timeval
    {
        uint64_t tv_sec;
        uint64_t tv_usec;
    };
#else
#define DLL_EXPORT
#include <sys/time.h>
#endif

    /**
     * @brief signal handler function type
     *
     * @param signum signal number
     * @param userdata pointer to user defined data, can be NULL
     */
    typedef void (*sigfn_handler_func)(int signum, void *userdata);

    /**
     * @brief attach handler to specific signal
     *
     * @param signum signal to be handled
     * @param handler function associated with this signal
     * @param userdata optional user data passed to the function
     * @returns 0 on success, -1 on error
     */
    DLL_EXPORT int sigfn_handle(int signum, sigfn_handler_func handler, void *userdata);

    /**
     * @brief ignore a specific signal
     *
     * @param signum signal to ignore
     * @returns 0 on success, -1 on error
     */
    DLL_EXPORT int sigfn_ignore(int signum);

    /**
     * @brief reset a specific signal to its default behavior
     *
     * @param signum signal to reset
     * @returns 0 on success, -1 on error
     */
    DLL_EXPORT int sigfn_reset(int signum);

    /**
     * @brief wait for any of the specified signals
     *
     * @param signums array of signal numbers
     * @param count number of signals in the array
     * @param received signal number that was received, can be NULL
     * @returns 0 on success, -1 on error
     */
    DLL_EXPORT int sigfn_wait(const int *signums, size_t count, int *received);

    /**
     * @brief wait for any of the specified signals with a timeout
     *
     * @param signums array of signal numbers
     * @param count number of signals in the array
     * @param received signal number that was received, can be NULL
     * @param timeout maximum time to wait
     * @returns 0 on success, -1 on error, 1 if timed out
     */
    DLL_EXPORT int sigfn_wait_for(const int *signums, size_t count, int *received, const struct timeval *timeout);

    /**
     * @brief wait for any of the specified signals until a deadline
     *
     * @param signums array of signal numbers
     * @param count number of signals in the array
     * @param received signal number that was received, can be NULL
     * @param deadline time to stop waiting
     * @returns 0 on success, -1 on error, 1 if timed out
     */
    DLL_EXPORT int sigfn_wait_until(const int *signums, size_t count, int *received, const struct timeval *deadline);

    /**
     * @brief get the last error message
     *
     * @returns error message, NULL if the last operation was successful
     */
    DLL_EXPORT const char *sigfn_error();

#ifdef __cplusplus
}
#endif

#endif