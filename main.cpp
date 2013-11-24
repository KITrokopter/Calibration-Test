#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <list>
#include <vector>
#include <iostream>
#include <sys/time.h>

#include "libfreenect.hpp"
#include "CvKinect.hpp"
#include "Area.hpp"



void findObject(cv::Mat rgb) {
	timespec ts, te;

	if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
		std::cout << "Time error" << std::endl;
	}

	cv::Mat hsv;
	cv::Mat result;
	cv::Mat greyMap(cv::Size(rgb.cols, rgb.rows), CV_8UC1);
	
	cv::cvtColor(rgb, hsv, CV_BGR2HSV);
	unsigned char* data = hsv.data;
	size_t total = hsv.total() * 3;
	
	unsigned char hueVariance = 10;
	unsigned char minSaturation = 150;
	unsigned char minValue = 50;
	unsigned char maxValue = 200;
	
	// Mask.
	for (unsigned int i = 0; i < total; i += 3) {
		if ((data[i] < hueVariance || data[i] > 255 - hueVariance) && data[i + 1] > minSaturation && data[i + 2] > minValue && data[i + 2] < maxValue) {
			data[i + 1] = 0;
			data[i + 2] = 255;
		} else {
			data[i + 2] = 0;
		}
	}

	// Show result.
	cv::cvtColor(hsv, result, CV_HSV2BGR);
	cv::imshow("depth", result);
	cv::imshow("rgb", rgb);

	//cv::waitKey();
	int rowSize = result.total() / result.rows;
	int colSize = result.total() / result.cols;
	
	// Convert to grey image.
	for (unsigned int i = 0; i < total / 3; i++) {
		greyMap.data[i] = hsv.data[(i * 3) + 2];
	}
	
	// Eventually remove small lines or points here by removing pixels with little neighbours.
	
	std::list<Area> areas;
	
	// Search for areas.
	for (unsigned int i = 0; i < greyMap.total(); i++) {
		if (greyMap.data[i] == 255) {
			Area area(i, greyMap.data, rowSize, colSize);
			
			if (area.getWidth() != 0 && area.getHeight() != 0) {
				areas.push_back(area);
			}
		}
	}
	
	// Draw areas to result.
	for (std::list<Area>::iterator iter = areas.begin(); iter != areas.end(); iter++) {
		iter->draw(result, 0x22DD22);
	}

	// Show result.
	//cv::imshow("depth", result);
	//cv::imshow("rgb", rgb);

	if (clock_gettime(CLOCK_REALTIME, &te) == -1) {
		std::cout << "Time error" << std::endl;
	}

	long int time = (te.tv_sec - ts.tv_sec) * 1000000000L + te.tv_nsec - ts.tv_nsec;
	std::cout << "Time: " << time << std::endl;

	cv::waitKey();
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
	
	for (std::list<cv::Mat>::iterator iter = images.begin(); iter != images.end(); iter++) {
		cv::imshow("rgb", *iter);
	
		//////////////////////////
		findObject(*iter);
		//////////////////////////
		
		cv::Mat greyMat(cv::Size(640, 480), CV_8UC1, cv::Scalar(0));
	
		cv::cvtColor(*iter, greyMat, CV_BGR2GRAY);
		std::vector<cv::Point2f> corners;
		
		bool found = cv::findChessboardCorners(greyMat, cv::Size(7, 7), corners);
		
		std::cout << "Corners found: " << corners.size() << std::endl;
	}
	
	cv::destroyWindow("depth");
	cv::destroyWindow("rgb");
	
	std::cout << "Exit" << std::endl;
	
	return 0;
}
