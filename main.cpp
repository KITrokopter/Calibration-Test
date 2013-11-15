#include <stdio.h>
#include <cv.h>

#include "libfreenect.hpp"
#include "MyDevice.hpp"

int main(void) {
	printf("Hello World!\n");
	
	cv::Mat depthMat(cv::Size(640,480), CV_16UC1);
	cv::Mat depthf(cv::Size(640,480), CV_8UC1);
	cv::Mat rgbMat(cv::Size(640,480), CV_8UC3, cv::Scalar(0));
	cv::Mat ownMat(cv::Size(640,480), CV_8UC3, cv::Scalar(0));
	
	Freenect::Freenect freenect;
	MyDevice& device = freenect.createDevice<MyDevice>(0);
	
	(void) device;
	
	return 0;
}
