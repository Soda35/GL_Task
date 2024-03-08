#include "TaskManager.h"

TaskManager::TaskManager(size_t maxThreads) : _maxThreads(maxThreads) {
    _threads.reserve(maxThreads);
}

void TaskManager::add(Task task) {
    _tasks.push(task);
}

void TaskManager::start() {
    auto stopToken = _stopSource.get_token();

    for (size_t i = 0; i < std::min(_tasks.size(), _maxThreads); i++) {
        _threads.emplace_back(&TaskManager::run, this, stopToken);
    }

    for (size_t i = 0; i < _threads.size(); i++) {
        if (_threads[i].joinable())
        {
            _threads[i].join();
        }
    }
}

void TaskManager::run(std::stop_token stopToken) {
    while (!stopToken.stop_requested() && !_tasks.empty())
    {
        Task currentTask;

        {
            std::lock_guard<std::mutex> guard(_mutex);

            if (_tasks.empty())
            {
                return;
            }

            currentTask = _tasks.front();
            _tasks.pop();
        }

        const auto result = currentTask(stopToken);
        if (result)
        {
            _stopSource.request_stop();
        }
    }
}
