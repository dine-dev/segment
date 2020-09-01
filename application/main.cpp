#include "QuranImagePage.hpp"

int main(int argc, char **argv)
{
    // for (size_t it = 1; it < 605; it++)
    for (std::map<int, int>::const_iterator it = pagesAyahNumber.begin(); it != pagesAyahNumber.end(); ++it)
    {
        // QuranImagePage qimage(it->first);
        QuranImagePage qimage(it->first);
        qimage.showContours();
        // std::vector<cv::Rect> ayah_panel;
        // qimage.hasAyah(ayah_panel);
        // std::cout << "{" << it->first << "," << ayah_panel.size() << "},";
    }

    return 0;
}
