#include "QuranImagePage.hpp"

int main(int argc, char **argv)
{

    // for (size_t it = 1; it < 5; it++)
    for (std::set<int>::iterator it = pagesWithSourateHeader.begin(); it != pagesWithSourateHeader.end(); ++it)
    {
        QuranImagePage qimage(*it);
        qimage.showContours();
        // std::vector<cv::Rect> bounding_rects;
        // std::cout << qimage.hasSourateHeader(bounding_rects) << std::endl;
    }

    return 0;
}

