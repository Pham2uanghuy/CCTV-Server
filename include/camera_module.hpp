#ifndef CAMERA_MODULE_HPP
#define CAMERA_MODULE_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class CameraModule
{
public:
    CameraModule(const std::string &baseUrl);
    bool getFrame(std::vector<unsigned char> &jpegBuffer);

private:
    cv::VideoCapture cap;
    std::string url;
};

#endif
