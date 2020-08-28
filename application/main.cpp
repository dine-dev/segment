#include "QuranImagePage.hpp"

int main(int argc, char **argv)
{

    //for (size_t it = 1; it < 5; it++)
    int n = 0;
    //for (std::map<int,int>::const_iterator it = pagesWithSourateHeader.begin(); it != pagesWithSourateHeader.end(); ++it)
    for (std::map<int,int>::const_iterator it = pagesWithBasmala.begin(); it != pagesWithBasmala.end(); ++it)
    {
        QuranImagePage qimage(it->first);
        qimage.showContours();
        std::vector<cv::Rect> bounding_rects;
        //qimage.hasSourateHeader(bounding_rects);
        qimage.hasBasmala(bounding_rects);
        n += bounding_rects.size();
        //std::cout << qimage.hasSourateHeader(bounding_rects) << std::endl;
    }
    std::cout << n << std::endl;

    return 0;
}

