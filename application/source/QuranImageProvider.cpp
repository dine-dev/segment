#include "QuranImageProvider.hpp"

QuranImageProvider::QuranImageProvider(const std::string &ressources_directory, const std::string &base_url)
    : _ressources_directory(ressources_directory), _base_url(base_url)
{
    if (!dirExists(_ressources_directory))
    {
        mkdir(_ressources_directory.c_str(), 0777);
    }
}

QuranImageProvider::~QuranImageProvider() {}


/******************************************************************************
 * Checks to see if a directory exists. Note: This method only checks the
 * existence of the full path AND if path leaf is a dir.
 *
 * @return  >0 if dir exists AND is a dir,
 *           0 if dir does not exist OR exists but not a dir,
 *          <0 if an error occurred (errno is also set)
 *****************************************************************************/
int QuranImageProvider::dirExists(const std::string &path)
{
    // https://stackoverflow.com/questions/18100097/portable-way-to-check-if-directory-exists-windows-linux-c
    struct stat info;

    int statRC = stat( path.c_str(), &info );
    if( statRC != 0 )
    {
        if (errno == ENOENT)  { return 0; } // something along the path does not exist
        if (errno == ENOTDIR) { return 0; } // something in path prefix is not a dir
        return -1;
    }

    return ( info.st_mode & S_IFDIR ) ? 1 : 0;
    
}

int QuranImageProvider::fileExists(const std::string &path)
{
    // https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
    struct stat info;
    return (stat (path.c_str(), &info) == 0);
}

std::string QuranImageProvider::getQuranImagePath(int pageNumber)
{
    std::string path = _ressources_directory + "/" + std::to_string(pageNumber) + ".jpg";
    if(!fileExists(path))
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
