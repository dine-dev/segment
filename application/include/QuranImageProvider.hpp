#ifndef QURAN_IMAGE_PROVIDER_H
#define QURAN_IMAGE_PROVIDER_H

#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

#include "HttpDownloader.hpp"

class QuranImageProvider
{
private:
    std::string _ressources_directory;
    std::string _base_url;

public:
    QuranImageProvider(const std::string &ressources_directory, const std::string &base_url);
    ~QuranImageProvider();
    std::string getQuranImagePath(int pageNumber);
    void fetchAllQuranImage();

private:
    int dirExists(const std::string &path);
    int fileExists(const std::string &path);
};

#endif