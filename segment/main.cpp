#include <iostream>
#include <opencv2/imgcodecs.hpp>

using namespace std;

int main()
{
	const auto img = cv::imread("capture.png");
	cout << "image size: " << img.cols << "x" << img.rows << endl;
	return 0;
}
