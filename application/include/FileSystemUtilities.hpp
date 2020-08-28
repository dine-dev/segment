#ifndef FILE_SYSTEM_UTILITIES_H
#define FILE_SYSTEM_UTILITIES_H

#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

namespace FileSystemUtilities
{
    int dirExists(const std::string &path);
    int fileExists(const std::string &path);
}

#endif