
#ifndef HTTP_DOWNLOADER_H
#define HTTP_DOWNLOADER_H

#include <stdio.h>
#include <curl/curl.h>
#include <iostream>

namespace HttpDownloader
{
    size_t callbackfunction(void *ptr, size_t size, size_t nmemb, void* userdata);
    bool download_jpeg(const std::string & urlFile, const std::string & fName);
}

#endif