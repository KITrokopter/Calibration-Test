#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <list>
#include <vector>
#include <iostream>
#include <unistd.h>

#include "libfreenect.hpp"
#include "CvKinect.hpp"

void findObject(cv::Mat rgb) {
	cv::Mat hsv;
	
	cv::cvtColor(rgb, hsv, CV_RGB2HSV);
	unsigned char* data = hsv.data;
	size_t total = hsv.total();
	
	// Mask
	for (unsigned int i = 0; i < total; i += 3) {
		if (data[i] < 20 || data[i] > 235) {
			data[i + 1] = 0;
			data[i + 2] = 255;
		}
	}
	
	// Show result
	cv::cvtColor(hsv, rgb, CV_HSV2RGB);
	cv::imshow("rgb", rgb);

	sleep(5);
}

int main(void) {
	printf("Hello World!\n");
	
	cv::Mat depthMat(cv::Size(640,480), CV_16UC1);
	cv::Mat depthf(cv::Size(640,480), CV_8UC1);
	cv::Mat rgbMat(cv::Size(640,480), CV_8UC3, cv::Scalar(0));
	
	std::list<cv::Mat> images;
	
	Freenect::Freenect freenect;
	CvKinect& device = freenect.createDevice<CvKinect>(0);
	
	device.startVideo();
	
	cv::namedWindow("rgb", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("depth", CV_WINDOW_AUTOSIZE);
	
	while (true) {
		device.getVideo(rgbMat);
		
		// if (images.size() > 0) {
		// 	cv::imshow("rgb", images.front());
		// } else {
			cv::imshow("rgb", rgbMat);
		// }
		
		char key = cv::waitKey(5);
		
		if (key == 'x') {
			break;
		} else if (key == 's') {
			images.push_back(rgbMat.clone());
		}
	}
	
	device.stopVideo();
	
	std::cout << "Images: " << images.size() << std::endl;
	
	for (std::list<cv::Mat>::iterator iter = images.begin(); iter != images.end(); ++iter) {
		cv::imshow("rgb", *iter);
	
		//////////////////////////
		findObject(*iter);
		//////////////////////////
		
		cv::Mat greyMat(cv::Size(640, 480), CV_8UC1, cv::Scalar(0));
	
		cv::cvtColor(*iter, greyMat, CV_RGB2GRAY);
		std::vector<cv::Point2f> corners;
		
		bool found = cv::findChessboardCorners(greyMat, cv::Size(7, 7), corners);
		
		std::cout << "Corners found: " << corners.size() << std::endl;
	}
	
	cv::destroyWindow("depth");
	cv::destroyWindow("rgb");
	
	std::cout << "Exit" << std::endl;
	
	return 0;
}
