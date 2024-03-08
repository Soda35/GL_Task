#include "FileSearcher.h"

#include <iostream>

FileSearcher::FileSearcher(size_t threadsNumber) :
    _manager(threadsNumber)
{
}

void FileSearcher::searchFile(const std::string& fileName)
{
    std::cout << "Searching..." << std::endl;
    fs::path rootPath = fs::current_path().root_path();

    for (const auto& directory : fs::directory_iterator(rootPath, fs::directory_options::skip_permission_denied))
    {
        try {
            if (!fs::is_directory(directory) || !fs::exists(directory)) {
                continue;
            }
        }
        catch (fs::filesystem_error exception) {
            std::cout << directory.path() << ": " << exception.what() << std::endl;
        }

        TaskManager::Task fileSearch = [directory, fileName](std::stop_token stopToken) {
            return subdirectoriesSearch(directory, fileName, stopToken);
        };

        _manager.add(fileSearch);
    }


    _manager.start();
}

bool FileSearcher::subdirectoriesSearch(const fs::path& directory, const std::string& fileName, std::stop_token stopToken)
{
    fs::recursive_directory_iterator recursiveIterator;

    try {
        recursiveIterator = fs::recursive_directory_iterator(directory, fs::directory_options::skip_permission_denied);
    }
    catch (fs::filesystem_error exception) {
        return false;
    }

    while (recursiveIterator != fs::end(recursiveIterator))
    {
        if (stopToken.stop_requested())
        {
            return false;
        }

        try {
            const auto subdirectory = *recursiveIterator;

            if (!fs::is_directory(subdirectory) || !fs::exists(directory)) {
                continue;
            }

            const auto& searchPath = subdirectory.path() / fileName;
            if (fs::exists(searchPath))
            {
                std::cout << "Found: " << searchPath << std::endl;
                return true;
            }

            ++recursiveIterator;
        }
        catch (fs::filesystem_error exception) {
            std::cout << exception.what() << std::endl;
            recursiveIterator.disable_recursion_pending();
            ++recursiveIterator;
        }
    }

    return false;
}
