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

#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

/**
 * @file threadpool.hpp
 * @brief Maxtek threadpool
 * @author Max Guerrero and John R. Patek Sr.
 */

#include <algorithm>
#include <functional>
#include <future>
#include <memory>
#include <queue>
#include <thread>
#include <typeinfo>
#include <vector>

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

namespace maxtek
{
    /**
     * @brief threadpool
     *
     * @class allows tasks to be submitted and executed asynchronously across multiple threads.
     */
    class DLL_EXPORT threadpool
    {
    public:

        /**
         * @brief constructs a new threadpool
         *
         * @param threads number of threads to use for constructing the threadpool
         * @exception std::runtime_error if threads is set to zero
         */
        threadpool(size_t threads = std::thread::hardware_concurrency());

        /**
         * @brief destroys threadpool after calling shutdown if necessary
        */
        ~threadpool();

        /**
         * @brief submits a function with its arguments to the threadpool
         * @tparam F function signature
         * @tparam Args function argument types
         * @param function function signature
         * @param args arguments to pass to the function
         * @returns a future holding the asynchronous function result
         * @exception std::runtime_error if the thread pool has been shut down
        */
        template <class F, class... Args>
        std::future<std::result_of_t<F(Args...)>> submit(F &&function, Args &&...args)
        {
            std::shared_ptr<std::packaged_task<std::result_of_t<F(Args...)>()>> packaged_task;
            std::function<void()> work;
            std::future<std::result_of_t<F(Args...)>> result;

            packaged_task = std::make_shared<std::packaged_task<std::result_of_t<F(Args...)>()>>(std::bind(std::forward<F>(function), std::forward<Args>(args)...));
            result = packaged_task->get_future();

            work = [packaged_task]()
            {
                (*packaged_task)();
            };

            push_task(std::move(work));

            return result;
        }

        /**
         * @brief check if the threadpool is active
         * @returns true if the threadpool is active, false if it has been shut down
        */
        bool active() const;

        /**
         * @brief shut down threadpool by joining threads and rejecting submissions
         * @exception std::runtime_error if the thread pool has already been shut down
        */
        void shutdown();

    private:
        void push_task(std::function<void()> &&task);
        bool pop_task(std::function<void()> &task);

        size_t num_threads;
        bool _active;
        std::vector<std::thread> _workers;
        std::queue<std::function<void()>> _tasks;
        std::mutex _mutex;
        std::condition_variable _condition;
    };
}
#endif