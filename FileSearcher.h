#pragma once

#include <string>

#include "TaskManager.h"

class FileSearcher
{
public:
    FileSearcher(size_t threadsNumber = 8);
    ~FileSearcher() = default;
    void searchFile(const std::string& fileName);

private:
    static bool subdirectoriesSearch(const fs::path& directory, const std::string& fileName, std::stop_token stopToken);
    TaskManager _manager;
};