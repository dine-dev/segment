#include "QuranImageProvider.hpp"

QuranImageProvider::QuranImageProvider(const std::string &ressources_directory, const std::string &base_url)
    : _ressources_directory(ressources_directory), _base_url(base_url)
{
    if (!FileSystemUtilities::dirExists(_ressources_directory))
    {
        mkdir(_ressources_directory.c_str(), 0777);
    }
}

QuranImageProvider::~QuranImageProvider() {}


std::string QuranImageProvider::getQuranImagePath(int pageNumber)
{
    std::string path = _ressources_directory + "/" + std::to_string(pageNumber) + ".jpg";
    if(!FileSystemUtilities::fileExists(path))
    {
        std::string urlFile = _base_url + std::to_string(pageNumber) + ".jpg";
        HttpDownloader::download_jpeg(urlFile, path);
    }
    
    return path;
}

void QuranImageProvider::fetchAllQuranImage()
{
    for (size_t i = 1; i < 605; i++)
    {
        std::cout << "image " << i << " downloaded at " << getQuranImagePath(i) << std::endl;
    }
}
