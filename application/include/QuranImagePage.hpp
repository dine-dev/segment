#ifndef QURAN_IMAGE_PAGE_H
#define QURAN_IMAGE_PAGE_H

#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "QuranMetaData.hpp"
#include "QuranImageProvider.hpp"

class QuranImagePage
{
private:
    int _number;
    std::string _riwaya;
    std::string _fileName;

public:
    QuranImagePage(int _number, std::string riwaya = "hafs");
    ~QuranImagePage();
    void showImagePage();
    void showContours();
    bool hasTextArea(cv::Rect &bounding_rect);
    bool hasSourateHeader(std::vector<cv::Rect> &bounding_rects);
    bool hasBasmala(std::vector<cv::Rect> &bounding_rects);
    bool hasTajweedFooter(cv::Rect &bounding_rect);
    bool hasSoujoud(cv::Rect &bounding_rect);
    bool hasRoubou3(cv::Rect &bounding_rect);
    
private:
    bool matchOne(cv::Rect &bounding_rect, std::string template_file_name, double threshold = 0.8, int match_method = cv::TM_CCOEFF_NORMED);
    bool matchSeveral(std::vector<cv::Rect> &bounding_rects, std::string template_file_name, double threshold = 0.8, int match_method = cv::TM_CCOEFF_NORMED);

};

#endif