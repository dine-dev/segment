#include "QuranImagePage.hpp"

QuranImagePage::QuranImagePage(int number, std::string riwaya) : _number(number), _riwaya(riwaya)
{
    if (riwaya == "hafs")
    {
        QuranImageProvider qip("ressources", "http://easyquran.com/quran-jpg/images/");
        _fileName = qip.getQuranImagePath(_number);
    }
}

QuranImagePage::~QuranImagePage()
{
}

void QuranImagePage::showImagePage()
{
    cv::Mat image = cv::imread(_fileName, cv::IMREAD_COLOR);
    if (!image.data)
    {
        std::cout << "Could not open or find the image" << std::endl;
        return;
    }

    cv::namedWindow("Display " + _fileName, cv::WINDOW_AUTOSIZE);
    cv::imshow("Display " + _fileName, image);

    cv::waitKey(0);
}

bool comp(const std::vector<cv::Point> &contour_lhs, const std::vector<cv::Point> &contour_rhs)
{
    return (cv::contourArea(contour_lhs) > cv::contourArea(contour_rhs));
}

bool rectangleHasIntersection (const cv::Rect & rect_lhs, const cv::Rect & rect_rhs) {
    return ((rect_lhs & rect_rhs).area() > 0);
}

void QuranImagePage::showContours()
{
    cv::Mat src = cv::imread(_fileName, cv::IMREAD_COLOR);

    // text panel
    cv::Rect text_panel;
    if (hasTextArea(text_panel))
    {
        cv::rectangle(src, text_panel, cv::Scalar(0, 255, 0), 3);
    }

    // tajweed footer
    cv::Rect tajweed_panel;
    if (hasTajweedFooter(tajweed_panel))
    {
        cv::rectangle(src, tajweed_panel, cv::Scalar(0, 255, 0), 3);
    }

    // sourate header
    std::vector<cv::Rect> sourate_header_panels;
    if (hasSourateHeader(sourate_header_panels))
    {
        for (size_t ind = 0; ind < sourate_header_panels.size(); ++ind)
        {
            cv::rectangle(src, sourate_header_panels[ind], cv::Scalar(0, 255, 0), 3);
        }
    }

    cv::imshow("Display page " + std::to_string(_number), src);
    cv::moveWindow("Display page " + std::to_string(_number), 600, 20);
    cv::waitKey(0);
    cv::destroyWindow("Display page " + std::to_string(_number));
}

bool QuranImagePage::hasTextArea(cv::Rect &bounding_rect)
{
    cv::Mat src = cv::imread(_fileName, cv::IMREAD_COLOR);

    // convert the image to Gray
    cv::Mat src_gray;
    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

    // 0: Binary 1: Binary Inverted 2: Threshold Truncated 3: Threshold to Zero 4: Threshold to Zero Inverted
    int threshold_type = 4, threshold_value = 185; // max value = 255;
    int const max_BINARY_value = 255;

    // perform threshold in grayscale
    cv::Mat dst;
    cv::threshold(src_gray, dst, threshold_value, max_BINARY_value, threshold_type);

    // determine contours
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(dst, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    // sort coutour by area
    std::sort(contours.begin(), contours.end(), comp);

    // text area is the second highest area coutours
    bounding_rect = cv::boundingRect((contours[1]));

    // require additional processing for first two pages because tajweed panel include in text panel
    if (_number < 3)
    {
        cv::Rect tajweed_panel;
        if (hasTajweedFooter(tajweed_panel))
        {
            // just remove the height of tajweed panel (plus offset) from text panel
            int height_offset = 5;
            bounding_rect.height -= (tajweed_panel.height + height_offset);
        }
    }
    return true;
}

bool QuranImagePage::hasSourateHeader(std::vector<cv::Rect> &bounding_rects)
{
    bool hasHeader = pagesWithSourateHeader.count(_number);
    if (hasHeader)
    {
        // Determine image template to use
        std::string template_dir = "../../application/template_matching_ressources/";
        std::string template_file_name = (_number < 3) ? "header_first_second_page.jpg" : "header.jpg";
        template_file_name = template_dir + template_file_name;
        matchSeveral(bounding_rects, template_file_name, 0.7);
    }
    return hasHeader;
}

bool QuranImagePage::hasBasmala(std::vector<cv::Rect> &bounding_rects)
{
    bool hasBasmala = pagesWithBasmala.count(_number);
    if (hasBasmala)
    {
    }
    return hasBasmala;
}

bool QuranImagePage::hasTajweedFooter(cv::Rect &bounding_rect)
{
    // Determine image template to use
    std::string template_dir = "../../application/template_matching_ressources/";
    std::string template_file_name = (_number == 1) ? "footer_first_page.jpg" : ((_number == 2) ? "footer_second_page.jpg" : "footer.jpg");
    template_file_name = template_dir + template_file_name;

    return matchOne(bounding_rect, template_file_name);
}

bool QuranImagePage::hasSoujoud(cv::Rect &bounding_rect)
{
    return false;
}

bool QuranImagePage::hasRoubou3(cv::Rect &bounding_rect)
{
    return false;
}

bool QuranImagePage::matchOne(cv::Rect &bounding_rect, std::string template_file_name, int match_method)
{
    // "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";

    cv::Mat img = cv::imread(_fileName, cv::IMREAD_COLOR);
    cv::Mat templ = cv::imread(template_file_name, cv::IMREAD_COLOR);

    // Source image to display
    cv::Mat img_display;
    img.copyTo(img_display);

    // Create the result matrix
    int result_cols = img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;

    cv::Mat result;
    result.create(result_rows, result_cols, CV_32FC1);

    // Do the Matching and Normalize
    cv::matchTemplate(img, templ, result, match_method);
    cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    // Localizing the best match with minMaxLoc
    double minVal, maxVal;
    cv::Point minLoc, maxLoc, matchLoc;

    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

    // For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
    matchLoc = (match_method == cv::TM_SQDIFF || match_method == cv::TM_SQDIFF_NORMED) ? minLoc : maxLoc;

    // return tajweed footer area rectangle
    bounding_rect = cv::Rect(matchLoc, cv::Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows));

    return true;
}

bool QuranImagePage::matchSeveral(std::vector<cv::Rect> &bounding_rects, std::string template_file_name, double threshold, int match_method)
{

    bounding_rects.clear();
    cv::Mat3b ref = cv::imread(_fileName, cv::IMREAD_COLOR);
    cv::Mat3b tpl = cv::imread(template_file_name, cv::IMREAD_COLOR);

    cv::Mat gref, gtpl;
    cv::cvtColor(ref, gref, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tpl, gtpl, cv::COLOR_BGR2GRAY);
    cv::Mat res(ref.rows-tpl.rows+1, ref.cols-tpl.cols+1, CV_32FC1);
    cv::matchTemplate(gref, gtpl, res, match_method);
    cv::threshold(res, res, threshold, 1., cv::THRESH_TOZERO);

    while (true) 
    {
        double minval, maxval;
        cv::Point minloc, maxloc;
        cv::minMaxLoc(res, &minval, &maxval, &minloc, &maxloc);

        if (maxval >= threshold)
        {
            // build bounding rectangle
            cv::Rect bounding_rect(maxloc, cv::Point(maxloc.x + tpl.cols, maxloc.y + tpl.rows));
            
            // add to vector only if has no intersection with previous added
            std::vector<cv::Rect>::iterator it = std::find_if(bounding_rects.begin(), bounding_rects.end(),
                                                              [=] (const cv::Rect & rect) { return rectangleHasIntersection(bounding_rect,rect); } );

            // if retangle has no intersection with existing rectangles in vector then add to vector
            if (it == bounding_rects.end()) {
                bounding_rects.push_back(bounding_rect);
            }
            else {
                (*it) = (*it) | bounding_rect;
            }
            
            // post processing
            cv::floodFill(res, maxloc, cv::Scalar(0), 0, cv::Scalar(.1), cv::Scalar(1.));
        }
        else
        {
            break;
        }
    }
    
    return true;
}
