#ifndef AREA_HPP
#define AREA_HPP

#include <cv.h>

class Area {
	private:
		unsigned int x, y;
		unsigned int width, height;
		unsigned int density;
		unsigned int borderSize;
		
	public:
		Area(unsigned int x, unsigned int y);
		Area(unsigned int position, unsigned char* data, unsigned int rowSize, unsigned int colSize);
		
		void addPoint(unsigned int x, unsigned int y);
		double getDensity();
		void draw(cv::Mat image, int bgr);
		
		unsigned int getX();
		unsigned int getY();
		unsigned int getWidth();
		unsigned int getHeight();
		
	private:
		inline unsigned char* getLocation(unsigned int x, unsigned int y, unsigned char* data, unsigned int rowSize, unsigned int colSize);
		void createArea(unsigned int x, unsigned int y, unsigned char* data, unsigned int rowSize, unsigned int colSize);
};

#endif // AREA_HPP
