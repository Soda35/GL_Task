#pragma once

#include <functional>
#include <thread>
#include <queue>
#include <filesystem> 

namespace fs = std::filesystem;

class TaskManager
{
public:
    using Task = std::function<bool(std::stop_token)>;

    TaskManager(size_t maxThreads);
    ~TaskManager() = default;

    void add(Task task);
    void start();

private:
    void run(std::stop_token stopToken);

    std::queue<Task> _tasks;
    std::vector<std::thread> _threads;
    std::stop_source _stopSource;
    std::mutex _mutex;
    size_t _maxThreads;
};