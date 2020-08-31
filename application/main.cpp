#include "QuranImagePage.hpp"

int main(int argc, char **argv)
{
    std::cout << pagesWithRoubou3.size() << std::endl;
    for (size_t it = 201; it < 605; it++)
    //for (std::map<int,int>::const_iterator it = pagesWithSourateHeader.begin(); it != pagesWithSourateHeader.end(); ++it)
    //for (std::map<int,int>::const_iterator it = pagesWithBasmala.begin(); it != pagesWithBasmala.end(); ++it)
    // for (std::map<int,int>::const_iterator it = pagesWithSoujoud.begin(); it != pagesWithSoujoud.end(); ++it)
    // for (std::map<int, int>::const_iterator it = pagesWithRoubou3.begin(); it != pagesWithRoubou3.end(); ++it)
    {
        QuranImagePage qimage(it);
        qimage.showContours();
    }

    return 0;
}
