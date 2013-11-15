#include "libfreenect.hpp"
#include "Mutex.hpp"
#include "MyDevice.hpp"

MyDevice::MyDevice(freenect_context *_ctx, int _index)
			: Freenect::FreenectDevice(_ctx, _index), m_buffer_depth(FREENECT_DEPTH_11BIT),m_buffer_rgb(FREENECT_VIDEO_RGB), m_gamma(2048), m_new_rgb_frame(false), m_new_depth_frame(false),depthMat(cv::Size(640,480),CV_16UC1), rgbMat(cv::Size(640,480),CV_8UC3,cv::Scalar(0)), ownMat(cv::Size(640,480),CV_8UC3,cv::Scalar(0))
{
	for (unsigned int i = 0; i < 2049; i++) {
		float v = i/2048.0;
		v = std::pow(v, 3) * 6;
		m_gamma[i] = v * 6 * 256;
	}
}

// Do not call directly even in child
void MyDevice::VideoCallback(void* _rgb, uint32_t timestamp) {
	std::cout << "RGB callback" << std::endl;
	m_rgb_mutex.lock();
	uint8_t* rgb = static_cast<uint8_t*>(_rgb);
	rgbMat.data = rgb;
	m_new_rgb_frame = true;
	m_rgb_mutex.unlock();
}

// Do not call directly even in child
void MyDevice::DepthCallback(void* _depth, uint32_t timestamp) {
	std::cout << "Depth callback" << std::endl;
	m_depth_mutex.lock();
	uint16_t* depth = static_cast<uint16_t*>(_depth);
	depthMat.data = (uchar*) depth;
	m_new_depth_frame = true;
	m_depth_mutex.unlock();
}
	
bool MyDevice::getVideo(cv::Mat& output) {
	m_rgb_mutex.lock();
	
	if(m_new_rgb_frame) {
		cv::cvtColor(rgbMat, output, CV_RGB2BGR);
		m_new_rgb_frame = false;
		m_rgb_mutex.unlock();
		return true;
	} else {
		m_rgb_mutex.unlock();
		return false;
	}
}

bool MyDevice::getDepth(cv::Mat& output) {
	m_depth_mutex.lock();
	
	if(m_new_depth_frame) {
		depthMat.copyTo(output);
		m_new_depth_frame = false;
		m_depth_mutex.unlock();
		return true;
	} else {
		m_depth_mutex.unlock();
		return false;
	}
}
