#include <iostream>
#include "threadpool.hpp"

maxtek::threadpool::threadpool(size_t threads)
{
    if (threads == 0)
    {
        throw std::runtime_error("failed to construct threadpool with zero threads");
    }

    _active = true;

    _workers.reserve(threads);

    while (_workers.size() < _workers.capacity())
    {
        _workers.push_back(std::thread([&]()
                                       {
        std::function<void()> task;
        while (pop_task(task))
        {
            task();
        } }));
    }
}

maxtek::threadpool::~threadpool()
{
    if (_active)
    {
        shutdown();
    }
}

bool maxtek::threadpool::active() const
{
    return _active;
}

void maxtek::threadpool::shutdown()
{
    if (!_active)
    {
        throw std::runtime_error("failed to shut down inactive threadpool");
    }
    _active = false;
    _condition.notify_all();
    for (std::thread &worker : _workers)
    {
        worker.join();
    }
}

void maxtek::threadpool::push_task(std::function<void()> &&task)
{
    std::unique_lock<std::mutex> lock(_mutex);
    if (!_active)
    {
        throw std::runtime_error("failed to submit to inactive threadpool");
    }
    _tasks.push(std::move(task));
    lock.unlock();
    _condition.notify_one();
}

bool maxtek::threadpool::pop_task(std::function<void()> &task)
{
    std::unique_lock<std::mutex> lock(_mutex);
    bool result(false);
    _condition.wait(
        lock,
        [&]()
        {
            return (!_active || !_tasks.empty());
        });
    if (_active)
    {
        task = _tasks.front();
        _tasks.pop();
        result = true;
    }
    return result;
}