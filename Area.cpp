#include <exception>
#include <iostream>

#include "Area.hpp"

Area::Area(unsigned int x, unsigned int y) : density(0), borderSize(0), width(0), height(0) {
	this->x = x;
	this->y = y;
}

Area::Area(unsigned int position, unsigned char* data, unsigned int rowSize, unsigned int colSize) : density(0), borderSize(0), width(0), height(0) {
	this->x = position % rowSize;
	this->y = position / rowSize;
	
	this->createArea(x, y, data, rowSize, colSize);
}

void Area::addPoint(unsigned int x, unsigned int y) {
	this->density++;

	if (x < this->x) {
		this->width += this->x - x;
		this->x = x;
	}
	
	if (y < this->y) {
		this->height += this->y - y;
		this->y = y;
	}
	
	if (x > this->x + this->width) {
		this->width = x - this->x;
	}
	
	if (y > this->y + this->height) {
		this->height = y - this->y;
	}
}

double Area::getDensity() {
	return (double) this->density / ((this->width + 1) * (this->height + 1));
}

void Area::draw(cv::Mat image, int bgr) {
	unsigned char b = (bgr >> 16) & 0xFF;
	unsigned char g = (bgr >> 8) & 0xFF;
	unsigned char r = bgr & 0xFF;
	unsigned char* pixel;
	
	// std::cout << x << "/" << y << "/" << width << "/" << height << std::endl;
	
	for (unsigned int i = x; i <=  x + width; i++) {
		pixel = image.data + 3 * (i + y * image.cols);
		pixel[0] = b;
		pixel[1] = g;
		pixel[2] = r;
		
		// std::cout << std::hex << uint64_t(image.data) << "/" << std::hex << uint64_t(pixel) << std::endl;
		
		pixel = image.data + 3 * (i + (y + height) * image.cols);
		pixel[0] = b;
		pixel[1] = g;
		pixel[2] = r;
	}
	
	for (unsigned int j = y; j <= y + height; j++) {
		pixel = image.data + 3 * (x + j * image.cols);
		pixel[0] = b;
		pixel[1] = g;
		pixel[2] = r;
		
		pixel = image.data + 3 * (x + width + j * image.cols);
		pixel[0] = b;
		pixel[1] = g;
		pixel[2] = r;
	}
	
	for (unsigned int i = 0; i < 200; i++) {
		image.data[i] = 255;
	}
}

unsigned char* Area::getLocation(unsigned int x, unsigned int y, unsigned char* data, unsigned int rowSize, unsigned int colSize) {
	if (x >= rowSize || y >= colSize) {
		throw std::exception();
	} else {
		return data + x + y * rowSize;
	}
}

void Area::createArea(unsigned int x, unsigned int y, unsigned char* data, unsigned int rowSize, unsigned int colSize) {
	addPoint(x, y);
	(*getLocation(x, y, data, rowSize, colSize)) = 0;
	
	if (x < rowSize - 1) {
		if ((*getLocation(x + 1, y, data, rowSize, colSize)) == 255) {
			createArea(x + 1, y, data, rowSize, colSize);
		}
	}
	
	if (x > 0) {
		if ((*getLocation(x - 1, y, data, rowSize, colSize)) == 255) {
			createArea(x - 1, y, data, rowSize, colSize);
		}
	}
	
	if (y < colSize - 1) {
		if ((*getLocation(x, y + 1, data, rowSize, colSize)) == 255) {
			createArea(x, y + 1, data, rowSize, colSize);
		}
	}
	
	if (y > 0) {
		if ((*getLocation(x, y - 1, data, rowSize, colSize)) == 255) {
			createArea(x, y - 1, data, rowSize, colSize);
		}
	}
}

unsigned int Area::getX() {
	return x;
}

unsigned int Area::getY() {
	return y;
}

unsigned int Area::getWidth() {
	return width;
}

unsigned int Area::getHeight() {
	return height;
}
