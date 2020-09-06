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

bool rectangleHasIntersection(const cv::Rect &rect_lhs, const cv::Rect &rect_rhs)
{
    return ((rect_lhs & rect_rhs).area() > 0);
}

void QuranImagePage::showContours()
{
    cv::Mat src = cv::imread(_fileName, cv::IMREAD_COLOR);

    // text panel
    cv::Rect text_panel;
    if (hasTextArea(text_panel))
    {
        cv::rectangle(src, text_panel, cv::Scalar(255, 0, 0), 3);
    }

    // tajweed footer
    cv::Rect tajweed_panel;
    if (hasTajweedFooter(tajweed_panel))
    {
        cv::rectangle(src, tajweed_panel, cv::Scalar(255, 0, 0), 3);
    }

    // sourate header
    std::vector<cv::Rect> sourate_header_panels;
    if (hasSourateHeader(sourate_header_panels))
    {
        for (size_t ind = 0; ind < sourate_header_panels.size(); ++ind)
        {
            cv::rectangle(src, sourate_header_panels[ind], cv::Scalar(255, 0, 0), 3);
        }
    }

    // basmala
    std::vector<cv::Rect> basmala_panels;
    if (hasBasmala(basmala_panels))
    {
        for (size_t ind = 0; ind < basmala_panels.size(); ++ind)
        {
            cv::rectangle(src, basmala_panels[ind], cv::Scalar(255, 0, 0), 3);
        }
    }

    // soujoud
    cv::Rect soujoud_panel;
    if (hasSoujoud(soujoud_panel))
    {
        cv::rectangle(src, soujoud_panel, cv::Scalar(255, 0, 0), 3);
    }

    // roukou3
    cv::Rect roubou3_panel;
    if (hasRoubou3(roubou3_panel))
    {
        cv::rectangle(src, roubou3_panel, cv::Scalar(255, 0, 0), 3);
    }

    // ayahs
    std::vector<cv::Rect> ayah_panel;
    if (hasAyah(ayah_panel))
    {
        for (size_t ind = 0; ind < ayah_panel.size(); ++ind)
        {
            cv::rectangle(src, ayah_panel[ind], cv::Scalar(255, 0, 0), 3);
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

    // require additional processing for first two pages because tajweed panel includes in text panel
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
        std::string template_dir = "../../application/templates/";
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
        // Determine image template to use
        std::string template_dir = "../../application/templates/";
        std::string template_file_name = (_number == 2) ? "basmalah_second_page.jpg" : "basmalah.jpg";
        template_file_name = template_dir + template_file_name;
        matchSeveral(bounding_rects, template_file_name, 0.3);
    }
    return hasBasmala;
}

bool QuranImagePage::hasTajweedFooter(cv::Rect &bounding_rect)
{
    // Determine image template to use
    std::string template_dir = "../../application/templates/";
    std::string template_file_name = (_number == 1) ? "footer_first_page.jpg" : ((_number == 2) ? "footer_second_page.jpg" : "footer.jpg");
    template_file_name = template_dir + template_file_name;

    return matchOne(bounding_rect, template_file_name);
}

bool QuranImagePage::hasSoujoud(cv::Rect &bounding_rect)
{
    bool hasSoujoud = pagesWithSoujoud.count(_number);
    if (hasSoujoud)
    {
        // Determine image template to use
        std::string template_dir = "../../application/templates/";
        std::string template_file_name = "soujoud.jpg";
        template_file_name = template_dir + template_file_name;

        return matchOne(bounding_rect, template_file_name);
    }
    return hasSoujoud;
}

bool QuranImagePage::hasRoubou3(cv::Rect &bounding_rect)
{
    bool hasRoubou3 = pagesWithRoubou3.count(_number) && pagesWithRoubou3.at(_number);
    if (hasRoubou3)
    {
        // Determine image template to use
        std::string template_dir = "../../application/templates/";
        std::string template_file_name = "roubou3.jpg";
        template_file_name = template_dir + template_file_name;

        return matchOne(bounding_rect, template_file_name, 0.3);
    }
    return hasRoubou3;
}

bool QuranImagePage::hasAyah(std::vector<cv::Rect> &bounding_rects)
{
    std::string template_dir = "../../application/templates/";
    std::string template_file_name = (_number == 1) ? "ayah_symbol_first_page.jpg" : ((_number == 2) ? "ayah_symbol_second_page.jpg" : "ayah_symbol.jpg");
    template_file_name = template_dir + template_file_name;

    // perform ayah symbol detection
    matchSeveral(bounding_rects, template_file_name, 0.4);

    // check if number of ayah symbol match number of ayah
    int number_of_ayah = pagesAyahNumber.at(_number);
    if (bounding_rects.size() < number_of_ayah)
    {
        // test other ayah symbols
        std::vector<std::string> template_file_names {
            "ayah_symbol_1.jpg",
            "ayah_symbol_2.jpg",
            "ayah_symbol_3.jpg"
        };

        for (size_t ind = 0; ind < template_file_names.size(); ++ind)
        {
            matchSeveral(bounding_rects, template_dir + template_file_names[ind], 0.4);
        }
    }

    return true;
}

bool QuranImagePage::matchOne(cv::Rect &bounding_rect, std::string template_file_name, double threshold, int match_method)
{
    cv::Mat3b ref = cv::imread(_fileName, cv::IMREAD_COLOR);
    cv::Mat3b tpl = cv::imread(template_file_name, cv::IMREAD_COLOR);

    cv::Mat gref, gtpl;
    cv::cvtColor(ref, gref, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tpl, gtpl, cv::COLOR_BGR2GRAY);
    cv::Mat res(ref.rows - tpl.rows + 1, ref.cols - tpl.cols + 1, CV_32FC1);
    cv::matchTemplate(gref, gtpl, res, match_method);
    cv::threshold(res, res, threshold, 1., cv::THRESH_TOZERO);

    double minval, maxval;
    cv::Point minloc, maxloc;
    cv::minMaxLoc(res, &minval, &maxval, &minloc, &maxloc);

    if (maxval >= threshold)
    {
        // build bounding rectangle
        bounding_rect = cv::Rect(maxloc, cv::Point(maxloc.x + tpl.cols, maxloc.y + tpl.rows));
        return true;
    }
    else
    {
        return false;
    }

}

bool QuranImagePage::matchSeveral(std::vector<cv::Rect> &bounding_rects, std::string template_file_name, double threshold, int match_method)
{
    cv::Mat3b ref = cv::imread(_fileName, cv::IMREAD_COLOR);
    cv::Mat3b tpl = cv::imread(template_file_name, cv::IMREAD_COLOR);

    cv::Mat gref, gtpl;
    cv::cvtColor(ref, gref, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tpl, gtpl, cv::COLOR_BGR2GRAY);
    cv::Mat res(ref.rows - tpl.rows + 1, ref.cols - tpl.cols + 1, CV_32FC1);
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
                                                              [=](const cv::Rect &rect) { return rectangleHasIntersection(bounding_rect, rect); });

            // if retangle has no intersection with existing rectangles in vector then add to vector
            if (it == bounding_rects.end())
            {
                bounding_rects.push_back(bounding_rect);
            }
            else
            {
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

bool QuranImagePage::segmentWordInPage() {}

/*

void ImageAnalyzer::segmentWordInPolygon(const std::string & pathImageQuranPage, const std::vector<cv::Point> & polygon) {
    cv::Mat3b img = cv::imread(pathImageQuranPage, cv::IMREAD_COLOR), img_disp = img.clone();
    //cv::rectangle(img_disp, cv::Point(481,83), cv::Point(523,137), cv::Scalar( 255, 0, 0 ), 1);
    //cv::rectangle(img_disp, cv::Point(18,23), cv::Point(623,80), cv::Scalar( 255, 0, 0 ), 1);

    cv::Mat mask = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
    std::vector<cv::Point> pts = {
        cv::Point(18, 23),
        cv::Point(623, 23),
        cv::Point(623, 137),
        cv::Point(523, 137),
        cv::Point(523, 80),
        cv::Point(18, 80)
    };

    cv::fillConvexPoly( mask, pts.data(), pts.size(), cv::Scalar(255) );
    
    cv::Mat imageDest = cv::Mat::zeros(img.rows, img.cols, CV_8UC3);
    imageDest.setTo(cv::Scalar(255,255,255));
    img.copyTo(imageDest, mask);
    
    //cv::imshow("mask",mask);
    //cv::imshow("imageDest",imageDest);

    cv::Mat src_gray_thr;
    cv::cvtColor(imageDest, src_gray_thr, cv::COLOR_BGR2GRAY);

    cv::threshold( src_gray_thr, src_gray_thr, 240, 255, cv::THRESH_BINARY);
    
    // invert black and white
    cv::bitwise_not ( src_gray_thr, src_gray_thr );
    

    cv::Mat kernel = (cv::Mat_<uchar>(9,9) << 0,0,0,0,1,0,0,0,0,
                                              0,0,1,1,1,0,0,0,0,
                                              0,0,0,1,1,0,0,0,0,
                                              0,0,0,0,1,0,0,0,0,
                                              0,0,1,1,1,1,1,0,0,
                                              0,0,0,0,1,0,0,0,0,
                                              0,0,0,0,1,0,0,0,0,
                                              0,0,0,0,1,0,0,0,0,
                                              0,0,0,0,1,0,0,0,0);

    cv::Mat dilation_dst;
    cv::dilate( src_gray_thr, dilation_dst, kernel);

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(dilation_dst, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
    std::vector<cv::Rect> contour_rect;

    // get bounding box of contours
    for (size_t idx = 0; idx < contours.size(); idx++) {
        cv::Rect rect = cv::boundingRect(contours[idx]);

        // check intersection of curret rectangle with other present rectangle
        std::vector<cv::Rect>::iterator it = std::find_if(contour_rect.begin(), contour_rect.end(),
                                                          [=] (const cv::Rect & cRect) { return rectangleHasIntersection(cRect,rect); } );
        
        // if curretn rectangle does not have intersection with other rectangles just add it
        if(it == contour_rect.end()){
            contour_rect.push_back(rect);
        }
        // if current rectangle has an intersection with other rectangles
        else {
            cv::Rect iRect = (*it);

            // if the intersection is exactly rectangle in vector that mean that rect contains the rectangle in vector
            if ( iRect == (iRect & rect) ) {
                (*it) = rect;
            }
            // if the intersection is exactly current rectangle that means that that the rectangle in vector contains rect contain
            else if ( rect == (iRect & rect) ){
                continue;
            }
            else {
                contour_rect.push_back(rect);
            }

        }
        
        // Draw the contours rectangle
        cv::drawContours( img, contours, idx, cv::Scalar(0, 0, 255), 1, 8, hierarchy, 0);
        //cv::rectangle(img, rect.tl(), rect.br(), cv::Scalar(255, 0, 0));
    }

    for (size_t idx = 0; idx < contour_rect.size(); idx++) {
        
        // Draw the contours rectangle
        cv::rectangle(img, contour_rect[idx].tl(), contour_rect[idx].br(), cv::Scalar(255, 0, 0));
    }
    
    cv::imshow("imageDest",src_gray_thr);
    cv::imshow("dilation_dst",dilation_dst);
    cv::imshow("img",img);
}


void ImageAnalyzer::getTextFrameRect(const cv::Mat & imageQuranPage, cv::Rect & textFrame) {
    cv::Mat src_gray_thr;
    cv::cvtColor(imageQuranPage, src_gray_thr, cv::COLOR_BGR2GRAY);
    cv::threshold( src_gray_thr, src_gray_thr, 240, 255, cv::THRESH_BINARY);

    // for src image determine contours then find max area contour and the associated bounding rectangle
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point> >::const_iterator result;

    cv::findContours(src_gray_thr, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
    result = std::max_element(contours.begin(), contours.end(), utils::comp);
    textFrame = cv::boundingRect((*result));
}

*/