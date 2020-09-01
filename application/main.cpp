#include "QuranImagePage.hpp"

int main(int argc, char **argv)
{
    std::cout << pagesWithRoubou3.size() << std::endl;
    int nAyah = 0;
    for (size_t it = 1; it < 605; it++)
    //for (std::map<int,int>::const_iterator it = pagesWithSourateHeader.begin(); it != pagesWithSourateHeader.end(); ++it)
    //for (std::map<int,int>::const_iterator it = pagesWithBasmala.begin(); it != pagesWithBasmala.end(); ++it)
    // for (std::map<int,int>::const_iterator it = pagesWithSoujoud.begin(); it != pagesWithSoujoud.end(); ++it)
    // for (std::map<int, int>::const_iterator it = pagesWithRoubou3.begin(); it != pagesWithRoubou3.end(); ++it)
    {
        QuranImagePage qimage(it);
        //qimage.showContours();
        std::vector<cv::Rect> ayah_panel;
        qimage.hasAyah(ayah_panel);
        nAyah += ayah_panel.size();
    }
        std::cout << "number of ayah " << nAyah << std::endl;

    return 0;
}
