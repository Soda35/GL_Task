#include <iostream> 

#include "FileSearcher.h"

int main(int argc, char* argv[])
{
    std::string fileName;
    std::cout << "Enter file name: ";
    std::cin >> fileName;

    FileSearcher searcher;
    searcher.searchFile(fileName);

    return 0;
}